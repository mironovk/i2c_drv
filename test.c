#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEVICE_BUFFER_SIZE 1024


void testFun(){
    printf("testFun called.");
}

//static int deviceDataLength = 0;

int main()
{
    //char* deviceBuffer = malloc(DEVICE_BUFFER_SIZE);
    char deviceBuffer[DEVICE_BUFFER_SIZE] = { 0 };
    printf("deviceBuffer:\t%s\n", deviceBuffer);

    memset(deviceBuffer, 0, DEVICE_BUFFER_SIZE);
    memcpy(deviceBuffer, "IOCTL_GET_TEMPERATURE", strlen("IOCTL_GET_TEMPERATURE"));
    size_t length = strlen(deviceBuffer);
    printf("deviceBuffer:\t%s\nlength = %lU\n", deviceBuffer, length);

    memset(deviceBuffer, 0, DEVICE_BUFFER_SIZE);
    strcpy(deviceBuffer, "U R in IOCTL_GET_EEPROOM");
    length = strlen(deviceBuffer);
    printf("deviceBuffer:\t%s\nlength = %lu\n", deviceBuffer, length);


	//getchar();

	return 0;
}
