#ifndef EFFECT_JOE_TEXT_SCROLL_H_INCLUDED
#define EFFECT_JOE_TEXT_SCROLL_H_INCLUDED


#include "artifactuary.h"


typedef struct effect_joe_text_scroll_state
{
    int32_t array_width;
    
    char* current_message;
    int32_t current_message_width;
    char* next_message;
    int32_t next_message_width;
    
    char* queued_message;
    
    int32_t scroll_distance; // 24.8 fixed point, pixels between left edge of message and right edge of array
    int32_t scroll_speed; // 24.8 fixed point, pixels per frame
    
    int32_t vertical_pos;
} effect_joe_text_scroll_state_t;


void effect_joe_text_scroll_init(effect_joe_text_scroll_state_t* state, int32_t width, int32_t height);
void effect_joe_text_scroll_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


#endif

