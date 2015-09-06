################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mkespfsimage/heatshrink_encoder.c \
../mkespfsimage/main.c 

O_SRCS += \
../mkespfsimage/heatshrink_encoder.o \
../mkespfsimage/main.o 

OBJS += \
./mkespfsimage/heatshrink_encoder.o \
./mkespfsimage/main.o 

C_DEPS += \
./mkespfsimage/heatshrink_encoder.d \
./mkespfsimage/main.d 


# Each subdirectory must supply rules for building sources it contributes
mkespfsimage/%.o: ../mkespfsimage/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


