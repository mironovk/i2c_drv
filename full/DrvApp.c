#include <stdio.h>
#include <stdlib.h>
// #include <fcntl.h>
#include <errno.h> 
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/fcntl.h> 
// #include <sys/stat.h>
#include <sys/ioctl.h>      
// #include <unistd.h>     
// #include <stdio.h>
// #include <stdlib.h>
#include "userIOCTL.h"

drv_ioctl_data data;

int main()
{
	int device = open("/dev/BMP180", O_RDWR);
	if (device < 0)
	{
		printf("Error!\t%X\n", errno);
		// printf("Error code name: %s\n", strerrorname_np(errno));
	}
	else
	{
		int res = 0;

		getchar();
		
		data.InputData = "Some data.";
		data.InputLength = strlen(data.InputData) + 1;
		data.OutputData = malloc(BUFFER_SIZE);
		data.OutputLength = BUFFER_SIZE;
		memcpy(data.OutputData, "U R in IOCTL_GET_PRESSURE", data.OutputLength);

		printf("data address: 0x%p\n", &data);
		printf("data.InputData address: 0x%p\ndata.InputData value: 0x%s\n", &data.InputData, data.InputData);
		printf("data.InputLength address: 0x%p\ndata.InputLength value: 0x%lu\n", &data.InputLength, data.InputLength);

		printf("data.OutputData address: 0x%p\ndata.OutputData value: 0x%s\n", &data.OutputData, data.OutputData);
		printf("data.OutputLength address: 0x%p\ndata.OutputLength value: 0x%lu\n", &data.OutputLength, data.OutputLength);
		
		getchar();

		// printf("data.InputLength address: 0x%p\n", &data);
		// printf("data.OutputData address: 0x%p\n", &data);
		// printf("data.OutputLength address: 0x%p\n", &data);
		// memset(data.OutputData, 0, BUFFER_SIZE); 
		// memcpy(data.OutputData, "U R in IOCTL_GET_PRESSURE", data.OutputLength);
		// char* data[BUFFER_SIZE] = { 0 };
		// char* data = (char*)malloc(BUFFER_SIZE);
		// printf("data.InputData: %s\n", data.InputData);
		// printf("data.OutputData: %s\n", data.OutputData);
		// getchar();
		// unsigned long d = &data;
		// // unsigned long d = (unsigned long)(&data);
		// drv_ioctl_data* p_data = (drv_ioctl_data*)d;
		// printf("InputData:\t%s\n", p_data->InputData);

		// getchar();
		
		/////////////////////////////// IOCTL_GET_TEMPERATURE ///////////////////////////////
	
		printf("IOCTL_GET_TEMPERATURE:\n");
		memset(data.OutputData, 0, BUFFER_SIZE);
		// memset(data, 0, BUFFER_SIZE);
		// getchar();
		// res = ioctl(device, IOCTL_GET_TEMPERATURE, &buffer);
		res = ioctl(device, IOCTL_GET_TEMPERATURE, &data);
		printf("ioctl res:\t%d\n", res);
		// printf("Temperature:\t%s\n", data);
		printf("Temperature:\t%s\n", data.OutputData);
		//printf("Error code name: %s\n", strerrorname_np(errno));
		// getchar();

		/////////////////////////////// IOCTL_GET_PRESSURE    ///////////////////////////////

		printf("IOCTL_GET_PRESSURE:\n");
		memset(data.OutputData, 0, BUFFER_SIZE);
		// memset(data, 0, BUFFER_SIZE);
		res = ioctl(device, IOCTL_GET_PRESSURE, &data);
		// printf("Pressure:\t%s\n", data);
		printf("Pressure:\t%s\n", data.OutputData);
		// getchar();

		/////////////////////////////// IOCTL_GET_ALTITUDE    ///////////////////////////////

		printf("IOCTL_GET_ALTITUDE:\n");
		memset(data.OutputData, 0, BUFFER_SIZE);
		// memset(data, 0, BUFFER_SIZE);
		res = ioctl(device, IOCTL_GET_ALTITUDE, &data);
		// printf("Altitude:\t%s\n", data);
		printf("Altitude:\t%s\n", data.OutputData);
		// getchar();

		/////////////////////////////// IOCTL_GET_EEPROOM     ///////////////////////////////

		printf("IOCTL_GET_EEPROOM:\n");
		memset(data.OutputData, 0, BUFFER_SIZE);
		// memset(data, 0, BUFFER_SIZE);
		res = ioctl(device, IOCTL_GET_EEPROOM, &data);
		// printf("EEPROM:\t%s\n", data);
		printf("EEPROM:\t%s\n", data.OutputData);
		// free(data);
		free(data.OutputData);
		
	}

	getchar();
	close(device);
	
	return 0;
}
