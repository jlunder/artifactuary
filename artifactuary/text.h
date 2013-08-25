#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED


#include "artifactuary.h"

#include "array.h"


typedef enum {
    FONT_PLAIN_4_BY_6,
    FONT_PLAIN_VAR_6,
    FONT_C64_8X8,
} text_font_t;


typedef struct {
    int32_t height;
    int32_t ex_height;
    int32_t ell_height;
    int32_t baseline;
    int32_t em_width;
    int32_t en_width;
    int32_t glyph_spacing; // how much extra space to add between glyphs
} text_font_metrics_t;


text_font_metrics_t const* text_get_font_metrics(text_font_t font);
int32_t text_get_glyph_width(text_font_t font, char c);
int32_t text_get_text_width(text_font_t font, char const* c);

// x,y is the top-left corner of the glyph
int32_t text_draw_glyph(text_font_t font, array_t* dest_array, int32_t x, int32_t y, char c, rgba_t color);
int32_t text_draw_text(text_font_t font, array_t* dest_array, int32_t x, int32_t y, char const* text, rgba_t color);


#endif


