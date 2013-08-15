#include "artifactuary.h"

#include <stdio.h>
#include <time.h>

#include "fire.h"
#include "scroll.h"


// the data pointers are set up in artifactuary_init
array_t artifactuary_arrays[ARTIFACTUARY_NUM_ARRAYS] = {
	{
		.width = ARTIFACTUARY_BUILDING_0_FACE_0_WIDTH,
		.height = ARTIFACTUARY_BUILDING_0_FACE_0_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_BUILDING_0_FACE_1_WIDTH,
		.height = ARTIFACTUARY_BUILDING_0_FACE_1_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_BUILDING_1_FACE_0_WIDTH,
		.height = ARTIFACTUARY_BUILDING_1_FACE_0_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_BUILDING_1_FACE_1_WIDTH,
		.height = ARTIFACTUARY_BUILDING_1_FACE_1_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_BUILDING_2_FACE_0_WIDTH,
		.height = ARTIFACTUARY_BUILDING_2_FACE_0_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_BUILDING_2_FACE_1_WIDTH,
		.height = ARTIFACTUARY_BUILDING_2_FACE_1_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_BUILDING_3_FACE_0_WIDTH,
		.height = ARTIFACTUARY_BUILDING_3_FACE_0_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_BUILDING_3_FACE_1_WIDTH,
		.height = ARTIFACTUARY_BUILDING_3_FACE_1_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_CITYSCAPE_WIDTH,
		.height = ARTIFACTUARY_CITYSCAPE_HEIGHT,
	},
};

rgba_t artifactuary_array_data[ARTIFACTUARY_NUM_PIXELS];
int32_t artifactuary_array_data_mapping[ARTIFACTUARY_NUM_PIXELS];


fire_state_t artifactuary_fire_state;
scroll_state_t artifactuary_scroll_state;


void artifactuary_init(void)
{
    rgba_t* data = artifactuary_array_data;
	
    array_init();
    
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        artifactuary_arrays[i].data = data;
        data += artifactuary_arrays[i].width * artifactuary_arrays[i].height;
    }
    
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_PIXELS; ++i) {
        artifactuary_array_data_mapping[i] = i;
        artifactuary_array_data[i].rgba = 0xFFFFFFFF;
    }
    
    fire_init(&artifactuary_fire_state, ARTIFACTUARY_CITYSCAPE_WIDTH, ARTIFACTUARY_CITYSCAPE_HEIGHT);
    scroll_init(&artifactuary_scroll_state, ARTIFACTUARY_BUILDING_3_FACE_0_WIDTH, ARTIFACTUARY_BUILDING_3_FACE_0_HEIGHT);
}


#define BILLION 1000000000


void artifactuary_process(float time)
{
    struct timespec process_start_time;
    struct timespec process_end_time;
    int64_t process_nsec;
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_start_time);
    
    fire_process(&artifactuary_fire_state, time, &artifactuary_arrays[ARTIFACTUARY_CITYSCAPE]);
    scroll_process(&artifactuary_scroll_state, time, &artifactuary_arrays[ARTIFACTUARY_BUILDING_3_FACE_0]);
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_end_time);
    
    process_nsec = process_end_time.tv_nsec + (int64_t)process_end_time.tv_sec * BILLION -
        (process_start_time.tv_nsec + (int64_t)process_start_time.tv_sec * BILLION);
    
    printf("frame time: %7.3fms/%7.3fms\n", (double)process_nsec * 1.0e-6, time * 1000.0);
}


