#include "artifactuary.h"

#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "effect_joe_fire.h"
#include "effect_joe_text_scroll.h"


typedef enum {
    MODE_NONE,
    MODE_BLANK,
    MODE_FRAME,
    MODE_FIRE,
    MODE_COUNT,
} artifactuary_mode_t;


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


int64_t artifactuary_last_process_nsec = 0;
int64_t artifactuary_last_frame_time_nsec = 0;

artifactuary_mode_t artifactuary_mode = MODE_NONE;
artifactuary_mode_t artifactuary_next_mode = MODE_FIRE;

effect_joe_fire_state_t artifactuary_fire_state[ARTIFACTUARY_NUM_ARRAYS];


void artifactuary_generate_data_mapping(void);
void artifactuary_ui_init(void);

void artifactuary_ui_process();
void artifactuary_ui_draw();

void artifactuary_mode_fire_init(void);
void artifactuary_mode_fire_shutdown(void);
void artifactuary_mode_fire_process(int64_t total_time_ns, int64_t frame_time_ns);


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
    
    artifactuary_ui_init();
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
            panel_count = 1;
            if(artifactuary_arrays[array].height % 2 == 0) {
	            panel_order = PANEL_LEFT_TO_RIGHT;
            }
            else {
	            panel_order = PANEL_RIGHT_TO_LEFT;
	        }
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
        assert((panel_width % 8 == 0) || (panel_width % 10 == 0));
        
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


void artifactuary_ui_init(void)
{
#ifdef USE_NCURSES
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    timeout(0);
    
    artifactuary_ui_draw();
    
    //endwin();
#endif
}


void artifactuary_ui_process(void)
{
#ifdef USE_NCURSES
    int ch;
    bool ui_modified = false;
    
    ch = getch();
    if(ch != ERR) {
        while(ch != ERR) {
            switch(ch) {
            case 'q':
                endwin();
                exit(0);
                break;
            case KEY_LEFT:
                artifactuary_next_mode -= 1;
                if(artifactuary_next_mode <= MODE_NONE) {
                    artifactuary_next_mode = MODE_COUNT - 1;
                }
                break;
            case KEY_RIGHT:
                artifactuary_next_mode += 1;
                if(artifactuary_next_mode >= MODE_COUNT) {
                    artifactuary_next_mode = MODE_NONE + 1;
                }
                break;
            }
            ch = getch();
        }
    }
    
    if(artifactuary_next_mode != artifactuary_mode) {
        ui_modified = true;
    }
    
    if(ui_modified) {
        artifactuary_ui_draw();
    }
    
    move(5, 0);
    if(artifactuary_last_process_nsec > 5000000 || artifactuary_last_frame_time_nsec > 34000000) {
        attron(A_BOLD);
    }
    printw("frame time: %7.3fms/%7.3fms", (double)artifactuary_last_frame_time_nsec * 1.0e-6, (double)artifactuary_last_process_nsec * 1.0e-6);
    attroff(A_BOLD);
    refresh();
#else
    // autoplay mode
#endif
}


void artifactuary_ui_draw(void)
{
#ifdef USE_NCURSES
    char const* mode_name;
    
    switch(artifactuary_next_mode) {
    case MODE_NONE: mode_name = "none (?)"; break;
    case MODE_BLANK: mode_name = "blank white"; break;
    case MODE_FRAME: mode_name = "rainbow frames"; break;
    case MODE_FIRE: mode_name = "fire"; break;
    default: mode_name = "!UNKNOWN!"; break;
    }
    
    erase();
    
    move(0, 0);
    printw("   ---ARTIFACTUARY---\n");
    printw("\n");
    printw("current mode: %s\n", mode_name);
    printw("\n");
#endif
}


void artifactuary_mode_fire_init(void)
{
    // initialize all the state structures used by the panel image generation
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        effect_joe_fire_init(&artifactuary_fire_state[i], artifactuary_arrays[i].width, artifactuary_arrays[i].height);
    }
}


void artifactuary_mode_fire_shutdown(void)
{
}


void artifactuary_mode_fire_process(int64_t total_time_ns, int64_t frame_time_ns)
{
    // process fire backgrounds for all panels
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        effect_joe_fire_process(&artifactuary_fire_state[i], &artifactuary_arrays[i], total_time_ns, frame_time_ns);
    }
}


#define BILLION 1000000000


void artifactuary_process(int64_t total_time_ns, int64_t frame_time_ns)
{
    struct timespec process_start_time;
    struct timespec process_end_time;
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_start_time);
    
    artifactuary_ui_process();
    
    if(artifactuary_next_mode != artifactuary_mode) {
        switch(artifactuary_mode) {
        case MODE_FIRE:
            artifactuary_mode_fire_shutdown();
            break;
        default:
            break;
        }
        artifactuary_mode = artifactuary_next_mode;
        switch(artifactuary_mode) {
        case MODE_FIRE:
            artifactuary_mode_fire_init();
            break;
        default:
            break;
        }
    }
    
    switch(artifactuary_mode) {
    case MODE_NONE:
        // fill the array black
        memset(artifactuary_array_data, 0, sizeof artifactuary_array_data);
        break;
    case MODE_BLANK:
        // fill the array white
        memset(artifactuary_array_data, 255, sizeof artifactuary_array_data);
        break;
    case MODE_FRAME:
        for(int32_t k = 0; k < ARTIFACTUARY_NUM_ARRAYS; ++k) {
            int32_t width = artifactuary_arrays[k].width;
            int32_t height = artifactuary_arrays[k].height;
            
            // resistor color sequence
            rgba_t array_colors[] = {
                {{  0,   0,   0, 255}}, // black
                {{127,  63,   0, 255}}, // brown
                {{255,   0,   0, 255}}, // red
                {{255, 127,   0, 255}}, // orange
                {{191, 191,   0, 255}}, // yellow
                {{  0, 255,   0, 255}}, // green
                {{  0,   0, 255, 255}}, // blue
                {{127,   0, 127, 255}}, // purple
                {{ 63,  63,  63, 255}}, // grey
                {{255, 255, 255, 255}}, // white
            };
            // fill array with the appropriate color
            for(int32_t i = 0; i < width * height; ++i) {
                artifactuary_arrays[k].data[i] = array_colors[k];
            }
            // draw white frame around the array
            for(int32_t j = 0; j < height; ++j) {
                artifactuary_arrays[k].data[j * width + 0].rgba = 0xFFFFFFFF;
                artifactuary_arrays[k].data[j * width + width - 1].rgba = 0xFFFFFFFF;
            }
            for(int32_t i = 0; i < artifactuary_arrays[k].width; ++i) {
                artifactuary_arrays[k].data[0 * width + i].rgba = 0xFFFFFFFF;
                artifactuary_arrays[k].data[(height - 1) * width + i].rgba = 0xFFFFFFFF;
            }
        }
        break;
    case MODE_FIRE:
        artifactuary_mode_fire_process(total_time_ns, frame_time_ns);
        break;
    }
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_end_time);
    
    // compute the CPU time used by the processing
    artifactuary_last_process_nsec = process_end_time.tv_nsec + (int64_t)process_end_time.tv_sec * BILLION -
        (process_start_time.tv_nsec + (int64_t)process_start_time.tv_sec * BILLION);
    artifactuary_last_frame_time_nsec = frame_time_ns;
}


