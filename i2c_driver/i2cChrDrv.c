#include "ChrPart.h"
#include "i2cPart.h"
#include "userIOCTL.h"

MODULE_LICENSE("Dual MIT/GPL");

/* Initialize I2C Board Info strucutre */
struct i2c_board_info bmp180_board_info = { I2C_BOARD_INFO("bmp180", 0x77) };	// i2c address 0x77


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

	/* Register i2c driver */
	i2c_add_driver(&bmp180_driver);

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
	
	i2c_del_driver(&bmp180_driver);
	i2c_unregister_device(bmp180_client);
}
module_init(i2cChrDrv_init);
module_exit(i2cChrDrv_exit);


static int i2cChrDrv_open(struct inode* Inode, struct file* File)
{
	if(deviceOpenCount == 0)
	{
		DBGMSG("i2cChrDrv_open called.\n");
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

	DBGMSG("i2cChrDrv_ioctl called.\n");

	data = (drv_ioctl_data*)Arg;
	DBGMSG("data address: 0x%p\n", data);

	switch(Cmd)
	{
		/////////////////////////////// IOCTL_GET_TEMPERATURE ///////////////////////////////
		case IOCTL_GET_TEMPERATURE: {
			DBGMSG("/////////////////////////////// IOCTL_GET_TEMPERATURE ///////////////////////////////\n");
			
			Calculation();

			deviceDataLength = strlen(temperature_buffer);
			length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
			
			res = copy_to_user(data->OutputData, temperature_buffer, length);
			DBGMSG("copy_to_user res = %d\n", res);
			
			break;
		}
		/////////////////////////////// IOCTL_GET_PRESSURE    ///////////////////////////////
		case IOCTL_GET_PRESSURE: {
			DBGMSG("/////////////////////////////// IOCTL_GET_PRESSURE    ///////////////////////////////\n");

			Calculation();

			deviceDataLength = strlen(pressure_buffer);
			length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;

			res = copy_to_user(data->OutputData, pressure_buffer, length);		
			DBGMSG("copy_to_user res = %d\n", res);
			break;
		}


		/////////////////////////////// IOCTL_GET_ALTITUDE    ///////////////////////////////
		case IOCTL_GET_ALTITUDE: {
			DBGMSG("/////////////////////////////// IOCTL_GET_ALTITUDE    ///////////////////////////////\n");

			Calculation();

			deviceDataLength = strlen(altitude_buffer);
			length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;

			res = copy_to_user (data->OutputData, altitude_buffer, length);
			DBGMSG("copy_to_user res = %d\n", res);
			break;
		}
	

		/////////////////////////////// IOCTL_GET_EEPROOM     ///////////////////////////////
		case IOCTL_GET_EEPROOM: {
			DBGMSG("/////////////////////////////// IOCTL_GET_EEPROOM     ///////////////////////////////\n");

			/* Read Calibration Values */
			ReadEEPROM();

			deviceDataLength = strlen(EEPROM_buffer);
			length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;

			res = copy_to_user (data->OutputData, EEPROM_buffer, length);
			DBGMSG("copy_to_user res = %d\n", res);
			break;
		}
	}

	return 0;
}


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
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE (i2c, bmp180_id_table);

static struct i2c_driver bmp180_driver = {
    .driver = {
		.name = "bmp180",
		.owner = THIS_MODULE,
	},
	.probe = bmp180_probe,
	.remove = bmp180_remove,
	.id_table = bmp180_id_table,
};

///////////////////////////////////////////////////////////////////////////


void Calculation()
{
    int res = 0;
	DBGMSG("Calculation() called!\n");

	/* Read Calibration Values */
	ReadEEPROM();
	
    /* Read Uncompensated Temperature Value */
    res = i2c_smbus_write_byte_data(bmp180_client, 0xF4, 0x2E);

    /* Sleep 4.5 ms (sleep 5 ms used) */
	mdelay(5);
    MSB = i2c_smbus_read_byte_data(bmp180_client, 0xF6);
    LSB = i2c_smbus_read_byte_data(bmp180_client, 0xF7);
    UT = (MSB << 8) + LSB;

    /* Read Uncompensated Pressure Value */
    res = i2c_smbus_write_byte_data(bmp180_client, 0xF4, (0x34 + (oss << 6)));
    
	/* Sleep 4.5 ms (sleep 5 ms used) */
	mdelay(5);
	
    MSB = i2c_smbus_read_byte_data(bmp180_client, 0xF6);
    LSB = i2c_smbus_read_byte_data(bmp180_client, 0xF7);
    XLSB = i2c_smbus_read_byte_data(bmp180_client, 0xF8);
    UP = ((MSB << 16) + (LSB << 8) + XLSB) >> (8 - oss);

    /* Calculate TRUE Temperature */
    X1 = ((UT - AC6) * AC5) >> 15;
    X2 = (MC << 11) / (X1 + MD);
    B5 = X1 + X2;
    T = (B5 + 8) >> 4;

	memset(temperature_buffer, 0, DEVICE_BUFFER_SIZE);
	sprintf(temperature_buffer, "%ld", T);
	DBGMSG("temperature_buffer:\t%s\n", temperature_buffer);

    /* Calculate TRUE Pressure */
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

	memset(pressure_buffer, 0, DEVICE_BUFFER_SIZE); 
	sprintf(pressure_buffer, "%ld", p);
	DBGMSG("pressure_buffer:\t%s\n", pressure_buffer);

	/* Calculate Altitude */


	memset(altitude_buffer, 0, DEVICE_BUFFER_SIZE);
	sprintf(altitude_buffer, "%ld", p);
	DBGMSG("altitude_buffer:\t%s\n", altitude_buffer);
}

void ReadEEPROM()
{
	DBGMSG("ReadEEPROM() called!\n");

	/* Read EEPROM data */
	AC1_1 = i2c_smbus_read_byte_data(bmp180_client, 0xAA);
	AC1_2 = i2c_smbus_read_byte_data(bmp180_client, 0xAB);
	AC1 = ((AC1_1 << 8) + AC1_2);
	
	AC2_1 = i2c_smbus_read_byte_data(bmp180_client, 0xAC);
	AC2_2 = i2c_smbus_read_byte_data(bmp180_client, 0xAD);
	AC2 = ((AC2_1 << 8) + AC2_2);

	AC3_1 = i2c_smbus_read_byte_data(bmp180_client, 0xAE);
	AC3_2 = i2c_smbus_read_byte_data(bmp180_client, 0xAF);
	AC3 = ((AC3_1 << 8) + AC3_2);

	AC4_1 = i2c_smbus_read_byte_data(bmp180_client, 0xB0);
	AC4_2 = i2c_smbus_read_byte_data(bmp180_client, 0xB1);
	AC4 = ((AC4_1 << 8) + AC4_2);

	AC5_1 = i2c_smbus_read_byte_data(bmp180_client, 0xB2);
	AC5_2 = i2c_smbus_read_byte_data(bmp180_client, 0xB3);
	AC5 = ((AC5_1 << 8) + AC5_2);

	AC6_1 = i2c_smbus_read_byte_data(bmp180_client, 0xB4);
	AC6_2 = i2c_smbus_read_byte_data(bmp180_client, 0xB5);
	AC6 = ((AC6_1 << 8) + AC6_2);

	B1_1 = i2c_smbus_read_byte_data(bmp180_client, 0xB6);
	B1_2 = i2c_smbus_read_byte_data(bmp180_client, 0xB7);
	B1 = ((B1_1 << 8) + B1_2);

	B2_1 = i2c_smbus_read_byte_data(bmp180_client, 0xB8);
	B2_2 = i2c_smbus_read_byte_data(bmp180_client, 0xB9);
	B2 = ((B2_1 << 8) + B2_2);

	MB_1 = i2c_smbus_read_byte_data(bmp180_client, 0xBA);
	MB_2 = i2c_smbus_read_byte_data(bmp180_client, 0xBB);
	MB = ((MB_1 << 8) + MB_2);

	MC_1 = i2c_smbus_read_byte_data(bmp180_client, 0xBC);
	MC_2 = i2c_smbus_read_byte_data(bmp180_client, 0xBD);
	MC = ((MC_1 << 8) + MC_2);

	MD_1 = i2c_smbus_read_byte_data(bmp180_client, 0xBE);
	MD_2 = i2c_smbus_read_byte_data(bmp180_client, 0xBF);
	MD = ((MD_1 << 8) + MD_2);

	memset(EEPROM_buffer, 0, DEVICE_BUFFER_SIZE);
	
	sprintf(EEPROM_buffer, "%hd %hd %hd %hu %hu %hu %hd %hd %hd %hd %hd", AC1, AC2, AC3, AC4, AC5, AC6, B1, B2, MB, MC, MD);
	DBGMSG("EEPROM_buffer:\t%s\n", EEPROM_buffer);
}

