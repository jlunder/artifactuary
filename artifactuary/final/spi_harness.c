#include "spi_harness.h"

#include <stdio.h>
#include <time.h>


#define BILLION 1000000000
#define FRAME_NSEC 33333333


void spi_harness_main(void)
{
    artifactuary_init();
    
    struct timespec wall_start_time;
    struct timespec wall_cur_time;
    int64_t cur_nsec;
    int64_t last_target_nsec;
    int64_t next_target_nsec;
    
    struct timespec process_start_time;
    struct timespec process_end_time;
    int64_t process_nsec;
    
    clock_gettime(CLOCK_MONOTONIC, &wall_start_time);
    cur_nsec = wall_start_time.tv_nsec + (int64_t)wall_start_time.tv_sec * BILLION;
    next_target_nsec = cur_nsec + FRAME_NSEC;
    
    while(true) {
        double last_frame_sec = (double)(next_target_nsec - last_target_nsec) * 1.0e-9;
        
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_start_time);
        artifactuary_process(last_frame_sec);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_end_time);
        
        process_nsec = process_end_time.tv_nsec + (int64_t)process_end_time.tv_sec * BILLION -
            (process_start_time.tv_nsec + (int64_t)process_start_time.tv_sec * BILLION);
        
        printf("frame time: %7.3fms/%7.3fms\n", (double)process_nsec * 1.0e-6, last_frame_sec);
        
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
    }
}


