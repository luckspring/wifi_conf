################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drivers/MobileApple80211.c \
../src/drivers/driver_atheros.c \
../src/drivers/driver_broadcom.c \
../src/drivers/driver_bsd.c \
../src/drivers/driver_hostap.c \
../src/drivers/driver_madwifi.c \
../src/drivers/driver_ndis.c \
../src/drivers/driver_ndis_.c \
../src/drivers/driver_nl80211.c \
../src/drivers/driver_none.c \
../src/drivers/driver_privsep.c \
../src/drivers/driver_ralink.c \
../src/drivers/driver_roboswitch.c \
../src/drivers/driver_rtw.c \
../src/drivers/driver_test.c \
../src/drivers/driver_wext.c \
../src/drivers/driver_wired.c \
../src/drivers/drivers.c \
../src/drivers/linux_ioctl.c \
../src/drivers/ndis_events.c \
../src/drivers/netlink.c \
../src/drivers/rfkill.c 

O_SRCS += \
../src/drivers/driver_wext.o \
../src/drivers/drivers.o \
../src/drivers/linux_ioctl.o \
../src/drivers/netlink.o \
../src/drivers/rfkill.o 

OBJS += \
./src/drivers/MobileApple80211.o \
./src/drivers/driver_atheros.o \
./src/drivers/driver_broadcom.o \
./src/drivers/driver_bsd.o \
./src/drivers/driver_hostap.o \
./src/drivers/driver_madwifi.o \
./src/drivers/driver_ndis.o \
./src/drivers/driver_ndis_.o \
./src/drivers/driver_nl80211.o \
./src/drivers/driver_none.o \
./src/drivers/driver_privsep.o \
./src/drivers/driver_ralink.o \
./src/drivers/driver_roboswitch.o \
./src/drivers/driver_rtw.o \
./src/drivers/driver_test.o \
./src/drivers/driver_wext.o \
./src/drivers/driver_wired.o \
./src/drivers/drivers.o \
./src/drivers/linux_ioctl.o \
./src/drivers/ndis_events.o \
./src/drivers/netlink.o \
./src/drivers/rfkill.o 

C_DEPS += \
./src/drivers/MobileApple80211.d \
./src/drivers/driver_atheros.d \
./src/drivers/driver_broadcom.d \
./src/drivers/driver_bsd.d \
./src/drivers/driver_hostap.d \
./src/drivers/driver_madwifi.d \
./src/drivers/driver_ndis.d \
./src/drivers/driver_ndis_.d \
./src/drivers/driver_nl80211.d \
./src/drivers/driver_none.d \
./src/drivers/driver_privsep.d \
./src/drivers/driver_ralink.d \
./src/drivers/driver_roboswitch.d \
./src/drivers/driver_rtw.d \
./src/drivers/driver_test.d \
./src/drivers/driver_wext.d \
./src/drivers/driver_wired.d \
./src/drivers/drivers.d \
./src/drivers/linux_ioctl.d \
./src/drivers/ndis_events.d \
./src/drivers/netlink.d \
./src/drivers/rfkill.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/%.o: ../src/drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


