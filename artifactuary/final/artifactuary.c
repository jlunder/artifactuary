#include "artifactuary.h"

#include <stdio.h>
#include <time.h>

#include "fire.h"
#include "scroll.h"


// the data pointers are set up in artifactuary_init
array_t artifactuary_arrays[ARTIFACTUARY_NUM_ARRAYS] = {
    {
        .width = ARTIFACTUARY_BACKDROP_WIDTH,
        .height = ARTIFACTUARY_BACKDROP_HEIGHT,
    },
    {
        .width = ARTIFACTUARY_BUILDING_B_WIDTH,
        .height = ARTIFACTUARY_BUILDING_B_HEIGHT,
    },
    {
        .width = ARTIFACTUARY_BUILDING_B2_WIDTH,
        .height = ARTIFACTUARY_BUILDING_B2_HEIGHT,
    },
    {
        .width = ARTIFACTUARY_BUILDING_C_WIDTH,
        .height = ARTIFACTUARY_BUILDING_C_HEIGHT,
    },
    {
        .width = ARTIFACTUARY_BUILDING_A_WIDTH,
        .height = ARTIFACTUARY_BUILDING_A_HEIGHT,
    },
};

rgba_t artifactuary_array_data[ARTIFACTUARY_NUM_PIXELS];
int32_t artifactuary_array_data_mapping[ARTIFACTUARY_NUM_PIXELS];


fire_state_t artifactuary_fire_state[ARTIFACTUARY_NUM_ARRAYS];
scroll_state_t artifactuary_short_scroll_state;
scroll_state_t artifactuary_tall_scroll_state;


void artifactuary_generate_data_mapping(void);


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
    artifactuary_generate_data_mapping();
    
    // initialize the panels all to full white so they're visible in the test
    // program
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_PIXELS; ++i) {
        artifactuary_array_data[i].rgba = 0xFFFFFFFF;
    }
    
    // initialize all the state structures used by the panel image generation
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        fire_init(&artifactuary_fire_state[i], artifactuary_arrays[i].width, artifactuary_arrays[i].height);
    }
    //scroll_init(&artifactuary_short_scroll_state, ARTIFACTUARY_BACKDROP_WIDTH, ARTIFACTUARY_BACKDROP_HEIGHT);
    scroll_init(&artifactuary_tall_scroll_state, ARTIFACTUARY_BUILDING_A_WIDTH, ARTIFACTUARY_BUILDING_A_HEIGHT);
}


void artifactuary_generate_data_mapping(void)
{
    int32_t array_data_offset = 0;
    int32_t light_index = 0;
    int32_t panel_count; // how many panels are fused together in this array?
    int32_t panel_width;
    int32_t panel_height;
    enum { PANEL_LEFT_TO_RIGHT, PANEL_RIGHT_TO_LEFT } panel_order;
    enum { DIR_LEFT, DIR_RIGHT } hdir;
    enum { DIR_UP, DIR_DOWN } vdir;
    
    for(int32_t array = 0; array < ARTIFACTUARY_NUM_ARRAYS; ++array) {
        // decide the initial panel directions based on which array this is
        // Vlad: this is where almost all the useful configuration of the
        //       array orientations happens.
        switch(array) {
        case ARTIFACTUARY_BACKDROP:
            //panel_count = 1;
            panel_count = 3;
            panel_order = PANEL_RIGHT_TO_LEFT;
            vdir = DIR_DOWN; // Vlad: for even panel counts this should be DIR_UP
            break;
            
        case ARTIFACTUARY_BUILDING_B:
            panel_count = 2;
            panel_order = PANEL_LEFT_TO_RIGHT;
            vdir = DIR_DOWN;
            break;
            
        case ARTIFACTUARY_BUILDING_B2:
            panel_count = 1;
            panel_order = PANEL_LEFT_TO_RIGHT;
            vdir = DIR_DOWN;
            break;
            
        case ARTIFACTUARY_BUILDING_C:
            panel_count = 3;
            panel_order = PANEL_LEFT_TO_RIGHT;
            vdir = DIR_DOWN;
            break;
            
        case ARTIFACTUARY_BUILDING_A:
            panel_count = 2;
            panel_order = PANEL_LEFT_TO_RIGHT;
            vdir = DIR_UP;
            break;
            
        default:
            assert("why is there an extra panel here?" == NULL);
            break;
        }
        
        panel_width = artifactuary_arrays[array].width / panel_count;
        panel_height = artifactuary_arrays[array].height;
        
        // the panels have to have an odd number of rows for the wiring to
        // dovetail nicely using this algorithm when there are multiple panels
        // if there's only one panel for this array, don't care
        assert((panel_height % 2 == 1) || (panel_count == 1));
        
        // check that the panel count results in sane panel widths for this
        // array width
        assert(artifactuary_arrays[array].width % panel_count == 0);
        assert((panel_width == 8) || (panel_width == 10));
        
        // this is the workhorse loop. for every panel, it walks up and down,
        // sided to side, tracing the path of the lights and noting the
        // location in the array data where this light's data comes from.
        for(int32_t k = 0; k < panel_count; ++k) {
            int32_t panel;
            
            // set the initial horizontal direction based on the panel
            // ordering, and the panel index
            switch(panel_order) {
            case PANEL_LEFT_TO_RIGHT:
                panel = k;
                hdir = DIR_RIGHT;
                break;
                
            case PANEL_RIGHT_TO_LEFT:
                panel = panel_count - 1 - k;
                hdir = DIR_LEFT;
                break;
            }
            
            for(int32_t j = 0; j < panel_height; ++j) {
                int32_t row;
                
                // find the row given the direction we're iterating
                switch(vdir) {
                case DIR_DOWN:
                    row = j;
                    break;
                    
                case DIR_UP:
                    row = panel_height - 1 - j;
                    break;
                    
                default:
                    assert("how did vdir get botched?" == NULL);
                }
                
                for(int32_t i = 0; i < panel_width; ++i) {
                    int32_t column;
                    int32_t array_data_index;
                    
                    // find the column given the direction we're iterating
                    switch(hdir) {
                    case DIR_RIGHT:
                        column = i;
                        break;
                    
                    case DIR_LEFT:
                        column = panel_width - 1 - i;
                        break;
                    
                    default:
                        assert("how did hdir get botched?" == NULL);
                    }
                    
                    array_data_index = 
                        array_data_offset +
                        row * artifactuary_arrays[array].width +
                        panel * panel_width +
                        column;
                    
                    assert(array_data_index >= 0);
                    assert(array_data_index < ARTIFACTUARY_NUM_PIXELS);
                    
                    artifactuary_array_data_mapping[light_index] = array_data_index;
                    ++light_index;
                }
                
                // reverse horizontal directions every row
                switch(hdir) {
                case DIR_RIGHT:
                    hdir = DIR_LEFT;
                    break;
                
                case DIR_LEFT:
                    hdir = DIR_RIGHT;
                    break;
                
                default:
                    assert("how did hdir get botched?" == NULL);
                }
            }
            
            // reverse vertical directions every panel
            switch(vdir) {
            case DIR_DOWN:
                vdir = DIR_UP;
                break;
                
            case DIR_UP:
                vdir = DIR_DOWN;
                break;
                
            default:
                assert("how did vdir get botched?" == NULL);
            }
        }
        
        array_data_offset += artifactuary_arrays[array].width * artifactuary_arrays[array].height;
    }
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
    //scroll_process(&artifactuary_short_scroll_state, time, &artifactuary_arrays[ARTIFACTUARY_BACKDROP]);
    scroll_process(&artifactuary_tall_scroll_state, time, &artifactuary_arrays[ARTIFACTUARY_BUILDING_A]);
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_end_time);
    
    // compute the CPU time used by the processing
    process_nsec = process_end_time.tv_nsec + (int64_t)process_end_time.tv_sec * BILLION -
        (process_start_time.tv_nsec + (int64_t)process_start_time.tv_sec * BILLION);
    
    if(time > 0.034) {
        printf("frame time: %7.3fms/%7.3fms\n", (double)process_nsec * 1.0e-6, time * 1000.0);
    }
}


