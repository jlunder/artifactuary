#ifndef FIRE_H_INCLUDED
#define FIRE_H_INCLUDED


#include "artifactuary.h"


typedef struct fire_state
{
    int32_t width;
    int32_t height;
    int32_t jitter;
    uint8_t* intensity;
} fire_state_t;


void fire_init(fire_state_t* state, int32_t width, int32_t height);
void fire_process(fire_state_t* state, float time, array_t* target_array);


#endif
