#ifndef EFFECT_TIMER_SELECT_H_INCLUDED
#define EFFECT_TIMER_SELECT_H_INCLUDED


#include "artifactuary.h"


typedef struct effect_timer_select_state
{
} effect_timer_select_state_t;


void effect_joe_fire_init(effect_timer_select_state_t* state, int32_t width, int32_t height);
void effect_joe_fire_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


#endif
