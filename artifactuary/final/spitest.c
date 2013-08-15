#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

uint8_t data[2000 * 3];
uint8_t palette[20 * 3] = {
    255, 255, 255,
    127, 127, 127,
    255,   0,   0,
      0, 255,   0,
      0,   0, 255,
    127, 127, 127,
    127,   0,   0,
      0, 127,   0,
      0,   0, 127,
    255, 127, 127,
    127, 255, 127,
    127, 127, 255,
    255,   0,   0,
      0, 255,   0,
      0,   0, 255,
    127, 127, 127,
    127,   0,   0,
      0, 127,   0,
      0,   0, 127,
    127, 127, 127,
};

uint8_t data_clear[20 * 3];

int main(int argc, char* argv[])
{
    uint32_t speed = 2000000;
    uint8_t bits = 8;
    uint8_t mode = 0;
    
    int fd = open("/dev/spidev0.0", O_RDWR);
    
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    
    write(fd, data_clear, sizeof data_clear);
    //for(int j = 0; j < 1000; ++j) {
    while(true) {
        struct timespec start_time;
        struct timespec end_time;
        struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 1000000};
        int64_t nsec;
        
        for(int i = 0; i < sizeof data; ++i) {
            data[((i - i % 3) + (i + 1) % 3 + 300/*j * 3*/) % sizeof data] = (palette[i % 60] * (sizeof data - i)) / sizeof data;
        }
        
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        for(int i = 0; i < 10; ++i) {
            struct spi_ioc_transfer xfer[4];
            int ret;
            
            memset(&xfer, 0, sizeof xfer);
            xfer[0].tx_buf = (intptr_t)data;
            xfer[0].len = sizeof data / 2;
            xfer[0].delay_usecs = 0;
            xfer[1].tx_buf = (intptr_t)(data + sizeof data / 2);
            xfer[1].len = sizeof data / 2;
            xfer[1].delay_usecs = 1000;
            
            ret = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer[0]);
            if(ret < 1) {
                perror("SPI send failed");
                exit(1);
            }
            ret = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer[1]);
            if(ret < 1) {
                perror("SPI send failed");
                exit(1);
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        
        nsec = ((int64_t)end_time.tv_sec * 1000000000 + end_time.tv_nsec) -
            ((int64_t)start_time.tv_sec * 1000000000 + start_time.tv_nsec);
        //printf("30 frames: %0.7f\n", (double)nsec * 1e-9);
//        break;
    }
    write(fd, data_clear, sizeof data_clear);
    fsync(fd);
    
    return 0;
}

