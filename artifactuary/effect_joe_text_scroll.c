#include "effect_joe_text_scroll.h"

#include <stdio.h>

#include "text.h"


typedef struct effect_joe_text_scroll_state
{
    int32_t array_width;
    
    char* current_message;
    int32_t current_message_width;
    
    int32_t vertical_pos;
    
    int32_t scroll_distance; // 24.8 fixed point, pixels between left edge of message and right edge of array
    int32_t scroll_speed; // 24.8 fixed point, pixels per frame
    
    rgba_t color;
    
    effect_t* background_effect;
} effect_joe_text_scroll_state_t;


void effect_joe_text_scroll_destroy(void* void_state);
void effect_joe_text_scroll_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


effect_t* effect_joe_text_scroll_create(int32_t width, int32_t height, rgba_t color, effect_t* background_effect)
{
    effect_t* effect = effect_alloc();
    effect_joe_text_scroll_state_t* state = (effect_joe_text_scroll_state_t*)malloc(sizeof (effect_joe_text_scroll_state_t));
    
    effect->void_state = state;
    effect->process = &effect_joe_text_scroll_process;
    effect->destroy = &effect_joe_text_scroll_destroy;
    
    state->array_width = width << 8;
    state->current_message = NULL;
    state->current_message_width = 0;
    state->vertical_pos = 1;
    state->scroll_distance = 0;
    state->scroll_speed = ((30 << 8) + 15) / 30; // 30 / 30 pixels per frame = 30 pixels per second
    
    state->color = color;
    
    state->background_effect = background_effect;
    
    state->current_message = strdup("Hello Playa!");
    state->current_message_width = text_get_text_width(FONT_PLAIN_VAR_6, state->current_message) << 8;
    
    return effect;
}


void effect_joe_text_scroll_destroy(void* void_state)
{
    effect_joe_text_scroll_state_t* state = (effect_joe_text_scroll_state_t*)void_state;
    
    if(state->current_message != NULL) {
        free(state->current_message);
    }
    if(state->background_effect != NULL) {
        effect_destroy(state->background_effect);
    }
    free(state);
}


void effect_joe_text_scroll_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    effect_joe_text_scroll_state_t* state = (effect_joe_text_scroll_state_t*)void_state;
    bool stable;
    
    assert((target_array->width << 8) == state->array_width);
    
    if(state->current_message != NULL) {
        state->scroll_distance += state->scroll_speed;
    }
    
    if((state->current_message != NULL) && (state->scroll_distance >= state->current_message_width + state->array_width)) {
        if(state->current_message != NULL) {
            free(state->current_message);
        }
        state->current_message = NULL;
    }
    
    if(state->current_message == NULL) {
        FILE* f = fopen("message", "rt");
        
        if(f != NULL) {
            state->current_message = malloc(10000);
            memset(state->current_message, 0, 10000);
            fread(state->current_message, 1, 9999, f);
            fclose(f);
            for(int i = 0; state->current_message[i] != 0; ++i) {
                if(state->current_message[i] == '\n') {
                    state->current_message[i] = 0;
                }
            }
        }
        else {
            // pause a while before checking the disk again
            state->current_message = strdup("");
        }
        state->current_message_width = text_get_text_width(FONT_PLAIN_VAR_6, state->current_message) << 8;
        state->scroll_distance = 0;
    }
    
    if(state->background_effect != NULL) {
        effect_process(state->background_effect, target_array, total_time_ns, frame_time_ns);
    }
    else {
        memset(target_array->data, 0, sizeof (rgba_t) * target_array->width * target_array->height);
    }
    
    if(state->current_message != NULL) {
        text_draw_text(FONT_PLAIN_VAR_6, target_array, target_array->width - (state->scroll_distance >> 8), state->vertical_pos, state->current_message, state->color);
    }
}


