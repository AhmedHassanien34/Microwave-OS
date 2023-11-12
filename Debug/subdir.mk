################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CLCD_program.c \
../DCM_program.c \
../DELAY_Program.c \
../DIO_program.c \
../KPD_program.c \
../LED_program.c \
../Math_Program.c \
../RTOS_program.c \
../TIMER_program.c \
../main.c 

OBJS += \
./CLCD_program.o \
./DCM_program.o \
./DELAY_Program.o \
./DIO_program.o \
./KPD_program.o \
./LED_program.o \
./Math_Program.o \
./RTOS_program.o \
./TIMER_program.o \
./main.o 

C_DEPS += \
./CLCD_program.d \
./DCM_program.d \
./DELAY_Program.d \
./DIO_program.d \
./KPD_program.d \
./LED_program.d \
./Math_Program.d \
./RTOS_program.d \
./TIMER_program.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


