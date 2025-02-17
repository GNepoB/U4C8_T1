# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico CACHE STRING "Board type")

# Importa o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

# Define o projeto
project(ADC_DisplayC C CXX ASM)
pico_sdk_init()

# Adiciona os arquivos fonte do projeto
add_executable(ADC_DisplayC ADC_DisplayC.c lib/ssd1306.c)

# Configura nome e versão do programa
pico_set_program_name(ADC_DisplayC "ADC_DisplayC")
pico_set_program_version(ADC_DisplayC "0.1")

# Habilita saída padrão via USB
pico_enable_stdio_usb(ADC_DisplayC 1)
pico_enable_stdio_uart(ADC_DisplayC 0)

# Inclui diretórios necessários
target_include_directories(ADC_DisplayC PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Linka as bibliotecas necessárias
target_link_libraries(ADC_DisplayC 
    pico_stdlib 
    hardware_i2c 
    hardware_adc 
    hardware_pwm  # Adiciona suporte a PWM
)

# Gera arquivos binários extras (UF2, BIN, HEX, etc.)
pico_add_extra_outputs(ADC_DisplayC)
