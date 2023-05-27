#include <linux/i2c.h>
#include <linux/delay.h>

#define DEVICE_NAME "BMP180"    /* Device and Driver Name */
// #define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt, ##__VA_ARGS__)
	
#define BMP180_ADDRESS 0x77 /* BMP180 I2C address */

short oss = 0; /* Pressure Accuracy */

/* EEPROM data */
short AC1 = 0;
short AC1_1 = 0;
short AC1_2 = 0;
short AC2 = 0;
short AC2_1 = 0;
short AC2_2 = 0;
short AC3 = 0;
short AC3_1 = 0;
short AC3_2 = 0;
unsigned short AC4 = 0;
unsigned short AC4_1 = 0;
unsigned short AC4_2 = 0;
unsigned short AC5 = 0;
unsigned short AC5_1 = 0;
unsigned short AC5_2 = 0;
unsigned short AC6 = 0;
unsigned short AC6_1 = 0;
unsigned short AC6_2 = 0;
short B1 = 0;
short B1_1 = 0;
short B1_2 = 0;
short B2 = 0;
short B2_1 = 0;
short B2_2 = 0;
short MB = 0;
short MB_1 = 0;
short MB_2 = 0;
short MC = 0;
short MC_1 = 0;
short MC_2 = 0;
short MD = 0;
short MD_1 = 0;
short MD_2 = 0;

/* Uncompensated values */
long UT_1 = 0;
long UT_2 = 0;
long UT = 0;
long UP = 0;
short MSB = 0;
short LSB = 0;
short XLSB = 0;

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

/* Calculating Altitude */

long p0 = 101325;
long Altitude = 0;


static int bmp180_probe (struct i2c_client *client, const struct i2c_device_id *id);
static int bmp180_remove (struct i2c_client *client);


static struct i2c_client *bmp180_client;
static struct i2c_device_id bmp180_id_table[];
static struct i2c_driver bmp180_driver;
static struct i2c_adapter* bmp180_adapter;

// static struct of_device_id bmp180_of_match_table[];

void Calculation(void);
void ReadEEPROM(void);