CC=arm-fsl-linux-gnueabi-gcc

CFLAGS = -O2 -Wall -g -fPIC -I./src

OBJ=./src/common/wpa_ctrl.o\
       eap_register.o\
      ./src/utils/eloop.o\
      ./src/utils/edit_simple.o\
      ./src/utils/os_unix.o\
      ./src/utils/wpa_debug.o\
      ./main.o\
      ./wpa_cli.o\
      
LDFLAGS=-lm -ldl -lpthread      


connect: $(OBJ)
	$(CC)  -o $@ $^ $(LDFLAGS)  

clean:
	rm connect $(OBJ)
