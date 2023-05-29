#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/fcntl.h> 
#include <sys/ioctl.h>      
#include "userIOCTL.h"
#include <math.h>

int main()
{
	int device = open("/dev/BMP180", O_RDWR);
	if (device < 0)
	{
		printf("Error!\t%X\n", errno);
	}
	else
	{
		int res = 0;
		drv_ioctl_data data;
		data.OutputData = malloc(BUFFER_SIZE);
		data.OutputLength = BUFFER_SIZE;

		///////////// IOCTL_GET_TEMPERATURE /////////////
		printf("///////////// IOCTL_GET_TEMPERATURE /////////////\n");
		memset(data.OutputData, 0, BUFFER_SIZE);
		res = ioctl(device, IOCTL_GET_TEMPERATURE, &data);
		printf("ioctl res:\t%d\n", res);
		printf("Temperature:\t%s\n", data.OutputData);

		///////////// IOCTL_GET_PRESSURE    /////////////
		printf("///////////// IOCTL_GET_PRESSURE    /////////////\n");
		memset(data.OutputData, 0, BUFFER_SIZE);
		res = ioctl(device, IOCTL_GET_PRESSURE, &data);
		printf("Pressure:\t%s\n", data.OutputData);

		///////////// IOCTL_GET_ALTITUDE    /////////////
		printf("///////////// IOCTL_GET_ALTITUDE    /////////////\n");
		memset(data.OutputData, 0, BUFFER_SIZE);
		res = ioctl(device, IOCTL_GET_ALTITUDE, &data);
		printf("Altitude raw:\t%s\n", data.OutputData);
		double alt;
		double p0_user = 101325;
		double p_user = strtod(data.OutputData, NULL);
		alt = 44330 * (1 - pow((p_user/p0_user), 1/5.255));
		printf("Altitude:\t%f\n", alt);

		///////////// IOCTL_GET_EEPROOM     /////////////
		printf("///////////// IOCTL_GET_EEPROOM     /////////////\n");
		memset(data.OutputData, 0, BUFFER_SIZE);
		res = ioctl(device, IOCTL_GET_EEPROOM, &data);
		printf("EEPROM:\t%s\n", data.OutputData);
		
		free(data.OutputData);
	}
	getchar();
	close(device);
	return 0;
}
