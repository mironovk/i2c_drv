#include "chr_drv_test.h"
#include "chr_drv_ioctl.h"

MODULE_LICENSE("Dual MIT/GPL");


int __init chr_drv_test_init(void)
{
	//printk("chr_drv_test_init called.");
	DBGMSG("i2c_drv_init called.\n");
	deviceMajorNum = register_chrdev(0, DEVICE_NAME, &chr_drv_test_functions);
	if(deviceMajorNum < 0)
	{
		DBGMSG("Register device failed\n");
		return -1;
	}
	else
	{
		DBGMSG("Device Major number = %d\n", deviceMajorNum);	
	}

	deviceClass = class_create(THIS_MODULE, DEVICE_CLASS);
	if(deviceClass == 0)
	{
		DBGMSG("Class create failed\n");
		unregister_chrdev(deviceMajorNum, DEVICE_NAME);
		return -1;
	}

	deviceHandle = device_create(deviceClass, NULL, MKDEV(deviceMajorNum, 0), NULL, DEVICE_NAME);
	if(deviceHandle == 0)
	{
		DBGMSG("Device create failed\n");
		class_unregister(deviceClass);
		class_destroy(deviceClass);
		unregister_chrdev(deviceMajorNum, DEVICE_NAME);
		return -1;
	}

	DBGMSG("i2c_drv_init completed!\n");
	return 0;
}

void __exit chr_drv_test_exit(void)
{
	DBGMSG("i2c_drv_exit called.\n");
	
	device_destroy(deviceClass, MKDEV(deviceMajorNum, 0));
	class_unregister(deviceClass);
	class_destroy(deviceClass);
	unregister_chrdev(deviceMajorNum, DEVICE_NAME);
}


static int i2c_drv_open(struct inode* Inode, struct file* File)
{
	if(deviceOpenCount == 0)
	{
		DBGMSG("i2c_drv_open called.\n");
		deviceOpenCount++;
		return 0;
	}
	else
	{
		DBGMSG("device already opened!\n");
		return -1;
	}
}

static int i2c_drv_release(struct inode* Inode, struct file* File)
{
	DBGMSG("i2c_drv_release called.\n");
	deviceOpenCount--;
	return 0;
}

static long int i2c_drv_ioctl(struct file* File, unsigned int Cmd, unsigned long Arg)
{
	DBGMSG("i2c_drv_ioctl called.\n");
	// char* data = (char*)Arg;
	drv_ioctl_data* data = (drv_ioctl_data*)Arg;
	// DBGMSG("struct.\n");
	// char tmp_buff[BUFFER_SIZE];
	// get_user(tmp_buff, d_data->InputData)
	// if(copy_from_user(data->InputData, tmp_buff, 5))
	// {
	// 	DBGMSG("Class create failed\n");
	// 	return -EFAULT;
	// }
		
	
	// DBGMSG("data->InputData: %s\n", tmp_buff);
	DBGMSG("drv_ioctl_data...\n");

	switch(Cmd)
	{
		/////////////////////////////// IOCTL_GET_TEMPERATURE ///////////////////////////////
		case IOCTL_GET_TEMPERATURE: {
			DBGMSG("IOCTL_GET_TEMPERATURE\n");
			// DBGMSG("data->InputData: %s\n", data->InputData);

			Calculation();
			DBGMSG("temperature_buffer: %s\n", temperature_buffer);
			deviceDataLength = strlen(temperature_buffer);
			{
				//size_t length = deviceDataLength > (data->OutputLength) ? data->OutputLength : deviceDataLength;
				DBGMSG("length = %lu\n", deviceDataLength);
				copy_to_user((char*)data->OutputData, temperature_buffer, deviceDataLength);
			}
			
			break;
		}
// 		/////////////////////////////// IOCTL_GET_PRESSURE    ///////////////////////////////
// 		case IOCTL_GET_PRESSURE: {
// 			DBGMSG("IOCTL_GET_PRESSURE\n");

// 			Calculation();
// 			DBGMSG("pressure_buffer: %s\n", pressure_buffer);
// 			// deviceDataLength = strlen(pressure_buffer);   
// 			// size_t length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
// 			// copy_to_user (data->OutputData, pressure_buffer, length);
// 			break;
// 		}


// 		/////////////////////////////// IOCTL_GET_ALTITUDE    ///////////////////////////////
// 		case IOCTL_GET_ALTITUDE: {
// 			DBGMSG("IOCTL_GET_ALTITUDE\n");

// 			Calculation();
// 			DBGMSG("altitude_buffer: %s\n", altitude_buffer);
// 			// deviceDataLength = strlen(altitude_buffer);
// 			// size_t length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
// 			// copy_to_user (data->OutputData, altitude_buffer, length);
// 			break;
// 		}
	

// 		/////////////////////////////// IOCTL_GET_EEPROOM     ///////////////////////////////
// 		case IOCTL_GET_EEPROOM: {
// 			DBGMSG("IOCTL_GET_EEPROOM\n");

// 			Calculation();
// 			DBGMSG("EEPROM_buffer: %s\n", EEPROM_buffer);
// 			// deviceDataLength = strlen(EEPROM_buffer);
// 			// size_t length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
// 			// copy_to_user (data->OutputData, EEPROM_buffer, length);
// 			break;
// 		}
	}

	return 0;
}

module_init(chr_drv_test_init);
module_exit(chr_drv_test_exit);

void Calculation ()
{
	memset(temperature_buffer, 0, DEVICE_BUFFER_SIZE);
	strcpy(temperature_buffer, "U R in IOCTL_GET_TEMPERATURE");

	memset(pressure_buffer, 0, DEVICE_BUFFER_SIZE); 
	strcpy(pressure_buffer, "U R in IOCTL_GET_PRESSURE");

	memset(altitude_buffer, 0, DEVICE_BUFFER_SIZE);
	strcpy(altitude_buffer, "U R in IOCTL_GET_ALTITUDE");

	memset(EEPROM_buffer, 0, DEVICE_BUFFER_SIZE);
	strcpy(EEPROM_buffer, "U R in IOCTL_GET_EEPROOM");
}



	// /* Read Chip ID */
	// int id = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xD0);
	// printk("ID: 0x%x\n", id);

	// /* Read Calibration Values */
	// AC1_1 = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xAA);
	// AC1_2 = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xAB);

	// if(dig_T2 > 32767)
	// 	dig_T2 -= 65536;

	// if(dig_T3 > 32767)
	// 	dig_T3 -= 65536;

	// /* Initialice the sensor */
	// i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf5, 5<<5);
	// i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));	

// #include <linux/module.h>
// #include <linux/init.h>
// #include <linux/proc_fs.h>
// #include <linux/i2c.h>

// #define SLAVE_DEVICE_NAME	"BMP180"	/* Device and Driver Name */
// #define BMP180_READ_ADDRESS	0xEF		/* BMP180 I2C address for read */
// #define BMP180_WRITE_ADDRESS	0xEE		/* BMP180 I2C address for write */

// static struct i2c_device_id i2c_drv_idtable[] = {
//       { "foo", 0 },
//       { "bar", 1 },
//       { }
// };

// MODULE_DEVICE_TABLE(i2c, i2c_drv_idtable);

// static int bmp180_probe (struct i2c_client *client, const struct i2c_device_id *id){
// 	return 0;
// }
// static int bmp180_remove (struct i2c_client *client){
// 	return 0;
// }

// static struct i2c_driver read_driver = {
//       .driver = {
//               .name   = "i2c_drv",
//             //   .pm     = &foo_pm_ops,  /* optional */
//       },

//     //   .id_table       = foo_idtable,
//     //   .probe_new      = bmp180_probe,
//     //   .remove         = bmp180_remove,
//     //   /* if device autodetection is needed: */
//     //   .class          = I2C_CLASS_SOMETHING,
//     //   .detect         = foo_detect,
//     //   .address_list   = normal_i2c,

//     //   .shutdown       = foo_shutdown, /* optional */
//     //   .command        = foo_command,  /* optional, deprecated */
// };

// static int bmp180_probe (struct i2c_client *client, const struct i2c_device_id *id);
// static int bmp180_remove (struct i2c_client *client);