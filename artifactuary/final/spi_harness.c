#include "spi_harness.h"

#include <time.h>

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


#define BILLION 1000000000
#define FRAME_NSEC 33333333


int spi_harness_fd;


void spi_harness_loop(void);


void spi_harness_main(void)
{
    uint32_t speed = 2000000;
    uint8_t bits = 8;
    uint8_t mode = 0;
    
    artifactuary_init();
    
    spi_harness_fd = open("/dev/spidev0.0", O_RDWR);
    
    if(spi_harness_fd <= 0) {
        printf("failed to pen /dev/spidev0.0\n");
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
    int64_t cur_nsec;
    int64_t last_target_nsec;
    int64_t next_target_nsec;
    
    uint8_t spi_frame_data[ARTIFACTUARY_NUM_PIXELS * 3];
    
    clock_gettime(CLOCK_MONOTONIC, &wall_start_time);
    cur_nsec = wall_start_time.tv_nsec + (int64_t)wall_start_time.tv_sec * BILLION;
    next_target_nsec = cur_nsec + FRAME_NSEC;
    
    while(true) {
        double last_frame_sec = (double)(next_target_nsec - last_target_nsec) * 1.0e-9;
        
        artifactuary_process(last_frame_sec);
        
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
        
        // output the frame over the SPI bus
        for(int32_t i = 0; i < ARTIFACTUARY_NUM_PIXELS * 3; i += 3) {
            int32_t index = artifactuary_array_data_mapping[i];
            spi_frame_data[i + 0] = artifactuary_array_data[index].c.r;
            spi_frame_data[i + 1] = artifactuary_array_data[index].c.g;
            spi_frame_data[i + 2] = artifactuary_array_data[index].c.b;
        }
        write(spi_fd, spi_frame_data, ARTIFACTUARY_NUM_PIXELS * 3);
    }
}



