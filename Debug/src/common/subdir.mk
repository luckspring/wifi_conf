################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/common/ieee802_11_common.c \
../src/common/wpa_common.c \
../src/common/wpa_ctrl.c 

O_SRCS += \
../src/common/ieee802_11_common.o \
../src/common/wpa_common.o \
../src/common/wpa_ctrl.o 

OBJS += \
./src/common/ieee802_11_common.o \
./src/common/wpa_common.o \
./src/common/wpa_ctrl.o 

C_DEPS += \
./src/common/ieee802_11_common.d \
./src/common/wpa_common.d \
./src/common/wpa_ctrl.d 


# Each subdirectory must supply rules for building sources it contributes
src/common/%.o: ../src/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


