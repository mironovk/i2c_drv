TARGET := i2cChrDrv
PWD := $(shell pwd)
KERNEL := $(shell uname -r)

obj-m := $(TARGET).o
//$(TARGET)-objs += chr_drv_test.o

all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean

dt: device_tree.dts
	dtc -@ -I dts -O dtb -o device_tree.dtbo device_tree.dts
