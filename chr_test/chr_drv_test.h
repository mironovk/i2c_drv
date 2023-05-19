#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
// #include <asm/uaccess.h>



#define DEVICE_NAME "BMP180_drv"
#define DEVICE_CLASS "MyI2CSensors"
#define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt, ##__VA_ARGS__)

//#define BMP180_address_RD 0xEE // 11101111
//#define BMP180_address_WR 0xEF // 11101110


#define DEVICE_BUFFER_SIZE 1024
static char temperature_buffer[DEVICE_BUFFER_SIZE];
static char pressure_buffer[DEVICE_BUFFER_SIZE];
static char altitude_buffer[DEVICE_BUFFER_SIZE];
static char EEPROM_buffer[DEVICE_BUFFER_SIZE];
static int deviceDataLength = 0;

//static struct i2c_adapter * bmp_i2c_adapter = NULL;
//static struct i2c_client * bmp180_i2c_client = NULL;

static int deviceMajorNum = 0;
static struct class *deviceClass = 0;
static struct device *deviceHandle = 0;
static int deviceOpenCount = 0;


int __init chr_drv_test_init(void);
void __exit chr_drv_test_exit(void);


static int i2c_drv_open(struct inode* Inode, struct file* File);
static int i2c_drv_release(struct inode* Inode, struct file* File);
static long int i2c_drv_ioctl(struct file* File, unsigned int Cmd, unsigned long Arg);

void Calculation(void);


static struct file_operations chr_drv_test_functions = {
	.owner = THIS_MODULE,
	.open = i2c_drv_open,
	.release = i2c_drv_release,
	.unlocked_ioctl = i2c_drv_ioctl
};
