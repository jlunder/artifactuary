#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED


#include "artifactuary.h"


typedef void (*effect_process_function_t)(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);
typedef void (*effect_free_function_t)(void* void_state);


typedef struct effect
{
    void* void_state;
    effect_process_function_t process;
    effect_free_function_t destroy;
} effect_t;


effect_t* effect_alloc(void);
void effect_process(effect_t* effect, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);
void effect_destroy(effect_t* effect);


#endif
