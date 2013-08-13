#ifndef SCROLL_H_INCLUDED
#define SCROLL_H_INCLUDED


#include "artifactuary.h"


typedef struct scroll_state
{
    int32_t array_width;
    
    char* current_message;
    int32_t current_message_width;
    char* next_message;
    int32_t next_message_width;
    
    char* queued_message;
    
    int32_t scroll_distance; // 24:8 fixed point, pixels between left edge of message and right edge of array
    int32_t scroll_speed; // 24:8 fixed point, pixels per frame
    
    int32_t vertical_pos;
} scroll_state_t;


void scroll_init(scroll_state_t* state);
void scroll_process(scroll_state_t* state, float time, rgba_t target_array[ARRAY_HEIGHT][ARRAY_STRIDE]);


#endif

