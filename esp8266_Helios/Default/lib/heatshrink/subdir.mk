################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/heatshrink/heatshrink.c \
../lib/heatshrink/heatshrink_decoder.c \
../lib/heatshrink/heatshrink_encoder.c \
../lib/heatshrink/test_heatshrink_dynamic.c \
../lib/heatshrink/test_heatshrink_dynamic_theft.c \
../lib/heatshrink/test_heatshrink_static.c 

OBJS += \
./lib/heatshrink/heatshrink.o \
./lib/heatshrink/heatshrink_decoder.o \
./lib/heatshrink/heatshrink_encoder.o \
./lib/heatshrink/test_heatshrink_dynamic.o \
./lib/heatshrink/test_heatshrink_dynamic_theft.o \
./lib/heatshrink/test_heatshrink_static.o 

C_DEPS += \
./lib/heatshrink/heatshrink.d \
./lib/heatshrink/heatshrink_decoder.d \
./lib/heatshrink/heatshrink_encoder.d \
./lib/heatshrink/test_heatshrink_dynamic.d \
./lib/heatshrink/test_heatshrink_dynamic_theft.d \
./lib/heatshrink/test_heatshrink_static.d 


# Each subdirectory must supply rules for building sources it contributes
lib/heatshrink/%.o: ../lib/heatshrink/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


