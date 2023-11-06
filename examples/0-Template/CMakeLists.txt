cmake_minimum_required(VERSION 3.25)

set(CMAKE_VERBOSE_MAKEFILE OFF)
project(NUCLEO-F401)

include(${CMAKE_SOURCE_DIR}/arm-gcc-toolchain.cmake)

set(STM_HAL_LIBRARY_PATH    "../../extern/STM32CubeF4")
set(CMSIS_DEVICE_F4_PATH    "../../extern/cmsis_device_f4")

file(GLOB SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/startup_stm32f401xe.s
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    ${CMAKE_SOURCE_DIR}/lib/SEGGER_RTT/*.c
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/CMSIS/Core/Include
    ${CMAKE_SOURCE_DIR}/${CMSIS_DEVICE_F4_PATH}/Include
    ${CMAKE_SOURCE_DIR}/lib/SEGGER_RTT
    ${CMAKE_SOURCE_DIR}/${STM_HAL_LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Inc
)

target_compile_options(${PROJECT_NAME} PUBLIC
    -mthumb -mcpu=cortex-m4 -fdata-sections
    -ffunction-sections -Wall -g -O0
    -mfpu=fpv4-sp-d16 -mfloat-abi=hard 
    $<$<COMPILE_LANGUAGE:CXX>: -fno-exceptions -fno-rtti -std=c++17> #C++ Specific flags
)

target_link_options(${PROJECT_NAME} PUBLIC
    -T${CMAKE_SOURCE_DIR}/src/STM32F401RETx_FLASH.ld -mthumb -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16 -mfloat-abi=hard
    -mcpu=cortex-m4 -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections
    -static -Wl,--print-memory-usage
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

enable_language(ASM)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
)

add_custom_target(
    load
    COMMAND JLinkExe -CommandFile ../script.jlink
)