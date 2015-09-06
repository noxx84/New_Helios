################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modules/config.c \
../modules/wifi.c 

OBJS += \
./modules/config.o \
./modules/wifi.o 

C_DEPS += \
./modules/config.d \
./modules/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
modules/%.o: ../modules/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


