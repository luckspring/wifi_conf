################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/base64.c \
../src/utils/common.c \
../src/utils/edit.c \
../src/utils/edit_readline.c \
../src/utils/edit_simple.c \
../src/utils/eloop.c \
../src/utils/eloop_none.c \
../src/utils/eloop_win.c \
../src/utils/ip_addr.c \
../src/utils/os_internal.c \
../src/utils/os_none.c \
../src/utils/os_unix.c \
../src/utils/os_win32.c \
../src/utils/pcsc_funcs.c \
../src/utils/radiotap.c \
../src/utils/trace.c \
../src/utils/uuid.c \
../src/utils/wpa_debug.c \
../src/utils/wpabuf.c 

O_SRCS += \
../src/utils/base64.o \
../src/utils/common.o \
../src/utils/edit.o \
../src/utils/edit_simple.o \
../src/utils/eloop.o \
../src/utils/os_unix.o \
../src/utils/uuid.o \
../src/utils/wpa_debug.o \
../src/utils/wpabuf.o 

OBJS += \
./src/utils/base64.o \
./src/utils/common.o \
./src/utils/edit.o \
./src/utils/edit_readline.o \
./src/utils/edit_simple.o \
./src/utils/eloop.o \
./src/utils/eloop_none.o \
./src/utils/eloop_win.o \
./src/utils/ip_addr.o \
./src/utils/os_internal.o \
./src/utils/os_none.o \
./src/utils/os_unix.o \
./src/utils/os_win32.o \
./src/utils/pcsc_funcs.o \
./src/utils/radiotap.o \
./src/utils/trace.o \
./src/utils/uuid.o \
./src/utils/wpa_debug.o \
./src/utils/wpabuf.o 

C_DEPS += \
./src/utils/base64.d \
./src/utils/common.d \
./src/utils/edit.d \
./src/utils/edit_readline.d \
./src/utils/edit_simple.d \
./src/utils/eloop.d \
./src/utils/eloop_none.d \
./src/utils/eloop_win.d \
./src/utils/ip_addr.d \
./src/utils/os_internal.d \
./src/utils/os_none.d \
./src/utils/os_unix.d \
./src/utils/os_win32.d \
./src/utils/pcsc_funcs.d \
./src/utils/radiotap.d \
./src/utils/trace.d \
./src/utils/uuid.d \
./src/utils/wpa_debug.d \
./src/utils/wpabuf.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


