#include "effect_joe_fire.h"


//void effect_blank_destroy(void* void_state);
void effect_blank_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


effect_t* effect_blank_create(rgba_t color)
{
    effect_t* effect = effect_alloc();
    
    // hack: stuff the color into the state pointer because it's tiny
    effect->void_state = (void*)color.rgba;
    effect->process = &effect_blank_process;
    
    return effect;
}


void effect_blank_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    int32_t width = target_array->width;
    int32_t height = target_array->height;
    rgba_t* data = target_array->data;
    rgba_t color = {.rgba = (uint32_t)void_state};
    
    for(int32_t i = 0; i < width * height; ++i) {
        target_array->data[i] = color;
    }
}