# TAREFA 1 - CONVERSÃO AD: JOYSTICK + DISPLAY
## Aluno
Gabriel Neponuceno Batista

## 📺 Demonstração + Instruções de uso

- **Simulação no Wokwi:** [Assista no YouTube](https://www.youtube.com/watch?v=0Nvjn4rx-Uw&ab_channel=GABRIELNEPONUCENOBATISTA)

## Funcionalidades do Projeto

O joystick fornecerá valores analógicos correspondentes aos eixos X e Y, que serão utilizados para:

Controlar a intensidade luminosa dos LEDs RGB, onde:

  • O LED Azul terá seu brilho ajustado conforme o valor do eixo Y. Quando o joystick estiver solto
  (posição central - valor 2048), o LED permanecerá apagado. À medida que o joystick for movido para
  cima (valores menores) ou para baixo (valores maiores), o LED aumentará seu brilho gradualmente,
  atingindo a intensidade máxima nos extremos (0 e 4095).
  
  • O LED Vermelho seguirá o mesmo princípio, mas de acordo com o eixo X. Quando o joystick estiver
  solto (posição central - valor 2048), o LED estará apagado. Movendo o joystick para a esquerda
  (valores menores) ou para a direita (valores maiores), o LED aumentará de brilho, sendo mais intenso
  nos extremos (0 e 4095).
  
  • Os LEDs serão controlados via PWM para permitir variação suave da intensidade luminosa.
  Exibir no display SSD1306 um quadrado de 8x8 pixels, inicialmente centralizado, que se moverá
  proporcionalmente aos valores capturados pelo joystick.


Adicionalmente, o botão do joystick terá as seguintes funcionalidades:

  • Alternar o estado do LED Verde a cada acionamento.
  
  • Modificar a borda do display para indicar quando foi pressionado, alternando entre diferentes estilos
  de borda a cada novo acionamento.


Finalmente, o botão A terá a seguinte funcionalidade:
  • Ativar ou desativar os LED PWM a cada acionamento.

