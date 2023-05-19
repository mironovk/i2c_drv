#include "i2c_drv.h"

MODULE_LICENSE("Dual MIT/GPL");


static int __init i2c_drv_init(void)
{
	DBGMSG("i2c_drv_init called.\n");
	deviceMajorNum = register_chrdev(0, DEVICE_NAME, &test_drv_functions);
	if(deviceMajorNum < 0)
	{
		DBGMSG("Register device failed\n");
		return -1;
	}
	else
	{
		DBGMSG("Device Major number = $d\n", deviceMajorNum);	
	}

	deviceClass = class_create(THIS_MODULE, DEVICE_CLASS);
	if(deviceClass == 0)
	{
		unregister_chrdev(deviceMajorNum, DEVICE_NAME);
		return -1;
	}

	deviceHandle = device_create(deviceClass, NULL, MKDEV(deviceMajorNum, 0), NULL, DEVICE_NAME);
	if(deviceHandle == 0)
	{
		class_unregister(deviceClass);
		class_destroy(deviceClass);
		unregister_chrdev(deviceMajorNum, DEVICE_NAME);
		return -1;
	}

	DBGMSG("i2c_drv_init completed!\n");
	return 0;
}

static int __exit i2c_drv_exit(void)
{
	class_unregister(deviceClass);
	class_destroy(deviceClass);
	unregister_chrdev(deviceMajorNum, DEVICE_NAME);
	return 0;
}


static int i2c_drv_open(struct inode* Inode, struct file* File)
{
	DBGMSG("i2c_drv_open called.\n");
	DBGMSG("i2c_drv_open completed!\n");
	return 0;
}

static int i2c_drv_release(struct inode* Inode, struct file* File)
{
	DBGMSG("i2c_drv_release called.\n");
	//DBGMSG("i2c_drv_release completed!\n");
	return 0;
}

static long int i2c_drv_ioctl(struct file* File, unsigned int Cmd, unsigned long Arg)
{
	
}

	/////////////////////////////// IOCTL_GET_TEMPERATURE ///////////////////////////////
	


	/////////////////////////////// IOCTL_GET_PRESSURE    ///////////////////////////////

	

	/////////////////////////////// IOCTL_GET_ALTITUDE    ///////////////////////////////

	

	/////////////////////////////// IOCTL_GET_EEPROOM     ///////////////////////////////


	/* Read Chip ID */
	int id = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xD0);
	printk("ID: 0x%x\n", id);

	/* Read Calibration Values */
	AC1_1 = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xAA);
	AC1_2 = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xAB);

	if(dig_T2 > 32767)
		dig_T2 -= 65536;

	if(dig_T3 > 32767)
		dig_T3 -= 65536;

	/* Initialice the sensor */
	i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf5, 5<<5);
	i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));	

