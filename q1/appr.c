#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char rbuf[1024];

int main()
{
        int fd;
        char option;
        fd = open("/dev/file_device", O_RDWR);
        if(fd > 0)
        {
                printf("Device File Opened successfully in the path /dev/file_device...\n");
        }
        else {
                printf("Cannot open device file...\n");
                return 0;
        }
    printf("reading the data.\n");
    read(fd,ubuf, sizeof(ubuf));
    printf("data read :%s\n",ubuf);
    close(fd);
        return 0;
}

