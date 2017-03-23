################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/eap_server/eap_server.c \
../src/eap_server/eap_server_aka.c \
../src/eap_server/eap_server_fast.c \
../src/eap_server/eap_server_gpsk.c \
../src/eap_server/eap_server_gtc.c \
../src/eap_server/eap_server_identity.c \
../src/eap_server/eap_server_ikev2.c \
../src/eap_server/eap_server_md5.c \
../src/eap_server/eap_server_methods.c \
../src/eap_server/eap_server_mschapv2.c \
../src/eap_server/eap_server_pax.c \
../src/eap_server/eap_server_peap.c \
../src/eap_server/eap_server_psk.c \
../src/eap_server/eap_server_pwd.c \
../src/eap_server/eap_server_sake.c \
../src/eap_server/eap_server_sim.c \
../src/eap_server/eap_server_tls.c \
../src/eap_server/eap_server_tls_common.c \
../src/eap_server/eap_server_tnc.c \
../src/eap_server/eap_server_ttls.c \
../src/eap_server/eap_server_vendor_test.c \
../src/eap_server/eap_server_wsc.c \
../src/eap_server/eap_sim_db.c \
../src/eap_server/ikev2.c \
../src/eap_server/tncs.c 

OBJS += \
./src/eap_server/eap_server.o \
./src/eap_server/eap_server_aka.o \
./src/eap_server/eap_server_fast.o \
./src/eap_server/eap_server_gpsk.o \
./src/eap_server/eap_server_gtc.o \
./src/eap_server/eap_server_identity.o \
./src/eap_server/eap_server_ikev2.o \
./src/eap_server/eap_server_md5.o \
./src/eap_server/eap_server_methods.o \
./src/eap_server/eap_server_mschapv2.o \
./src/eap_server/eap_server_pax.o \
./src/eap_server/eap_server_peap.o \
./src/eap_server/eap_server_psk.o \
./src/eap_server/eap_server_pwd.o \
./src/eap_server/eap_server_sake.o \
./src/eap_server/eap_server_sim.o \
./src/eap_server/eap_server_tls.o \
./src/eap_server/eap_server_tls_common.o \
./src/eap_server/eap_server_tnc.o \
./src/eap_server/eap_server_ttls.o \
./src/eap_server/eap_server_vendor_test.o \
./src/eap_server/eap_server_wsc.o \
./src/eap_server/eap_sim_db.o \
./src/eap_server/ikev2.o \
./src/eap_server/tncs.o 

C_DEPS += \
./src/eap_server/eap_server.d \
./src/eap_server/eap_server_aka.d \
./src/eap_server/eap_server_fast.d \
./src/eap_server/eap_server_gpsk.d \
./src/eap_server/eap_server_gtc.d \
./src/eap_server/eap_server_identity.d \
./src/eap_server/eap_server_ikev2.d \
./src/eap_server/eap_server_md5.d \
./src/eap_server/eap_server_methods.d \
./src/eap_server/eap_server_mschapv2.d \
./src/eap_server/eap_server_pax.d \
./src/eap_server/eap_server_peap.d \
./src/eap_server/eap_server_psk.d \
./src/eap_server/eap_server_pwd.d \
./src/eap_server/eap_server_sake.d \
./src/eap_server/eap_server_sim.d \
./src/eap_server/eap_server_tls.d \
./src/eap_server/eap_server_tls_common.d \
./src/eap_server/eap_server_tnc.d \
./src/eap_server/eap_server_ttls.d \
./src/eap_server/eap_server_vendor_test.d \
./src/eap_server/eap_server_wsc.d \
./src/eap_server/eap_sim_db.d \
./src/eap_server/ikev2.d \
./src/eap_server/tncs.d 


# Each subdirectory must supply rules for building sources it contributes
src/eap_server/%.o: ../src/eap_server/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


