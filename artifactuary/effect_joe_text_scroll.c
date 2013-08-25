#include "effect_joe_text_scroll.h"

#include "text.h"


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


void effect_joe_text_scroll_destroy(void* void_state);
void effect_joe_text_scroll_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


effect_t* effect_joe_text_scroll_create(int32_t width, int32_t height)
{
    effect_t* effect = effect_alloc();
    effect_joe_text_scroll_state_t* state = (effect_joe_text_scroll_state_t*)malloc(sizeof (effect_joe_text_scroll_state_t));
    
    effect->void_state = state;
    effect->process = &effect_joe_text_scroll_process;
    effect->destroy = &effect_joe_text_scroll_destroy;
    
    state->array_width = width << 8;
    state->current_message = NULL;
    state->current_message_width = 0;
    state->next_message = NULL;
    state->next_message_width = 0;
    state->queued_message = NULL;
    state->scroll_distance = 0;
    state->scroll_speed = ((1 << 8) * 10 + 15) / 30; // 10 / 30 pixels per frame = 3 pixels per second
    
    state->vertical_pos = 0;
    
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
    if(state->next_message != NULL) {
        free(state->next_message);
    }
    if(state->queued_message != NULL) {
        free(state->queued_message);
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
    
    do {
        stable = true;
        if((state->current_message != NULL) && (state->scroll_distance >= state->current_message_width + state->array_width)) {
            state->scroll_distance -= state->current_message_width;
            if(state->current_message != NULL) {
                free(state->current_message);
            }
            state->current_message = NULL;
            stable = false;
        }
        
        if((state->current_message == NULL) && (state->next_message != NULL)) {
            state->current_message = state->next_message;
            state->current_message_width = state->next_message_width;
            state->next_message = NULL;
            state->next_message_width = 0;
            stable = false;
        }
        
        if((state->next_message == NULL) && (state->queued_message != NULL)) {
            state->next_message = state->queued_message;
            state->next_message_width = (strlen(state->next_message) * 8) << 8;
            state->queued_message = NULL;
            stable = false;
        }
    } while(!stable);
    
    if(state->current_message == NULL) {
        state->scroll_distance = 0;
    }
    
    memset(target_array->data, 0, sizeof (rgba_t) * target_array->width * target_array->height);
    
    if(state->current_message != NULL) {
        rgba_t color = {{  0, 255,   0, 127}};
        text_draw_text(FONT_PLAIN_VAR_6, target_array, target_array->width - (state->scroll_distance >> 8), state->vertical_pos, state->current_message, color);
        if(state->next_message != NULL && (state->scroll_distance - state->current_message_width > 0)) {
            text_draw_text(FONT_PLAIN_VAR_6, target_array, target_array->width - ((state->scroll_distance - state->current_message_width) >> 8), state->vertical_pos, state->next_message, color);
        }
    }
}


