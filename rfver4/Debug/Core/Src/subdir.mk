################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/dma.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/max5980.c \
../Core/Src/string.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/tim.c \
../Core/Src/uart.c 

OBJS += \
./Core/Src/dma.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/max5980.o \
./Core/Src/string.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/tim.o \
./Core/Src/uart.o 

C_DEPS += \
./Core/Src/dma.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/max5980.d \
./Core/Src/string.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/tim.d \
./Core/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -c -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/max5980.cyclo ./Core/Src/max5980.d ./Core/Src/max5980.o ./Core/Src/max5980.su ./Core/Src/string.cyclo ./Core/Src/string.d ./Core/Src/string.o ./Core/Src/string.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su

.PHONY: clean-Core-2f-Src

