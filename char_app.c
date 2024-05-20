/***************************************************************************//**
*  \file       test_app.c
*
*  \details    Userspace application to test the Device driver
*
*  \author     saravnanb@spanidea.com
*******************************************************************************/
#include <stdio.h>
#include <sys/types.h>  //clocks, size, threads which passed to ioctl commands.
#include <fcntl.h>  //function provides for control over open files
#include <unistd.h>  //file that provides access to the POSIX operating system API.(Portable operating system).
#include<sys/ioctl.h>
                                         //a-helps differentiate between different ioctl commands associated with the same driver or device. The command number should be unique for each ioctl operation.
#define WR_VALUE _IOW('a','a',int32_t*) // used to create a command code for a write operation. a-used to identify the driver or device associated with the ioctl command.
#define RD_VALUE _IOR('a','b',int32_t*) // int32_t* - passed to the ioctl command. In this case, it's a pointer to a 32-bit integer.
 
int main()
{
        int fd;
        int32_t value, number;
        printf("*********************************\n");
 
        printf("\nOpening Driver\n");
        fd = open("/dev/exam_devices", O_RDWR);
        if(fd < 0) 
	{
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Enter the Value to send\n");
        scanf("%d",&number);
        printf("Writing Value to Driver\n");
        ioctl(fd, WR_VALUE, (int32_t*) &number); // int32_t* -  is a pointer to a memory location. This location is where data is passed to or received from the device
                                                 // It is cast to (int32_t*) because the device driver is likely expecting a pointer to a 32-bit integer. 
        printf("Reading Value from Driver\n");
        ioctl(fd, RD_VALUE, (int32_t*) &value);
        printf("Value is %d\n", value);
 
        printf("Closing Driver\n");
        close(fd);
}
