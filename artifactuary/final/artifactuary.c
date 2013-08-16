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
		.width = ARTIFACTUARY_BUILDING_3_WIDTH,
		.height = ARTIFACTUARY_BUILDING_3_HEIGHT,
	},
	{
		.width = ARTIFACTUARY_CITYSCAPE_WIDTH,
		.height = ARTIFACTUARY_CITYSCAPE_HEIGHT,
	},
};

rgba_t artifactuary_array_data[ARTIFACTUARY_NUM_PIXELS];
int32_t artifactuary_array_data_mapping[ARTIFACTUARY_NUM_PIXELS];


fire_state_t artifactuary_fire_state[ARTIFACTUARY_NUM_ARRAYS];
scroll_state_t artifactuary_short_scroll_state;
scroll_state_t artifactuary_tall_scroll_state;


void artifactuary_init(void)
{
    rgba_t* data = artifactuary_array_data;
	
	// init modules
    array_init();
    
    // set up data pointers in the framebuffer arrays manually
    // this is so we can address the data as one linear block when we're
    // remapping the data to output it to the light array
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        artifactuary_arrays[i].data = data;
        data += artifactuary_arrays[i].width * artifactuary_arrays[i].height;
    }
    
    // set up the data mapping for the light array
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_PIXELS; ++i) {
        artifactuary_array_data_mapping[i] = i;
    }
    
    {
		int32_t base = 0;
		int32_t width = ARTIFACTUARY_BUILDING_0_FACE_0_WIDTH;
		int32_t height = ARTIFACTUARY_BUILDING_0_FACE_0_HEIGHT;
	
		for(int32_t j = 0; j < height; j += 2) {
			for(int32_t i = 0; i < width; ++i) {
				artifactuary_array_data_mapping[base + i] = base + width - 1 - i;
			}
			base += width;
			for(int32_t i = 0; i < width; ++i) {
				artifactuary_array_data_mapping[base + i] = base + i;
			}
			base += width;
		}
    }
    
    // initialize the panels all to full white so they're visible in the test program
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_PIXELS; ++i) {
        artifactuary_array_data[i].rgba = 0xFFFFFFFF;
    }
    
    // initialize all the state structures used by the panel image generation
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        fire_init(&artifactuary_fire_state[i], artifactuary_arrays[i].width, artifactuary_arrays[i].height);
    }
    scroll_init(&artifactuary_short_scroll_state, ARTIFACTUARY_BUILDING_0_FACE_0_WIDTH, ARTIFACTUARY_BUILDING_0_FACE_0_HEIGHT);
    scroll_init(&artifactuary_tall_scroll_state, ARTIFACTUARY_BUILDING_3_WIDTH, ARTIFACTUARY_BUILDING_3_HEIGHT);
}


#define BILLION 1000000000


void artifactuary_process(float time)
{
    struct timespec process_start_time;
    struct timespec process_end_time;
    int64_t process_nsec;
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_start_time);
    
    // process fire backgrounds for all panels
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        fire_process(&artifactuary_fire_state[i], time, &artifactuary_arrays[i]);
    }
    // process scrolling text for the tall building
    scroll_process(&artifactuary_short_scroll_state, time, &artifactuary_arrays[ARTIFACTUARY_BUILDING_0_FACE_0]);
    scroll_process(&artifactuary_tall_scroll_state, time, &artifactuary_arrays[ARTIFACTUARY_BUILDING_3]);
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_end_time);
    
    // compute the CPU time used by the processing
    process_nsec = process_end_time.tv_nsec + (int64_t)process_end_time.tv_sec * BILLION -
        (process_start_time.tv_nsec + (int64_t)process_start_time.tv_sec * BILLION);
    
    if(time > 0.034) {
        printf("frame time: %7.3fms/%7.3fms\n", (double)process_nsec * 1.0e-6, time * 1000.0);
    }
}


