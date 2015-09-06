################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mqtt/mqtt.c \
../mqtt/mqtt_msg.c \
../mqtt/proto.c \
../mqtt/queue.c \
../mqtt/ringbuf.c \
../mqtt/utils.c 

OBJS += \
./mqtt/mqtt.o \
./mqtt/mqtt_msg.o \
./mqtt/proto.o \
./mqtt/queue.o \
./mqtt/ringbuf.o \
./mqtt/utils.o 

C_DEPS += \
./mqtt/mqtt.d \
./mqtt/mqtt_msg.d \
./mqtt/proto.d \
./mqtt/queue.d \
./mqtt/ringbuf.d \
./mqtt/utils.d 


# Each subdirectory must supply rules for building sources it contributes
mqtt/%.o: ../mqtt/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


