################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/eap_peer/eap.c \
../src/eap_peer/eap_aka.c \
../src/eap_peer/eap_fast.c \
../src/eap_peer/eap_fast_pac.c \
../src/eap_peer/eap_gpsk.c \
../src/eap_peer/eap_gtc.c \
../src/eap_peer/eap_ikev2.c \
../src/eap_peer/eap_leap.c \
../src/eap_peer/eap_md5.c \
../src/eap_peer/eap_methods.c \
../src/eap_peer/eap_mschapv2.c \
../src/eap_peer/eap_otp.c \
../src/eap_peer/eap_pax.c \
../src/eap_peer/eap_peap.c \
../src/eap_peer/eap_psk.c \
../src/eap_peer/eap_pwd.c \
../src/eap_peer/eap_sake.c \
../src/eap_peer/eap_sim.c \
../src/eap_peer/eap_tls.c \
../src/eap_peer/eap_tls_common.c \
../src/eap_peer/eap_tnc.c \
../src/eap_peer/eap_ttls.c \
../src/eap_peer/eap_vendor_test.c \
../src/eap_peer/eap_wsc.c \
../src/eap_peer/ikev2.c \
../src/eap_peer/mschapv2.c \
../src/eap_peer/tncc.c 

O_SRCS += \
../src/eap_peer/eap.o \
../src/eap_peer/eap_gtc.o \
../src/eap_peer/eap_leap.o \
../src/eap_peer/eap_md5.o \
../src/eap_peer/eap_methods.o \
../src/eap_peer/eap_mschapv2.o \
../src/eap_peer/eap_otp.o \
../src/eap_peer/eap_peap.o \
../src/eap_peer/eap_tls.o \
../src/eap_peer/eap_tls_common.o \
../src/eap_peer/eap_ttls.o \
../src/eap_peer/eap_wsc.o \
../src/eap_peer/mschapv2.o 

OBJS += \
./src/eap_peer/eap.o \
./src/eap_peer/eap_aka.o \
./src/eap_peer/eap_fast.o \
./src/eap_peer/eap_fast_pac.o \
./src/eap_peer/eap_gpsk.o \
./src/eap_peer/eap_gtc.o \
./src/eap_peer/eap_ikev2.o \
./src/eap_peer/eap_leap.o \
./src/eap_peer/eap_md5.o \
./src/eap_peer/eap_methods.o \
./src/eap_peer/eap_mschapv2.o \
./src/eap_peer/eap_otp.o \
./src/eap_peer/eap_pax.o \
./src/eap_peer/eap_peap.o \
./src/eap_peer/eap_psk.o \
./src/eap_peer/eap_pwd.o \
./src/eap_peer/eap_sake.o \
./src/eap_peer/eap_sim.o \
./src/eap_peer/eap_tls.o \
./src/eap_peer/eap_tls_common.o \
./src/eap_peer/eap_tnc.o \
./src/eap_peer/eap_ttls.o \
./src/eap_peer/eap_vendor_test.o \
./src/eap_peer/eap_wsc.o \
./src/eap_peer/ikev2.o \
./src/eap_peer/mschapv2.o \
./src/eap_peer/tncc.o 

C_DEPS += \
./src/eap_peer/eap.d \
./src/eap_peer/eap_aka.d \
./src/eap_peer/eap_fast.d \
./src/eap_peer/eap_fast_pac.d \
./src/eap_peer/eap_gpsk.d \
./src/eap_peer/eap_gtc.d \
./src/eap_peer/eap_ikev2.d \
./src/eap_peer/eap_leap.d \
./src/eap_peer/eap_md5.d \
./src/eap_peer/eap_methods.d \
./src/eap_peer/eap_mschapv2.d \
./src/eap_peer/eap_otp.d \
./src/eap_peer/eap_pax.d \
./src/eap_peer/eap_peap.d \
./src/eap_peer/eap_psk.d \
./src/eap_peer/eap_pwd.d \
./src/eap_peer/eap_sake.d \
./src/eap_peer/eap_sim.d \
./src/eap_peer/eap_tls.d \
./src/eap_peer/eap_tls_common.d \
./src/eap_peer/eap_tnc.d \
./src/eap_peer/eap_ttls.d \
./src/eap_peer/eap_vendor_test.d \
./src/eap_peer/eap_wsc.d \
./src/eap_peer/ikev2.d \
./src/eap_peer/mschapv2.d \
./src/eap_peer/tncc.d 


# Each subdirectory must supply rules for building sources it contributes
src/eap_peer/%.o: ../src/eap_peer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


