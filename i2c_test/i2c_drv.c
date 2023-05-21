#include "i2c_drv.h"

MODULE_LICENSE("Dual MIT/GPL");

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
	{ "bmp180", 0 },    ///////////////////////////////// Что задает 2 параметр?
	// { "bmp180_wr", 0 },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE (i2c, bmp180_id_table);

static struct of_device_id bmp180_of_match_table[] = {
	{ .compatible = "MyI2CSensors, bmp180" }, ///////////////////////////////// Что задает 2 параметр .data ?
      { /* sentinel */ }
};
MODULE_DEVICE_TABLE (of, bmp180_of_match_table);

static struct i2c_driver bmp180_driver = {
	.driver = {
		.name = "bmp180",
		.of_match_table = bmp180_of_match_table,
	},
      // .probe_new = bmp180_probe,
      .probe = bmp180_probe,
	.remove = bmp180_remove,
	.id_table = bmp180_id_table,
	
};
// module_i2c_driver(bmp180_driver);

////////////////////////////////////////////////////////////////////////////////////


void Calculation()
{
      int res = 0;
	/* Read EEPROM data */
	int id = i2c_smbus_read_byte_data(bmp180_client, 0xD0);
	printk("ID: 0x%x\n", id);

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
	
      /* Read Uncompensated Temperature Value */
      res = i2c_smbus_write_byte_data(bmp180_client, 0xF4, 0x2E);
      //sleep DELAY
	UT = i2c_smbus_read_word_data(bmp180_client, 0xF6);
      MSB = i2c_smbus_read_byte_data(bmp180_client, 0xF6);
      LSB = i2c_smbus_read_byte_data(bmp180_client, 0xF7);
      UT = (MSB << 8) + LSB;

      /* Read Uncompensated Pressure Value */
      res = i2c_smbus_write_byte_data(bmp180_client, 0xF4, (0x34 + (oss << 6)));
      //sleep DELAY
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

	/* Initialice the sensor */
	// i2c_smbus_write_byte_data(bmp180_client, 0xf5, 5<<5);
	// i2c_smbus_write_byte_data(bmp180_client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));	
}

static int __init bmp180_init(void)
{
      DBGMSG("bmp180_init called!\n");
	return i2c_add_driver(&bmp180_driver);
}
static void __exit bmp180_exit(void)
{
      DBGMSG("bmp180_exit called!\n");
	i2c_del_driver(&bmp180_driver);
}
module_init(bmp180_init);
module_exit(bmp180_exit);

