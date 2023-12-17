################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/cc1101.c \
../Core/Src/dma.c \
../Core/Src/main.c \
../Core/Src/neo6m.c \
../Core/Src/protokol.c \
../Core/Src/si4432.c \
../Core/Src/spi.c \
../Core/Src/string.c \
../Core/Src/tim.c \
../Core/Src/uart.c 

OBJS += \
./Core/Src/cc1101.o \
./Core/Src/dma.o \
./Core/Src/main.o \
./Core/Src/neo6m.o \
./Core/Src/protokol.o \
./Core/Src/si4432.o \
./Core/Src/spi.o \
./Core/Src/string.o \
./Core/Src/tim.o \
./Core/Src/uart.o 

C_DEPS += \
./Core/Src/cc1101.d \
./Core/Src/dma.d \
./Core/Src/main.d \
./Core/Src/neo6m.d \
./Core/Src/protokol.d \
./Core/Src/si4432.d \
./Core/Src/spi.d \
./Core/Src/string.d \
./Core/Src/tim.d \
./Core/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -c -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/cc1101.cyclo ./Core/Src/cc1101.d ./Core/Src/cc1101.o ./Core/Src/cc1101.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/neo6m.cyclo ./Core/Src/neo6m.d ./Core/Src/neo6m.o ./Core/Src/neo6m.su ./Core/Src/protokol.cyclo ./Core/Src/protokol.d ./Core/Src/protokol.o ./Core/Src/protokol.su ./Core/Src/si4432.cyclo ./Core/Src/si4432.d ./Core/Src/si4432.o ./Core/Src/si4432.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/string.cyclo ./Core/Src/string.d ./Core/Src/string.o ./Core/Src/string.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su

.PHONY: clean-Core-2f-Src

