#include "i2c_drv.h"

MODULE_LICENSE("Dual MIT/GPL");

////////////////////////////////////////////////////////////////////////////////////
#include "i2c_drv.h"
static struct i2c_device_id i2c_drv_idtable[] = {
      { "foo", my_id_for_foo },
      { "bar", my_id_for_bar },
      { }
};

MODULE_DEVICE_TABLE(i2c, i2c_drv_idtable);

static struct i2c_driver read_driver = {
      .driver = {
              .name   = "i2c_drv",
              .pm     = &foo_pm_ops,  /* optional */
      },

      .id_table       = foo_idtable,
      .probe_new      = foo_probe,
      .remove         = foo_remove,
      /* if device autodetection is needed: */
      .class          = I2C_CLASS_SOMETHING,
      .detect         = foo_detect,
      .address_list   = normal_i2c,

      .shutdown       = foo_shutdown, /* optional */
      .command        = foo_command,  /* optional, deprecated */
}

////////////////////////////////////////////////////////////////////////////////////

module_i2c_driver(read_driver);

// static int __init foo_init(void)
// {
//       return i2c_add_driver(&foo_driver);
// }
// module_init(foo_init);

// static void __exit foo_cleanup(void)
// {
//       i2c_del_driver(&foo_driver);
// }
// module_exit(foo_cleanup);

//The module_i2c_driver() macro can be used to reduce above code.



////////////////////////////////////////////////////////////////////////////////////




	/* Read EEPROM data */
	int id = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xD0);
	printk("ID: 0x%x\n", id);

	/* Read Calibration Values */
	int AC1 = i2c_smbus_read_word_data(bmp180_i2c_client, 0xAA);
	int AC2 = i2c_smbus_read_word_data(bmp180_i2c_client, 0xAC);
	int AC3 = i2c_smbus_read_word_data(bmp180_i2c_client, 0xAE);
	int AC4 = i2c_smbus_read_word_data(bmp180_i2c_client, 0xB0);
	int AC5 = i2c_smbus_read_word_data(bmp180_i2c_client, 0xB2);
	int AC6 = i2c_smbus_read_word_data(bmp180_i2c_client, 0xB4);


	/* Initialice the sensor */
	i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf5, 5<<5);
	i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));	

