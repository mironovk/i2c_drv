#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/i2c.h>

/* DEVICE_TREE:    arch/arm/boot/dts/stm32mp15xx-dkx.dtsi */ 
#define DEVICE_NAME "BMP180"
#define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt, ##__VA_ARGS__)

// #define SLAVE_DEVICE_NAME	"BMP180"	/* Device and Driver Name */
#define BMP180_ADDRESS 0x77
// #define BMP180_READ_ADDRESS	0xEF		/* BMP180 I2C address for read */
// #define BMP180_WRITE_ADDRESS	0xEE		/* BMP180 I2C address for write */
#define DELAY 4500  /* Calculation delay, us */
short oss = 0; /* Pressure Accuracy */

/* EEPROM data */
short AC1 = 0;
short AC2 = 0;
short AC3 = 0;
unsigned short AC4 = 0;
unsigned short AC5 = 0;
unsigned short AC6 = 0;
short B1 = 0;
short B2 = 0;
short MB = 0;
short MC = 0;
short MD = 0;

/* Uncompensated values */
long UT = 0;
long UP = 0;
u8 MSB = 0;
u8 LSB = 0;
u8 XLSB = 0;

/* Calculating Temperature */
long X1 = 0;
long X2 = 0;
long B5 = 0;
long B6 = 0;
long T = 0;

/* Calculating Pressure */

long X3 = 0;
long B3 = 0;
unsigned long B4 = 0;
unsigned long B7 = 0;
long p = 0;




static struct i2c_client *bmp180_client;

static int bmp180_probe (struct i2c_client *client, const struct i2c_device_id *id);

static int bmp180_remove (struct i2c_client *client);

/* drivers/gpio/gpio-pa9570.c */
static struct i2c_device_id bmp180_id_table[];

static struct of_device_id bmp180_of_match_table[];

/* drivers/gpio/gpio-pa9570.c */
static struct i2c_driver bmp180_driver;

void Calculation(void);

static int __init bmp180_init(void);

static void __exit bmp180_exit(void);


// #include <linux/init.h>
// #include <linux/kernel.h>
// #include <linux/module.h>
// #include <linux/device.h>
// #include <linux/slab.h>
// #include <linux/fs.h>


// #define DEVICE_NAME "BMP180_drv"
// #define DEVICE_CLASS "MySensors"
// #define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt, ##__VA_ARGS__)

// #define BMP180_address_RD 11101111
// #define BMP180_address_WR 11101110

// static struct i2c_adapter * bmp_i2c_adapter = NULL;
// static struct i2c_client * bmp180_i2c_client = NULL;

// static int deviceMajorNum = 0;
// static struct class* deviceClass = 0;
// static struct device* deviceHandle = 0;
// static int deviceOpenCount = 0;


// static int __init i2c_drv_init(void);
// static int __exit i2c_drv_exit(void);


// static int i2c_drv_open(struct inode* Inode, struct file* File);
// static int i2c_drv_release(struct inode* Inode, struct file* File);
// static long int i2c_drv_ioctl(struct file* File, unsigned int Cmd, unsigned long Arg);


// static struct file_operations test_drv_functions = {
// 	.owner = THIS_MODULE,
// 	.open = i2c_drv_open,
// 	.release = i2c_drv_release,
// 	.unlocked_ioctl = i2c_drv_ioctl
// };


// #define DEVICE_BUFFER_SIZE 1024

// static char deviceBuffer[DEVICE_BUFFER_SIZE];
// static int deviceDataLength = 0;