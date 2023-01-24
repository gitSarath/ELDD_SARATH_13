#include "header.h"
#include<sys/ioctl.h>

#define RD_DATA _IOR('a','a',int32_t*)
#define WR_DATA _IOW('a','b',int32_t*)


int main()
{
        int fd,num;
        int32_t val=0;
        fd=open("/dev/led_device",O_RDWR);

        if(fd<0)
        {
                printf("cant open the file\n");
                return 0;
        }
        printf("enter the data to send..\n");
        scanf("%d",&num);



