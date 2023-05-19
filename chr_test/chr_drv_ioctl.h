#include <linux/ioctl.h>
//#include <sys/ioctl.h>

#define DRV_MAGIC 0xB2

#define TEMPERATURE_GET 0x70 
#define PRESSURE_GET 0x71 
#define ALTITUDE_GET 0x72 
#define EEPROM_GET 0x73

#define IOCTL_GET_TEMPERATURE _IOR(DRV_MAGIC, TEMPERATURE_GET, char*)
#define IOCTL_GET_PRESSURE _IOR(DRV_MAGIC, PRESSURE_GET, char*)
#define IOCTL_GET_ALTITUDE _IOR(DRV_MAGIC, ALTITUDE_GET, char*)
#define IOCTL_GET_EEPROOM _IOR(DRV_MAGIC, EEPROM_GET, char*)

#define BUFFER_SIZE 1024

typedef struct _drv_ioctl_data { 
	char* InputData;
	size_t InputLength;
	char* OutputData;
	size_t OutputLength; 
} drv_ioctl_data;



// #define IOCTL_DRV_SET _IOW(DRV_MAGIC, DRV_SET, char*)
// #define IOCTL_DRV_GET_AND_SET _IOWR(DRV_MAGIC, DRV_GET_AND_SET, char*) #define IOCTL_DRV_DBG_MESSAGE _IO(DRV_MAGIC, DRV_DBG_MESSAGE)