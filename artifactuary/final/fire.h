#ifndef FIRE_H_INCLUDED
#define FIRE_H_INCLUDED


#include "artifactuary.h"


typedef struct fire_state
{
    uint8_t intensity[ARRAY_HEIGHT + 1][ARRAY_STRIDE];
} fire_state_t;


void fire_init(fire_state_t* state);
void fire_process(fire_state_t* state, float time, rgba_t target_array[ARRAY_HEIGHT][ARRAY_STRIDE]);


#endif
