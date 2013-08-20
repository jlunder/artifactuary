#include "effect_timer_select.h"


effect_t* effect_alloc(void)
{
    effect_t* effect = (effect_t*)malloc(sizeof (effect_t));
    
    memset(effect, 0, sizeof *effect);
    return effect;
}

void effect_process(effect_t* effect, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    effect->process(effect->void_state, target_array, total_time_ns, frame_time_ns);
}


void effect_destroy(effect_t* effect)
{
    if(effect->destroy != NULL) {
        effect->destroy(effect->void_state);
    }
}


