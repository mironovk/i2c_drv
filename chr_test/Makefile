TARGET := chr_drv_test
PWD := $(shell pwd)
KERNEL := $(shell uname -r)

obj-m := $(TARGET).o
//$(TARGET)-objs += chr_drv_test.o

all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean
