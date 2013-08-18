#include "spi_harness.h"

#include <stdio.h>
#include <time.h>

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


#define BILLION 1000000000
#define FRAME_NSEC 33333333


// this gamma table purposely only goes to 127 because full brightness is
// TOO BRIGHT
uint8_t spi_harness_gamma_table[256]  = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,
    4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  7,  7,
    7,  7,  7,  8,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 10, 11,
   11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16,
   16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 21, 22, 22,
   23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30,
   30, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 37, 37, 38, 38, 39,
   40, 40, 41, 41, 42, 43, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50,
   50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 62,
   62, 63, 64, 65, 66, 67, 67, 68, 69, 70, 71, 72, 73, 74, 74, 75,
   76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
   92, 93, 94, 95, 96, 97, 98, 99,100,101,102,104,105,106,107,108,
  109,110,111,113,114,115,116,117,118,120,121,122,123,125,126,127
};

int spi_harness_fd;


void spi_harness_loop(void);
void spi_harness_output_spi_data(void);


void spi_harness_main(void)
{
    uint32_t speed = 2000000;
    uint8_t bits = 8;
    uint8_t mode = 0;
    
    artifactuary_init();
    
    spi_harness_fd = open("/dev/spidev0.0", O_RDWR);
    
    if(spi_harness_fd <= 0) {
        perror("failed to open /dev/spidev0.0 (try running as root)\n");
        return;
    }
    
    ioctl(spi_harness_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    ioctl(spi_harness_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(spi_harness_fd, SPI_IOC_WR_MODE, &mode);
    
    spi_harness_loop();
}


void spi_harness_loop(void)
{
    struct timespec wall_start_time;
    struct timespec wall_cur_time;
    int64_t start_nsec;
    int64_t cur_nsec;
    int64_t last_target_nsec;
    int64_t next_target_nsec;
    
    clock_gettime(CLOCK_MONOTONIC, &wall_start_time);
    start_nsec = wall_start_time.tv_nsec + (int64_t)wall_start_time.tv_sec * BILLION;
    cur_nsec = start_nsec;
    next_target_nsec = cur_nsec + FRAME_NSEC;
    last_target_nsec = cur_nsec;
    
    while(true) {
        artifactuary_process(last_target_nsec - start_nsec, next_target_nsec - last_target_nsec);
        
        clock_gettime(CLOCK_MONOTONIC, &wall_cur_time);
        cur_nsec = wall_cur_time.tv_nsec + (int64_t)wall_cur_time.tv_sec * BILLION;
        
        last_target_nsec = next_target_nsec;
        // did we come in within our 30fps deadline?
        if(cur_nsec < next_target_nsec) {
            // yes: pause and wait for the deadline before we start the next frame
            struct timespec next_target_time = {
                .tv_sec = (time_t)(next_target_nsec / BILLION),
                .tv_nsec = (long)(next_target_nsec % BILLION)
            };
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_target_time, NULL);
            next_target_nsec += FRAME_NSEC;
        }
        else {
            // no: compute the next deadline from the current time instead so we don't keep falling farther behind forever
            next_target_nsec = cur_nsec + FRAME_NSEC;
        }
        
        spi_harness_output_spi_data();
    }
}


void spi_harness_output_spi_data(void)
{
    uint8_t spi_frame_data[ARTIFACTUARY_NUM_PIXELS * 3];
    struct spi_ioc_transfer xfer[4];
    int ret;
    
    // output the frame over the SPI bus
    
    for(int32_t i = 0, j = 0; i < ARTIFACTUARY_NUM_PIXELS; ++i, j += 3) {
        int32_t index = artifactuary_array_data_mapping[i];
        spi_frame_data[j + 0] = spi_harness_gamma_table[artifactuary_array_data[index].c.b];
        spi_frame_data[j + 1] = spi_harness_gamma_table[artifactuary_array_data[index].c.r];
        spi_frame_data[j + 2] = spi_harness_gamma_table[artifactuary_array_data[index].c.g];
    }
    
    memset(&xfer, 0, sizeof xfer);
    
    // this transfer is broken in two because the SPI_IOC_MESSAGE ioctl seems
    // to barf if you give it more than 4k of data at a time.
    xfer[0].tx_buf = (intptr_t)spi_frame_data;
    xfer[0].len = sizeof spi_frame_data / 2;
    xfer[0].delay_usecs = 0;
    xfer[1].tx_buf = (intptr_t)(spi_frame_data + (ARTIFACTUARY_NUM_PIXELS * 3) / 2);
    xfer[1].len = sizeof spi_frame_data - sizeof spi_frame_data / 2;
    xfer[1].delay_usecs = 1000;
    
    ret = ioctl(spi_harness_fd, SPI_IOC_MESSAGE(1), &xfer[0]);
    if(ret < 1) {
        perror("SPI send (a) failed");
    }
    ret = ioctl(spi_harness_fd, SPI_IOC_MESSAGE(1), &xfer[1]);
    if(ret < 1) {
        perror("SPI send (b) failed");
    }
}


