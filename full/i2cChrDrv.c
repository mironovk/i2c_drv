#include "ChrPart.h"
#include "i2cPart.h"
#include "userIOCTL.h"

MODULE_LICENSE("Dual MIT/GPL");

/* Initialize I2C Board Info strucutre */
static struct i2c_board_info bmp180_board_info = { I2C_BOARD_INFO("bmp180", 0x77) };


int __init i2cChrDrv_init(void)
{
	DBGMSG("I2cChrDrv_init called.\n");
	deviceMajorNum = register_chrdev(0, DEVICE_NAME, &i2cChrDrv_functions);
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

	/* Initialize i2c_adapter structure */
	bmp180_adapter = i2c_get_adapter(1); // bus i2c1

	/* Initialize i2c_client structure */
	bmp180_client = i2c_new_client_device(bmp180_adapter, &bmp180_board_info);

	i2c_add_driver(&bmp180_driver);
	// if (platform_driver_register(&bmp180_driver))
	// {
	// 	DBGMSG("Error! Could not load driver!\n")
	// 	return -1;
	// }
	DBGMSG("i2cChrDrv_init completed!\n");
	return 0;
	// return 0;
}

void __exit i2cChrDrv_exit(void)
{
	DBGMSG("i2cChrDrv_exit called.\n");
	
	device_destroy(deviceClass, MKDEV(deviceMajorNum, 0));
	class_unregister(deviceClass);
	class_destroy(deviceClass);
	unregister_chrdev(deviceMajorNum, DEVICE_NAME);
	
	// platform_driver_unregister(&bmp180_driver)
	i2c_del_driver(&bmp180_driver);
	// return 0;
}
module_init(i2cChrDrv_init);
module_exit(i2cChrDrv_exit);


static int i2cChrDrv_open(struct inode* Inode, struct file* File)
{
	if(deviceOpenCount == 0)
	{
		DBGMSG("i2cChrDrv_open called.\n");
		// bmp180_client = i2c_new_client_device(&adapter, &info);
		deviceOpenCount++;
		return 0;
	}
	else
	{
		DBGMSG("Device already opened!\n");
		return -13;
	}
}

static int i2cChrDrv_release(struct inode* Inode, struct file* File)
{
	DBGMSG("i2cChrDrv_release called.\n");
	deviceOpenCount--;
	return 0;
}

static long int i2cChrDrv_ioctl(struct file* File, unsigned int Cmd, unsigned long Arg)
{
	int res;
	int length;
	drv_ioctl_data* data;

	res = 0;
	length = 0;
	// data = NULL;

	DBGMSG("i2cChrDrv_ioctl called.\n");
	// char* data = (char*)Arg;
	data = (drv_ioctl_data*)Arg;
	DBGMSG("data address: 0x%p\n", data);
	// char* data = (char*)Arg;
	// get_user(temperature_buffer, data->InputData)
	// if(copy_from_user(data->InputData, tmp_buff, 5))
	// {
	// 	DBGMSG("Class create failed\n");
	// 	return -EFAULT;
	// }
		
	
	// DBGMSG("data->InputData: %s\n", temperature_buffer);
	// DBGMSG("drv_ioctl_data...\n");

	switch(Cmd)
	{
		/////////////////////////////// IOCTL_GET_TEMPERATURE ///////////////////////////////
		case IOCTL_GET_TEMPERATURE: {
			DBGMSG("IOCTL_GET_TEMPERATURE\n");
			// DBGMSG("data->InputData: %s\n", data->InputData);
			// DBGMSG("IOCTL_GET_TEMPERATURE\n", );
			// Calculation();
			DBGMSG("bmp180_client address: 0x%p\n", bmp180_client);
			DBGMSG("bmp180_driver address: 0x%p\n", &bmp180_driver);
			DBGMSG("client address: 0x%X\n", bmp180_client->addr);


			// MSB = i2c_smbus_read_byte_data(bmp180_client, 0xF6);
			DBGMSG("MSB = %d\n", MSB);
			DBGMSG("temperature_buffer: %s\n", temperature_buffer);
			deviceDataLength = strlen(temperature_buffer);
			{
				length = deviceDataLength > BUFFER_SIZE ? BUFFER_SIZE : deviceDataLength;
				DBGMSG("length = %d\n", length);
				res = copy_to_user(data->OutputData, temperature_buffer, length);
				// int res = copy_to_user(data, temperature_buffer, length);
				DBGMSG("copy_to_user res = %d\n", res);
			}
			
			break;
		}
		/////////////////////////////// IOCTL_GET_PRESSURE    ///////////////////////////////
		case IOCTL_GET_PRESSURE: {
			DBGMSG("IOCTL_GET_PRESSURE\n");

			Calculation();
			DBGMSG("pressure_buffer: %s\n", pressure_buffer);
			// deviceDataLength = strlen(pressure_buffer);   
			// size_t length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
			// copy_to_user (data, pressure_buffer, length);
			length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
			res = copy_to_user(data->OutputData, pressure_buffer, length);		
			// int res = copy_to_user(data, pressure_buffer, length);
			DBGMSG("copy_to_user res = %d\n", res);
			break;
		}


		/////////////////////////////// IOCTL_GET_ALTITUDE    ///////////////////////////////
		case IOCTL_GET_ALTITUDE: {
			DBGMSG("IOCTL_GET_ALTITUDE\n");

			Calculation();
			DBGMSG("altitude_buffer: %s\n", altitude_buffer);
			// deviceDataLength = strlen(altitude_buffer);
			// size_t length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
			
			length = deviceDataLength > BUFFER_SIZE ? BUFFER_SIZE : deviceDataLength;
			res = copy_to_user (data->OutputData, altitude_buffer, length);
			// int res = copy_to_user(data, altitude_buffer, length);
			DBGMSG("copy_to_user res = %d\n", res);
			break;
		}
	

		/////////////////////////////// IOCTL_GET_EEPROOM     ///////////////////////////////
		case IOCTL_GET_EEPROOM: {
			DBGMSG("IOCTL_GET_EEPROOM\n");

			Calculation();
			DBGMSG("EEPROM_buffer: %s\n", EEPROM_buffer);
			// deviceDataLength = strlen(EEPROM_buffer);
			// size_t length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
			
			length = deviceDataLength > BUFFER_SIZE ? BUFFER_SIZE : deviceDataLength;
			res = copy_to_user (data->OutputData, EEPROM_buffer, length);
			// int res = copy_to_user(data, EEPROM_buffer, length);
			DBGMSG("copy_to_user res = %d\n", res);
			break;
		}
	}

	return 0;
}

// void Calculation ()
// {
// 	memset(temperature_buffer, 0, DEVICE_BUFFER_SIZE);
// 	strcpy(temperature_buffer, "U R in IOCTL_GET_TEMPERATURE");

// 	memset(pressure_buffer, 0, DEVICE_BUFFER_SIZE); 
// 	strcpy(pressure_buffer, "U R in IOCTL_GET_PRESSURE");

// 	memset(altitude_buffer, 0, DEVICE_BUFFER_SIZE);
// 	strcpy(altitude_buffer, "U R in IOCTL_GET_ALTITUDE");

// 	memset(EEPROM_buffer, 0, DEVICE_BUFFER_SIZE);
// 	strcpy(EEPROM_buffer, "U R in IOCTL_GET_EEPROOM");
// }

static int bmp180_probe (struct i2c_client *client, const struct i2c_device_id *id) 
{
    DBGMSG("bmp180_probe called!\n");
    if (client->addr != BMP180_ADDRESS)
    {
        DBGMSG("Wrong device address!\n");
        return -1;
    }

    bmp180_client = client;

    return 0;
}

static int bmp180_remove (struct i2c_client *client)
{
    DBGMSG("bmp180_remove called!\n");
    return 0;
}

static struct i2c_device_id bmp180_id_table[] = {
	{ "bmp180", 0 },    
	// { "bmp180_wr", 0 },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE (i2c, bmp180_id_table);

// static struct of_device_id bmp180_of_match_table[] = {
// 	//{ .compatible = "MyI2CSensors,bmp180" }, 
//       { /* sentinel */ }
// };
// MODULE_DEVICE_TABLE (of, bmp180_of_match_table);

static struct i2c_driver bmp180_driver = {
    .driver = {
		.name = "bmp180",
		.owner = THIS_MODULE,
		// .of_match_table = bmp180_of_match_table,
	},
	.probe = bmp180_probe,
	.remove = bmp180_remove,
	.id_table = bmp180_id_table,
      // .probe_new = bmp180_probe,
};
// module_i2c_driver(bmp180_driver);

////////////////////////////////////////////////////////////////////////////////////


void Calculation()
{
    int res = 0;
	DBGMSG("Calculation called!\n");
	/* Read EEPROM data */
	// int id = i2c_smbus_read_byte_data(bmp180_client, 0xD0);
	// printk("ID: 0x%x\n", id);

	/* Read Calibration Values */
	AC1 = i2c_smbus_read_word_data(bmp180_client, 0xAA);
	AC2 = i2c_smbus_read_word_data(bmp180_client, 0xAC);
	AC3 = i2c_smbus_read_word_data(bmp180_client, 0xAE);
	AC4 = i2c_smbus_read_word_data(bmp180_client, 0xB0);
	AC5 = i2c_smbus_read_word_data(bmp180_client, 0xB2);
	AC6 = i2c_smbus_read_word_data(bmp180_client, 0xB4);
    B1 = i2c_smbus_read_word_data(bmp180_client, 0xB6);
	B2 = i2c_smbus_read_word_data(bmp180_client, 0xB8);
	MB = i2c_smbus_read_word_data(bmp180_client, 0xBA);
	MC = i2c_smbus_read_word_data(bmp180_client, 0xBC);
	MD = i2c_smbus_read_word_data(bmp180_client, 0xBE);

	DBGMSG("EEPROM data read!\n");

	memset(EEPROM_buffer, 0, DEVICE_BUFFER_SIZE);
	DBGMSG("EEPROM_buffer address: 0x%p\nEEPROM_buffer value: %s\n", EEPROM_buffer, EEPROM_buffer);
	sprintf(EEPROM_buffer, "%hd %hd %hd %hu %hu %hu %hd %hd %hd %hd %hd", AC1, AC2, AC3, AC4, AC5, AC6, B1, B2, MB, MC, MD);
	// strcpy(EEPROM_buffer, "U R in IOCTL_GET_EEPROOM");
	
    /* Read Uncompensated Temperature Value */
    res = i2c_smbus_write_byte_data(bmp180_client, 0xF4, 0x2E);
    //sleep DELAY
	mdelay(5);
	UT = i2c_smbus_read_word_data(bmp180_client, 0xF6);
    MSB = i2c_smbus_read_byte_data(bmp180_client, 0xF6);
    LSB = i2c_smbus_read_byte_data(bmp180_client, 0xF7);
    UT = (MSB << 8) + LSB;

    /* Read Uncompensated Pressure Value */
    res = i2c_smbus_write_byte_data(bmp180_client, 0xF4, (0x34 + (oss << 6)));
    //sleep DELAY 5ms
	mdelay(5);
	// int UT = i2c_smbus_read_word_data(bmp180_client, 0xF6);
    MSB = i2c_smbus_read_byte_data(bmp180_client, 0xF6);
    LSB = i2c_smbus_read_byte_data(bmp180_client, 0xF7);
    XLSB = i2c_smbus_read_byte_data(bmp180_client, 0xF8);
    UP = ((MSB << 16) + (LSB << 8) + XLSB) >> (8 - oss);

    /* Calculate Temperature */
    X1 = ((UT - AC6) * AC5) >> 15;
    X2 = (MC << 11) / (X1 + MD);
    B5 = X1 + X2;
    T = (B5 + 8) >> 4;

	memset(temperature_buffer, 0, DEVICE_BUFFER_SIZE);
	sprintf(temperature_buffer, "%ld", T);
	// strcpy(temperature_buffer, "U R in IOCTL_GET_TEMPERATURE");

    /* Calculate Pressure */
	B6 = B5 - 4000;
    X1 = (B2 * ((B6 * B6) >> 12)) >> 11;
    X2 = (AC2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((AC1 << 2) + X3) << oss) + 2) >> 2;
    X1 = (AC3 * B6) >> 13;
    X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = (AC4 * (unsigned long)(X3 + 32768)) >> 15;
    B7 = ((unsigned long)UP - B3) * (50000 >> oss);
    if (B7 < 0x80000000)
    {
    	p = (B7 << 1) / B4;
    }
    else
    {
    	p = (B7 / B4) << 1;
    }
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    p = p + ((X1 + X2 + 3791) >> 4);
    // sprintf(pressure_buffer, "%ld", p);

	memset(pressure_buffer, 0, DEVICE_BUFFER_SIZE); 
	sprintf(pressure_buffer, "%ld", p);
	// strcpy(pressure_buffer, "U R in IOCTL_GET_PRESSURE");

	// memset(altitude_buffer, 0, DEVICE_BUFFER_SIZE);
	// strcpy(altitude_buffer, "U R in IOCTL_GET_ALTITUDE");
}

