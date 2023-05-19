#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>


#define DEVICE_NAME "BMP180_drv"
#define DEVICE_CLASS "MySensors"
#define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt, ##__VA_ARGS__)

#define BMP180_address_RD 11101111
#define BMP180_address_WR 11101110

static struct i2c_adapter * bmp_i2c_adapter = NULL;
static struct i2c_client * bmp180_i2c_client = NULL;

static int deviceMajorNum = 0;
static struct class* deviceClass = 0;
static struct device* deviceHandle = 0;
static int deviceOpenCount = 0;


static int __init i2c_drv_init(void);
static int __exit i2c_drv_exit(void);


static int i2c_drv_open(struct inode* Inode, struct file* File);
static int i2c_drv_release(struct inode* Inode, struct file* File);
static long int i2c_drv_ioctl(struct file* File, unsigned int Cmd, unsigned long Arg);


static struct file_operations test_drv_functions = {
	.owner = THIS_MODULE,
	.open = i2c_drv_open,
	.release = i2c_drv_release,
	.unlocked_ioctl = i2c_drv_ioctl
};


#define DEVICE_BUFFER_SIZE 1024

static char deviceBuffer[DEVICE_BUFFER_SIZE];
static int deviceDataLength = 0;









