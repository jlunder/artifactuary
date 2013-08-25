#include "artifactuary.h"

#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "effect_timer_select.h"

#include "effect_joe_fire.h"
#include "effect_joe_text_scroll.h"
#include "effect_vlad_fire_0.h"
#include "effect_vlad_plasma_0.h"
#include "effect_vlad_sinewave_0.h"
#include "effect_vlad_mandelbrot_0.h"
#include "effect_vlad_acid_trap_0.h"
#include "effect_vlad_rainbow_slider_0.h"
#include "effect_vlad_rainbow_ladder_0.h"
#include "effect_vlad_sparkler_0.h"

typedef enum {
    MODE_NONE,
    MODE_POWER_TEST,
    MODE_FRAME,
    MODE_PLAY,
} artifactuary_mode_t;


// the data pointers are set up in artifactuary_init
array_t artifactuary_arrays[ARTIFACTUARY_NUM_ARRAYS] = {
    {
        .width = ARTIFACTUARY_BUILDING_A_WIDTH,
        .height = ARTIFACTUARY_BUILDING_A_HEIGHT,
    },
    {
        .width = ARTIFACTUARY_BUILDING_B_WIDTH,
        .height = ARTIFACTUARY_BUILDING_B_HEIGHT,
    },
    {
        .width = ARTIFACTUARY_BUILDING_C_WIDTH,
        .height = ARTIFACTUARY_BUILDING_C_HEIGHT,
    },
};

rgba_t artifactuary_array_data[ARTIFACTUARY_NUM_PIXELS];
int32_t artifactuary_array_data_mapping[ARTIFACTUARY_NUM_PIXELS];


int64_t artifactuary_last_process_nsec = 0;
int64_t artifactuary_last_frame_time_nsec = 0;

artifactuary_mode_t artifactuary_mode = MODE_NONE;
artifactuary_mode_t artifactuary_next_mode = MODE_PLAY;

artifactuary_mode_t artifactuary_ui_selected_array = 0;

effect_t* artifactuary_effects[ARTIFACTUARY_NUM_ARRAYS];


void artifactuary_generate_data_mapping(void);
void artifactuary_ui_init(void);

void artifactuary_ui_process();
void artifactuary_ui_draw();

void artifactuary_mode_power_test_init(void);
void artifactuary_mode_frame_init(void);
void artifactuary_mode_play_init(void);

void artifactuary_mode_power_test_effect_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);
void artifactuary_mode_frame_effect_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


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
        case ARTIFACTUARY_BUILDING_A:
            panel_count = 3;
            panel_order = PANEL_LEFT_TO_RIGHT;
            vdir = DIR_UP;
            break;
            
        case ARTIFACTUARY_BUILDING_B:
            panel_count = 3;
            panel_order = PANEL_LEFT_TO_RIGHT;
            vdir = DIR_DOWN;
            break;
            
        case ARTIFACTUARY_BUILDING_C:
            panel_count = 3;
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
        
        // ...FUCK THE TIDY WIRING
        //assert((panel_height % 2 == 1) || (panel_count == 1));
        
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
    
    atexit(endwin);
#endif
}


#define BILLION 1000000000


void artifactuary_process(int64_t total_time_ns, int64_t frame_time_ns)
{
    struct timespec process_start_time;
    struct timespec process_end_time;
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_start_time);
    
    artifactuary_ui_process();
    
    if(artifactuary_next_mode != artifactuary_mode) {
        for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
            if(artifactuary_effects[i] != NULL) {
                effect_destroy(artifactuary_effects[i]);
                artifactuary_effects[i] = NULL;
            }
        }
        artifactuary_mode = artifactuary_next_mode;
        switch(artifactuary_mode) {
        case MODE_POWER_TEST:
            artifactuary_mode_power_test_init();
            break;
        case MODE_FRAME:
            artifactuary_mode_frame_init();
            break;
        case MODE_PLAY:
            artifactuary_mode_play_init();
            break;
        case MODE_NONE:
        default:
            break;
        }
        artifactuary_ui_draw();
    }
    
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        if(artifactuary_effects[i] != NULL) {
            effect_process(artifactuary_effects[i], &artifactuary_arrays[i], total_time_ns, frame_time_ns);
        }
        else {
            // clear to black
            memset(artifactuary_arrays[i].data, 0, artifactuary_arrays[i].width * artifactuary_arrays[i].height * sizeof (rgba_t));
        }
    }
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_end_time);
    
    // compute the CPU time used by the processing
    artifactuary_last_process_nsec = process_end_time.tv_nsec + (int64_t)process_end_time.tv_sec * BILLION -
        (process_start_time.tv_nsec + (int64_t)process_start_time.tv_sec * BILLION);
    artifactuary_last_frame_time_nsec = frame_time_ns;
}


void artifactuary_ui_process(void)
{
#ifdef USE_NCURSES
    int ch;
    bool ui_modified = false;
    effect_t* selected_effect = artifactuary_effects[artifactuary_ui_selected_array];
    
    ch = getch();
    if(ch != ERR) {
        while(ch != ERR) {
            switch(ch) {
            case 'q':
                exit(0);
                break;
            case 'W':
                artifactuary_next_mode = MODE_POWER_TEST;
                break;
            case 'F':
                artifactuary_next_mode = MODE_FRAME;
                break;
            case 'p':
            case 'P':
                artifactuary_next_mode = MODE_PLAY;
                break;
            default:
                if(artifactuary_mode == MODE_PLAY) {
                    switch(ch) {
                    case KEY_UP:
                        artifactuary_ui_selected_array = (artifactuary_ui_selected_array + ARTIFACTUARY_NUM_ARRAYS - 1) % ARTIFACTUARY_NUM_ARRAYS;
                        break;
                    case KEY_DOWN:
                        artifactuary_ui_selected_array = (artifactuary_ui_selected_array + 1) % ARTIFACTUARY_NUM_ARRAYS;
                        break;
                    case KEY_LEFT:
                        effect_timer_select_previous_subeffect(selected_effect);
                        break;
                    case KEY_RIGHT:
                        effect_timer_select_next_subeffect(selected_effect);
                        break;
                    case ':':
                        effect_timer_select_set_advance_ms(selected_effect, 0);
                        break;
                    case ';':
                        {
                            int32_t advance_ms = effect_timer_select_get_advance_ms(selected_effect);
                            if(advance_ms > 1000) {
                                advance_ms -= 1000;
                            }
                            else {
                                advance_ms = 0;
                            }
                            effect_timer_select_set_advance_ms(selected_effect, advance_ms);
                        }
                        break;
                    case '\'':
                        {
                            int32_t advance_ms = effect_timer_select_get_advance_ms(selected_effect);
                            advance_ms += 1000;
                            effect_timer_select_set_advance_ms(selected_effect, advance_ms);
                        }
                        break;
                    case '<':
                        effect_timer_select_set_crossfade_ms(selected_effect, 0);
                        break;
                    case ',':
                        {
                            int32_t crossfade_ms = effect_timer_select_get_crossfade_ms(selected_effect);
                            if(crossfade_ms > 100) {
                                crossfade_ms -= 100;
                            }
                            else {
                                crossfade_ms = 0;
                            }
                            effect_timer_select_set_crossfade_ms(selected_effect, crossfade_ms);
                        }
                        break;
                    case '.':
                        {
                            int32_t crossfade_ms = effect_timer_select_get_crossfade_ms(selected_effect);
                            crossfade_ms += 100;
                            effect_timer_select_set_crossfade_ms(selected_effect, crossfade_ms);
                        }
                        break;
                    }
                }
            }
            ch = getch();
        }
    }
    
    artifactuary_ui_draw();
    
    if(artifactuary_last_process_nsec > 5000000 || artifactuary_last_frame_time_nsec > 34000000) {
        attron(A_BOLD);
    }
    move(7, 0);
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
    
    switch(artifactuary_mode) {
    case MODE_POWER_TEST: mode_name = "blank white - power test (debug)"; break;
    case MODE_FRAME: mode_name = "rainbow frames (debug)"; break;
    case MODE_PLAY: mode_name = "playing"; break;
    default: mode_name = "!UNKNOWN!"; break;
    }
    
    erase();
    
    move(0, 0);
    printw(
        "---ARTIFACTUARY---\n"
        "\n"
        "Pp play mode -- F frame test mode\n"
        "W - power test mode (all white, USE WITH CAUTION)\n"
        "\n"
        "current mode: %s\n", mode_name);
    if(artifactuary_mode == MODE_PLAY) {
        printw(
            "up/down array -- left/right effect\n"
            ";' advance time -- ,. crossfade time\n"
            "\n");
        for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
            int32_t id = effect_timer_select_get_current_subeffect(artifactuary_effects[i]);
            effect_t* effect = artifactuary_effects[i];
            
            if(i == artifactuary_ui_selected_array) {
                attron(A_BOLD);
            }
            printw("ARRAY %d: %s (advance %gs%s%s, crossfade %gs)\n",
                i,
                effect_timer_select_get_subeffect_name(effect, id),
                effect_timer_select_get_advance_ms(effect) / 1000.0f,
                effect_timer_select_get_advance_ms(effect) == 0 ? " [stop]" : "",
                effect_timer_select_get_shuffle(effect) ? " [shuffle]" : "",
                effect_timer_select_get_crossfade_ms(effect) / 1000.0f);
            attroff(A_BOLD);
        }
    }
#endif
}


void artifactuary_mode_power_test_init(void)
{
    // initialize all the state structures used by the panel image generation
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        artifactuary_effects[i] = effect_alloc();
        artifactuary_effects[i]->process = &artifactuary_mode_power_test_effect_process;
    }
}

void artifactuary_mode_frame_init(void)
{
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
        {{ 95,  95,  95, 255}}, // grey
        {{255, 255, 255, 255}}, // white
    };
    
    assert(ARTIFACTUARY_NUM_ARRAYS < 10);
    
    // initialize all the state structures used by the panel image generation
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        artifactuary_effects[i] = effect_alloc();
        artifactuary_effects[i]->process = &artifactuary_mode_frame_effect_process;
        artifactuary_effects[i]->void_state = (void*)array_colors[i].rgba;
    }
}


void artifactuary_mode_play_init(void)
{
    // initialize all the state structures used by the panel image generation
    for(int32_t i = 0; i < ARTIFACTUARY_NUM_ARRAYS; ++i) {
        int32_t width = artifactuary_arrays[i].width;
        int32_t height = artifactuary_arrays[i].height;
        
        artifactuary_effects[i] = effect_timer_select_create();
        effect_timer_select_set_advance_ms(artifactuary_effects[i], 30000 + 10000 * i);
        effect_timer_select_set_crossfade_ms(artifactuary_effects[i], 500);
        effect_timer_select_set_shuffle(artifactuary_effects[i], true);
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_vlad_fire_0_create(width, height), "Vlad fire 0");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_vlad_plasma_0_create(), "Vlad plasma 0");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_joe_fire_create(width, height), "Joe fire");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_joe_text_scroll_create(width, height, effect_joe_fire_create(width, height)), "Text scroll test");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_vlad_sinewave_0_create(), "Sinewave 0");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_vlad_sparkler_0_create(width, height), "Sparkler 0");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_vlad_rainbow_slider_0_create(), "Rainbow Slider 0");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_vlad_rainbow_ladder_0_create(), "Rainbow Ladder 0");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_vlad_acid_trap_0_create(), "Acid Trap 0");
        effect_timer_select_add_subeffect(artifactuary_effects[i], effect_vlad_mandelbrot_0_create(), "Mandelbrot 0");
    }
}


void artifactuary_mode_power_test_effect_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    // fill the array white
    memset(target_array->data, 255, target_array->width * target_array->height * sizeof (rgba_t));
}


void artifactuary_mode_frame_effect_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    int32_t width = target_array->width;
    int32_t height = target_array->height;
    rgba_t* data = target_array->data;
    rgba_t color;
    
    color.rgba = (uint32_t)void_state;
    
    // fill array with the appropriate color
    for(int32_t i = 0; i < width * height; ++i) {
        data[i] = color;
    }
    // draw white frame around the array
    for(int32_t j = 0; j < height; ++j) {
        data[j * width + 0].rgba = 0xFFFFFFFF;
        data[j * width + width - 1].rgba = 0xFFFFFFFF;
    }
    for(int32_t i = 0; i < width; ++i) {
        data[0 * width + i].rgba = 0xFFFFFFFF;
        data[(height - 1) * width + i].rgba = 0xFFFFFFFF;
    }
}


