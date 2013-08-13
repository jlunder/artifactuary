#include "artifactuary.h"


#include "fire.h"
#include "scroll.h"


rgba_t array_data[ARRAY_HEIGHT][ARRAY_STRIDE];


fire_state_t fire_state;
scroll_state_t scroll_state;


void artifactuary_init(void)
{
    fire_init(&fire_state);
    scroll_init(&scroll_state);
}


void artifactuary_process(float time)
{
    /*
    for(int j = 0; j < ARRAY_HEIGHT; ++j) {
        for(int i = 0; i < ARRAY_WIDTH; ++i) {
            array_colors[j][i][0] = 255;
            array_colors[j][i][1] = 255;
            array_colors[j][i][2] = 127;
        }
    }
    */
    fire_process(&fire_state, time, array_data);
    scroll_process(&scroll_state, time, array_data);
}


