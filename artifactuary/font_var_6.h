#ifndef FONT_VAR_6_H_INCLUDED
#define FONT_VAR_6_H_INCLUDED


#include <stdint.h>


#define FONT_VAR_6_HEIGHT 6
#define FONT_VAR_6_BASELINE 4
#define FONT_VAR_6_EM_WIDTH 5
#define FONT_VAR_6_EN_WIDTH 4
#define FONT_VAR_6_GLYPH_SPACING 1


extern struct font_var_6_glyph {
    uint8_t width;
    uint8_t scanlines[6];
} font_var_6_glyphs[96];


#endif


