scp ~/Desktop/i2c_drv/cmd ~/Desktop/i2c_drv/DrvApp.c ~/Desktop/i2c_drv/i2c_drv.c ~/Desktop/i2c_drv/i2c_drv.h ~/Desktop/i2c_drv/Makefile kirillmironov@192.168.31.244:/Users/kirillmironov/GIT/i2c_drv

i2cdetect -l /* Список шин */

i2cdetect -y 1 /* Список устройств на шине 1 */

i2cget -y 2 0x28 0x1b /* Номер шины / ID устройства / Адрес регистра */

i2cdump -y 2 0x28 /* Номер шины / ID устройства – Считывает значения со всех регистров устройства */

i2cget -y 1 0xEE /* Device ID */

ls -l /sys/bus/i2c/devices/

sudo dtoverlay device_tree.dtbo


