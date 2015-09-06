################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../espfstest/espfs.c \
../espfstest/heatshrink_decoder.c \
../espfstest/main.c 

OBJS += \
./espfstest/espfs.o \
./espfstest/heatshrink_decoder.o \
./espfstest/main.o 

C_DEPS += \
./espfstest/espfs.d \
./espfstest/heatshrink_decoder.d \
./espfstest/main.d 


# Each subdirectory must supply rules for building sources it contributes
espfstest/%.o: ../espfstest/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


