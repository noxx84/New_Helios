################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/auth.c \
../user/base64.c \
../user/cgi.c \
../user/cgiOneWire.c \
../user/cgiflash.c \
../user/cgiwifi.c \
../user/ds1820.c \
../user/espfs.c \
../user/heatshrink_decoder.c \
../user/httpd.c \
../user/httpdespfs.c \
../user/io.c \
../user/stdout.c \
../user/user_main.c 

OBJS += \
./user/auth.o \
./user/base64.o \
./user/cgi.o \
./user/cgiOneWire.o \
./user/cgiflash.o \
./user/cgiwifi.o \
./user/ds1820.o \
./user/espfs.o \
./user/heatshrink_decoder.o \
./user/httpd.o \
./user/httpdespfs.o \
./user/io.o \
./user/stdout.o \
./user/user_main.o 

C_DEPS += \
./user/auth.d \
./user/base64.d \
./user/cgi.d \
./user/cgiOneWire.d \
./user/cgiflash.d \
./user/cgiwifi.d \
./user/ds1820.d \
./user/espfs.d \
./user/heatshrink_decoder.d \
./user/httpd.d \
./user/httpdespfs.d \
./user/io.d \
./user/stdout.d \
./user/user_main.d 


# Each subdirectory must supply rules for building sources it contributes
user/%.o: ../user/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


