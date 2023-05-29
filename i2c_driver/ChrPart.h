#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "BMP180"
#define DEVICE_CLASS "MyI2CSensors"
#define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt, ##__VA_ARGS__)

#define DEVICE_BUFFER_SIZE 1024
static char temperature_buffer[DEVICE_BUFFER_SIZE];
static char pressure_buffer[DEVICE_BUFFER_SIZE];
static char altitude_buffer[DEVICE_BUFFER_SIZE];
static char EEPROM_buffer[DEVICE_BUFFER_SIZE];
static int deviceDataLength = 0;

static int deviceMajorNum = 0;
static struct class *deviceClass = 0;
static struct device *deviceHandle = 0;
static int deviceOpenCount = 0;

int __init i2cChrDrv_init(void);
void __exit i2cChrDrv_exit(void);

static int i2cChrDrv_open(struct inode* Inode, struct file* File);
static int i2cChrDrv_release(struct inode* Inode, struct file* File);
static long int i2cChrDrv_ioctl(struct file* File, unsigned int Cmd, unsigned long Arg);

void Calculation(void);

static struct file_operations i2cChrDrv_functions = {
	.owner = THIS_MODULE,
	.open = i2cChrDrv_open,
	.release = i2cChrDrv_release,
	.unlocked_ioctl = i2cChrDrv_ioctl
};