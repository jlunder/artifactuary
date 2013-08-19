#ifndef EFFECT_VLAD_FIRE_0_H_INCLUDED
#define EFFECT_VLAD_FIRE_0_H_INCLUDED


#include "artifactuary.h"


typedef struct effect_vlad_fire_0_state
{
    int32_t width;
    int32_t height;
    uint8_t* intensity;
} effect_vlad_fire_0_state_t;


void effect_vlad_fire_0_init(effect_vlad_fire_0_state_t* state, int32_t width, int32_t height);
void effect_vlad_fire_0_shutdown(effect_vlad_fire_0_state_t* state);
void effect_vlad_fire_0_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


#endif
