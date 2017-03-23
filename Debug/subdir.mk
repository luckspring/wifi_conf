################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../eap_register.c \
../main.c \
../wpa_cli.c 

O_SRCS += \
../eap_register.o \
../main.o \
../wpa_cli.o 

OBJS += \
./eap_register.o \
./main.o \
./wpa_cli.o 

C_DEPS += \
./eap_register.d \
./main.d \
./wpa_cli.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


