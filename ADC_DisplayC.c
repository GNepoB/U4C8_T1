#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "hardware/pwm.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define Joystick_X 26  // GPIO para eixo X
#define Joystick_Y 27  // GPIO para eixo Y
#define botao_Joystick 22 // GPIO para botão do Joystick
#define botao_A 5 // GPIO para botão A
#define LED_verde 11
#define LED_azul 12
#define LED_vermelho 13

ssd1306_t ssd; // Inicializa a estrutura do display
bool cor = true;
volatile bool espessura = false;
static void gpio_irq_handler(uint gpio, uint32_t events);

bool estado_LED_verde = false;

void setup_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice, 4095);  // Máximo de 4095 para corresponder ao ADC
    pwm_set_chan_level(slice, PWM_CHAN_A, 0);
    pwm_set_chan_level(slice, PWM_CHAN_B, 0);
    pwm_set_enabled(slice, true);
}

int main()
{
    gpio_init(botao_A);
    gpio_set_dir(botao_A, GPIO_IN);
    gpio_pull_up(botao_A);

    gpio_init(botao_Joystick);
    gpio_set_dir(botao_Joystick, GPIO_IN);
    gpio_pull_up(botao_Joystick);

    gpio_init(LED_azul);
    gpio_set_dir(LED_azul, GPIO_OUT);
    gpio_init(LED_vermelho);
    gpio_set_dir(LED_vermelho, GPIO_OUT);
    gpio_init(LED_verde);
    gpio_set_dir(LED_verde, GPIO_OUT);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    adc_init();
    adc_gpio_init(Joystick_X);
    adc_gpio_init(Joystick_Y);  
    
    setup_pwm(LED_azul);
    setup_pwm(LED_vermelho);

    uint slice_azul = pwm_gpio_to_slice_num(LED_azul);
    uint slice_vermelho = pwm_gpio_to_slice_num(LED_vermelho);

    uint16_t adc_value_x;
    uint16_t adc_value_y;  

    gpio_set_irq_enabled_with_callback(botao_Joystick, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(botao_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    while (true)
    {
        adc_select_input(0); // Seleciona o ADC para eixo X. O pino 26 como entrada analógica
        adc_value_x = adc_read();
        int diff_x = adc_value_x - 2047;  // Valor de -2047 a +2047
        uint16_t intensidade_X = abs(diff_x);
        pwm_set_chan_level(slice_vermelho, PWM_CHAN_B, intensidade_X);

        adc_select_input(1); // Seleciona o ADC para eixo Y. O pino 27 como entrada analógica
        adc_value_y = adc_read();    
        int diff_y = adc_value_y - 2047;
        uint16_t intensidade_Y = abs(diff_y);
        pwm_set_chan_level(slice_azul, PWM_CHAN_A, intensidade_Y);

        int pos_x = 60 - (diff_x * 56) / 2047;  // Centraliza em 60 e inverte o eixo X
        int pos_y = 28 - (diff_y * 24) / 2047;  // Centraliza em 28 (eixo Y)

        // Garante que o quadrado não saia da tela
        if (pos_x < 0) pos_x = 0;
        if (pos_x > 120) pos_x = 120; // 128 - 8 (tamanho do quadrado)
        if (pos_y < 0) pos_y = 0;
        if (pos_y > 56) pos_y = 56;   // 64 - 8 (tamanho do quadrado)

        // Atualiza o display
        ssd1306_fill(&ssd, false); // Limpa a tela
        ssd1306_rect(&ssd, 0, 0, 128, 64, cor, !cor);
        if(espessura == 1){
            ssd1306_rect(&ssd, 1, 1, 126, 62, cor, !cor);
            ssd1306_rect(&ssd, 2, 2, 124, 60, cor, !cor);
        }
        ssd1306_rect(&ssd, pos_y, pos_x, 8, 8, true, true); // Desenha o quadrado na nova posição
        ssd1306_send_data(&ssd); // Envia os dados para o display

        sleep_ms(100);
    }
}

void gpio_irq_handler(uint gpio, uint32_t events){
    static bool estado_PWM = true; // Variável para armazenar o estado atual do PWM

    if(gpio_get(botao_Joystick) == 0 && gpio_get(botao_A) == 1){
        sleep_ms(40); // Debouncing simples
        estado_LED_verde = !estado_LED_verde;
        gpio_put(LED_verde, estado_LED_verde);
    }
    
    if(gpio_get(botao_Joystick) == 1 && gpio_get(botao_A) == 0){
        sleep_ms(40); // Debouncing simples
 
        estado_PWM = !estado_PWM; // Alterna o estado do PWM
        
        uint slice_azul = pwm_gpio_to_slice_num(LED_azul);
        uint slice_vermelho = pwm_gpio_to_slice_num(LED_vermelho);

        if (estado_PWM) {
            pwm_set_enabled(slice_azul, true);
            pwm_set_enabled(slice_vermelho, true);
        } else {
            pwm_set_chan_level(slice_vermelho, PWM_CHAN_B, 0);
            pwm_set_chan_level(slice_azul, PWM_CHAN_A, 0);
            pwm_set_enabled(slice_azul, false);
            pwm_set_enabled(slice_vermelho, false);
        }
        espessura = !espessura;
    }
}