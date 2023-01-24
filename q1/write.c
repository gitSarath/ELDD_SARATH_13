#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char ubuf[1024];

int main()
{
        int fd;
        fd = open("/dev/file_device", O_RDWR);
        if(fd > 0)
        {
                printf("Device File Opened successfully in the path /dev/file_device...\n");
        }
        else {
                printf("Cannot open device file...\n");
                return 0;
        }
    printf("writing.....\n");
    write(fd,"hello\n",6);
    close(fd);
        return 0;
}

