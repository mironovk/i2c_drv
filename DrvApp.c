#include <stdio.h>
#include "chr_drv_ioctl.h"

int main()
{
	int device = open("/dev/BMP180", O_RDWR, 0777);
	if (device < 0)
		printf("Error!\n");

	drv_ioctl_data data = { 0 };

	/////////////////////////////// IOCTL_GET_TEMPERATURE ///////////////////////////////
	
	printf("IOCTL_GET_TEMPERATURE:\n");
	data.OutputData = malloc(1024);
	data.InputLength = strlen(data.InputLength) + 1;

	int res = ioctl(device, IOCTL_GET_TEMPERATURE, &data);
	printf("Temperature:\t$s\n", data.OutputData);

	/////////////////////////////// IOCTL_GET_PRESSURE    ///////////////////////////////

	printf("IOCTL_GET_PRESSURE:\n");
	data.OutputData = malloc(1024);
	data.InputLength = strlen(data.InputLength) + 1;

	int res = ioctl(device, IOCTL_GET_PRESSURE, &data);
	printf("Pressure:\t$s\n", data.OutputData);

	/////////////////////////////// IOCTL_GET_ALTITUDE    ///////////////////////////////

	printf("IOCTL_GET_ALTITUDE:\n");
	data.OutputData = malloc(1024);
	data.InputLength = strlen(data.InputLength) + 1;

	int res = ioctl(device, IOCTL_GET_ALTITUDE, &data);
	printf("Altitude:\t$s\n", data.OutputData);

	/////////////////////////////// IOCTL_GET_EEPROOM     ///////////////////////////////

	printf("IOCTL_GET_EEPROOM:\n");
	data.OutputData = malloc(1024);
	data.InputLength = strlen(data.InputLength) + 1;

	int res = ioctl(device, IOCTL_GET_ALTITUDE, &data);
	printf("EEPROM:\t$s\n", data.OutputData);


	






	// int N = 4;
	// int M = 3;
	
	// int arr[N][M] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}};
	
	// int i_max = 0;
	// int j_max = 0;
	
	// for(int i = 0; i < N; i++)
	// {
	// 	for(int j = 0; j < M; j++)
	// 	{
	// 		printf("%d", arr[i][j]);
	// 		if(arr[i][j] > arr[i_max][j_max])
	// 		{
	// 			i_max = i;
	// 			j_max = j;
	// 		}		
	// 	}
	// 	printf("\n");
	// }
	
	// printf("i_max = %d\n j_max = %d\n", i_max, j_max);
	
	return 0;
}
