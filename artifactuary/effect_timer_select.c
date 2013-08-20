#include "effect_timer_select.h"


#define EFFECT_TIMER_SELECT_MAX_SUBEFFECTS 100


typedef struct effect_timer_select_state
{
    int32_t advance_ms;
    int32_t crossfade_ms;
    bool shuffle;
    
    int64_t last_transition_time_ns;
    int32_t cur_subeffect;
    int32_t next_subeffect;
    
    int32_t num_subeffects;
    struct {
        effect_t* effect;
        char const* name;
    } subeffects[EFFECT_TIMER_SELECT_MAX_SUBEFFECTS];
} effect_timer_select_state_t;


void effect_timer_select_destroy(void* void_state);
void effect_timer_select_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


effect_t* effect_timer_select_create()
{
    effect_t* effect = effect_alloc();
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)malloc(sizeof (effect_timer_select_state_t));
    
    effect->void_state = state;
    effect->process = &effect_timer_select_process;
    effect->destroy = &effect_timer_select_destroy;
    
    state->advance_ms = 10000;
    state->crossfade_ms = 1000;
    state->shuffle = true;
    
    state->last_transition_time_ns = -1;
    state->cur_subeffect = 0;
    state->next_subeffect = -1;
    
    state->num_subeffects = 0;
    memset(state->subeffects, 0, sizeof state->subeffects);
    
    return effect;
}


void effect_timer_select_destroy(void* void_state)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)void_state;
    
    for(int32_t i = 0; i < state->num_subeffects; ++i) {
        effect_destroy(state->subeffects[i].effect);
    }
    free(state);
}


void effect_timer_select_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)void_state;
    
    if(state->last_transition_time_ns < 0) {
        state->last_transition_time_ns = total_time_ns;
    }
    
    if(state->advance_ms > 0) {
        if(state->last_transition_time_ns + (int64_t)state->advance_ms * 1000000 <= total_time_ns) {
            if(state->next_subeffect >= 0) {
                // not yet finished last transition? hmm
                state->cur_subeffect = state->next_subeffect;
            }
        
            if(state->shuffle) {
                state->next_subeffect = rand() % state->num_subeffects;
            }
            else {
                state->next_subeffect = (state->next_subeffect + 1) % state->num_subeffects;
            }
        
            state->last_transition_time_ns += (int64_t)state->advance_ms * 1000000;
            if(state->last_transition_time_ns + (int64_t)state->advance_ms * 1000000 <= total_time_ns) {
                state->last_transition_time_ns = total_time_ns;
            }
        }
    }
    
    if(state->next_subeffect >= 0) {
        if(state->last_transition_time_ns + (int64_t)state->crossfade_ms * 1000000 <= total_time_ns) {
            state->cur_subeffect = state->next_subeffect;
            state->next_subeffect = -1;
        }
    }
    
    if(state->next_subeffect >= 0) {
        int32_t width = target_array->width;
        int32_t height = target_array->height;
        rgba_t* temp_array_data = (rgba_t*)alloca(width * height * sizeof (rgba_t));
        array_t temp_array = {.width = width, .height = height, .data = temp_array_data};
        uint8_t crossfade = ((total_time_ns - state->last_transition_time_ns) * 256) / (int64_t)state->crossfade_ms * 1000000;
        
        effect_process(state->subeffects[state->cur_subeffect].effect, target_array, total_time_ns, frame_time_ns);
        effect_process(state->subeffects[state->next_subeffect].effect, &temp_array, total_time_ns, frame_time_ns);
        array_composite_explicit_alpha(target_array, &temp_array, crossfade);
    }
}


bool effect_timer_select_get_shuffle(effect_t* effect)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    return state->shuffle;
}


void effect_timer_select_set_shuffle(effect_t* effect, bool shuffle)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    state->shuffle = shuffle;
}


int32_t effect_timer_select_add_subeffect(effect_t* effect, effect_t* subeffect, char const* subeffect_name)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    assert(state->num_subeffects < EFFECT_TIMER_SELECT_MAX_SUBEFFECTS);
    
    state->subeffects[state->num_subeffects].effect = subeffect;
    state->subeffects[state->num_subeffects].name = subeffect_name;
    return state->num_subeffects++;
}


char const* effect_timer_select_get_subeffect_name(effect_t* effect, int32_t subeffect_id)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    assert(subeffect_id >= 0 && subeffect_id < state->num_subeffects);
    
    return state->subeffects[subeffect_id].name;
}


int32_t effect_timer_select_get_current_subeffect(effect_t* effect)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    return state->cur_subeffect;
}


void effect_timer_select_next_subeffect(effect_t* effect)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    effect_timer_select_goto_subeffect(effect, (state->cur_subeffect + 1) % state->num_subeffects);
}


void effect_timer_select_previous_subeffect(effect_t* effect)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    effect_timer_select_goto_subeffect(effect, (state->cur_subeffect + state->num_subeffects - 1) % state->num_subeffects);
}


void effect_timer_select_goto_subeffect(effect_t* effect, int32_t subeffect_id)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    assert(subeffect_id >= 0 && subeffect_id < state->num_subeffects);
    
    if(state->next_subeffect >= 0) {
        state->cur_subeffect = state->next_subeffect;
    }
    state->next_subeffect = subeffect_id;
    state->last_transition_time_ns = -1;
}


int32_t effect_timer_select_get_advance_ms(effect_t* effect)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    return state->advance_ms;
}


void effect_timer_select_set_advance_ms(effect_t* effect, int32_t advance_ms)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    state->last_transition_time_ns = -1;
    state->advance_ms = advance_ms;
}


int32_t effect_timer_select_get_crossfade_ms(effect_t* effect)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    return state->crossfade_ms;
}


void effect_timer_select_set_crossfade_ms(effect_t* effect, int32_t crossfade_ms)
{
    effect_timer_select_state_t* state = (effect_timer_select_state_t*)effect->void_state;
    
    assert(effect->process == &effect_timer_select_process);
    
    state->crossfade_ms = crossfade_ms;
}


