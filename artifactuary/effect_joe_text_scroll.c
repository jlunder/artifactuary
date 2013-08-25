#include "effect_joe_text_scroll.h"

#include "text.h"


effect_joe_text_scroll_glyph_t const* effect_joe_text_scroll_get_glyph(char c);
void effect_joe_text_scroll_draw(effect_joe_text_scroll_state_t* state, array_t* target_array, char const* text, int32_t text_offset, rgba_t text_color);


void effect_joe_text_scroll_init(effect_joe_text_scroll_state_t* state, int32_t width, int32_t height)
{
    state->array_width = width << 8;
    state->current_message = NULL;
    state->current_message_width = 0;
    state->next_message = NULL;
    state->next_message_width = 0;
    state->queued_message = NULL;
    state->scroll_distance = 0;
    state->scroll_speed = ((1 << 8) * 10 + 15) / 30; // 10 / 30 pixels per frame = 3 pixels per second
    
    assert(height >= EFFECT_JOE_TEXT_SCROLL_CHAR_HEIGHT);
    
    state->vertical_pos = 0;
    
    state->current_message = strdup("Hello Playa!");
    state->current_message_width = (strlen(state->current_message) * EFFECT_JOE_TEXT_SCROLL_CHAR_WIDTH) << 8;
}


void effect_joe_text_scroll_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    effect_joe_text_scroll_state_t* state = (effect_joe_text_scroll_state_t*)void_state;
    bool stable;
    
    assert((target_array->width << 8) == state->array_width);
    assert(target_array->height >= state->vertical_pos + EFFECT_JOE_TEXT_SCROLL_CHAR_HEIGHT);
    
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
    
    if(state->current_message != NULL) {
        rgba_t color = {{  0, 255,   0, 255}};
        effect_joe_text_scroll_draw(state, target_array, state->current_message, state->scroll_distance, color);
        if(state->next_message != NULL) {
            effect_joe_text_scroll_draw(state, target_array, state->next_message, state->scroll_distance - state->current_message_width, color);
        }
    }
}


effect_joe_text_scroll_glyph_t const* effect_joe_text_scroll_get_glyph(char c)
{
    if((c < EFFECT_JOE_TEXT_SCROLL_CHARSET_START) || (c >= EFFECT_JOE_TEXT_SCROLL_CHARSET_END)) {
        return &effect_joe_text_scroll_font_plain_4x6[rand() % (EFFECT_JOE_TEXT_SCROLL_CHARSET_END - EFFECT_JOE_TEXT_SCROLL_CHARSET_START)];
    }
    else {
        return &effect_joe_text_scroll_font_plain_4x6[c - EFFECT_JOE_TEXT_SCROLL_CHARSET_START];
    }
}


void effect_joe_text_scroll_draw(effect_joe_text_scroll_state_t* state, array_t* target_array, char const* text, int32_t text_offset, rgba_t text_color)
{
    int32_t pixel_pos = (state->array_width - text_offset) >> 8;
    
    //printf("drawing message \"%s\" at [%d] %d,%d\n", text, text_offset, pixel_pos, state->vertical_pos);
    
    while(pixel_pos < 0)
    {
        if(*text == 0) {
            return;
        }
        
        if(pixel_pos + EFFECT_JOE_TEXT_SCROLL_CHAR_WIDTH > 0) {
            effect_joe_text_scroll_glyph_t const* glyph = effect_joe_text_scroll_get_glyph(*text);
            
            // a char clipped on the left
            for(int j = 0; j < EFFECT_JOE_TEXT_SCROLL_CHAR_HEIGHT; ++j) {
                uint32_t scanline = glyph->scanlines[j];
                scanline = scanline >> -pixel_pos;
                for(int i = -pixel_pos; (scanline != 0) && (i < EFFECT_JOE_TEXT_SCROLL_CHAR_WIDTH); ++i) {
                    if(scanline & 1) {
                        target_array->data[(state->vertical_pos + j) * target_array->width + pixel_pos + i] = text_color;
                    }
                    scanline = scanline >> 1;
                }
            }
        }
        
        pixel_pos += EFFECT_JOE_TEXT_SCROLL_CHAR_WIDTH;
        ++text;
    }
    
    while(pixel_pos + EFFECT_JOE_TEXT_SCROLL_CHAR_WIDTH < target_array->width) {
        effect_joe_text_scroll_glyph_t const* glyph;
        
        if(*text == 0) {
            return;
        }
        
        glyph = effect_joe_text_scroll_get_glyph(*text);
        // an unclipped char
        for(int j = 0; j < EFFECT_JOE_TEXT_SCROLL_CHAR_HEIGHT; ++j) {
            uint32_t scanline = glyph->scanlines[j];
            for(int i = 0; (scanline != 0) && (i < EFFECT_JOE_TEXT_SCROLL_CHAR_WIDTH); ++i) {
                if(scanline & 1) {
                     target_array->data[(state->vertical_pos + j) * target_array->width + pixel_pos + i] = text_color;
                }
                scanline = scanline >> 1;
            }
        }
        
        pixel_pos += EFFECT_JOE_TEXT_SCROLL_CHAR_WIDTH;
        ++text;
    }
    
    if(*text == 0) {
        return;
    }
    if((pixel_pos < target_array->width) && (*text != 0)) {
        effect_joe_text_scroll_glyph_t const* glyph = effect_joe_text_scroll_get_glyph(*text);
        // a clipped char on the right!
        for(int j = 0; j < EFFECT_JOE_TEXT_SCROLL_CHAR_HEIGHT; ++j) {
            uint32_t scanline = glyph->scanlines[j];
            for(int i = 0; (scanline != 0) && (i < (target_array->width - pixel_pos)); ++i) {
                if(scanline & 1) {
                    target_array->data[(state->vertical_pos + j) * target_array->width + pixel_pos + i] = text_color;
                }
                scanline = scanline >> 1;
            }
        }
    }
}


