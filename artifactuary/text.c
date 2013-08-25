#include "text.h"


#include "font_var_6.h"


typedef struct {
    text_font_metrics_t metrics;
    const int8_t* glyphs[96];
} text_font_definition_t;


text_font_definition_t text_font_var_6 = {
    .metrics = {
        .height = FONT_VAR_6_HEIGHT,
        .baseline = FONT_VAR_6_BASELINE,
        .em_width = FONT_VAR_6_EM_WIDTH,
        .en_width = FONT_VAR_6_EN_WIDTH,
        .glyph_spacing = FONT_VAR_6_GLYPH_SPACING,
    },
    .glyphs = {
        (uint8_t*)&font_var_6_glyphs[ 0], (uint8_t*)&font_var_6_glyphs[ 1],
        (uint8_t*)&font_var_6_glyphs[ 2], (uint8_t*)&font_var_6_glyphs[ 3],
        (uint8_t*)&font_var_6_glyphs[ 4], (uint8_t*)&font_var_6_glyphs[ 5],
        (uint8_t*)&font_var_6_glyphs[ 6], (uint8_t*)&font_var_6_glyphs[ 7],
        (uint8_t*)&font_var_6_glyphs[ 8], (uint8_t*)&font_var_6_glyphs[ 9],
        (uint8_t*)&font_var_6_glyphs[10], (uint8_t*)&font_var_6_glyphs[11],
        (uint8_t*)&font_var_6_glyphs[12], (uint8_t*)&font_var_6_glyphs[13],
        (uint8_t*)&font_var_6_glyphs[14], (uint8_t*)&font_var_6_glyphs[15],
        (uint8_t*)&font_var_6_glyphs[16], (uint8_t*)&font_var_6_glyphs[17],
        (uint8_t*)&font_var_6_glyphs[18], (uint8_t*)&font_var_6_glyphs[19],
        (uint8_t*)&font_var_6_glyphs[20], (uint8_t*)&font_var_6_glyphs[21],
        (uint8_t*)&font_var_6_glyphs[22], (uint8_t*)&font_var_6_glyphs[23],
        (uint8_t*)&font_var_6_glyphs[24], (uint8_t*)&font_var_6_glyphs[25],
        (uint8_t*)&font_var_6_glyphs[26], (uint8_t*)&font_var_6_glyphs[27],
        (uint8_t*)&font_var_6_glyphs[28], (uint8_t*)&font_var_6_glyphs[29],
        (uint8_t*)&font_var_6_glyphs[30], (uint8_t*)&font_var_6_glyphs[31],
        (uint8_t*)&font_var_6_glyphs[32], (uint8_t*)&font_var_6_glyphs[33],
        (uint8_t*)&font_var_6_glyphs[34], (uint8_t*)&font_var_6_glyphs[35],
        (uint8_t*)&font_var_6_glyphs[36], (uint8_t*)&font_var_6_glyphs[37],
        (uint8_t*)&font_var_6_glyphs[38], (uint8_t*)&font_var_6_glyphs[39],
        (uint8_t*)&font_var_6_glyphs[40], (uint8_t*)&font_var_6_glyphs[41],
        (uint8_t*)&font_var_6_glyphs[42], (uint8_t*)&font_var_6_glyphs[43],
        (uint8_t*)&font_var_6_glyphs[44], (uint8_t*)&font_var_6_glyphs[45],
        (uint8_t*)&font_var_6_glyphs[46], (uint8_t*)&font_var_6_glyphs[47],
        (uint8_t*)&font_var_6_glyphs[48], (uint8_t*)&font_var_6_glyphs[49],
        (uint8_t*)&font_var_6_glyphs[50], (uint8_t*)&font_var_6_glyphs[51],
        (uint8_t*)&font_var_6_glyphs[52], (uint8_t*)&font_var_6_glyphs[53],
        (uint8_t*)&font_var_6_glyphs[54], (uint8_t*)&font_var_6_glyphs[55],
        (uint8_t*)&font_var_6_glyphs[56], (uint8_t*)&font_var_6_glyphs[57],
        (uint8_t*)&font_var_6_glyphs[58], (uint8_t*)&font_var_6_glyphs[59],
        (uint8_t*)&font_var_6_glyphs[60], (uint8_t*)&font_var_6_glyphs[61],
        (uint8_t*)&font_var_6_glyphs[62], (uint8_t*)&font_var_6_glyphs[63],
        (uint8_t*)&font_var_6_glyphs[64], (uint8_t*)&font_var_6_glyphs[65],
        (uint8_t*)&font_var_6_glyphs[66], (uint8_t*)&font_var_6_glyphs[67],
        (uint8_t*)&font_var_6_glyphs[68], (uint8_t*)&font_var_6_glyphs[69],
        (uint8_t*)&font_var_6_glyphs[70], (uint8_t*)&font_var_6_glyphs[71],
        (uint8_t*)&font_var_6_glyphs[72], (uint8_t*)&font_var_6_glyphs[73],
        (uint8_t*)&font_var_6_glyphs[74], (uint8_t*)&font_var_6_glyphs[75],
        (uint8_t*)&font_var_6_glyphs[76], (uint8_t*)&font_var_6_glyphs[77],
        (uint8_t*)&font_var_6_glyphs[78], (uint8_t*)&font_var_6_glyphs[79],
        (uint8_t*)&font_var_6_glyphs[80], (uint8_t*)&font_var_6_glyphs[81],
        (uint8_t*)&font_var_6_glyphs[82], (uint8_t*)&font_var_6_glyphs[83],
        (uint8_t*)&font_var_6_glyphs[84], (uint8_t*)&font_var_6_glyphs[85],
        (uint8_t*)&font_var_6_glyphs[86], (uint8_t*)&font_var_6_glyphs[87],
        (uint8_t*)&font_var_6_glyphs[88], (uint8_t*)&font_var_6_glyphs[89],
        (uint8_t*)&font_var_6_glyphs[90], (uint8_t*)&font_var_6_glyphs[91],
        (uint8_t*)&font_var_6_glyphs[92], (uint8_t*)&font_var_6_glyphs[93],
        (uint8_t*)&font_var_6_glyphs[94], (uint8_t*)&font_var_6_glyphs[95],
    },
};


text_font_metrics_t const* text_get_font_metrics(text_font_t font)
{
    // haven't stubbed in font data for other fonts ATM
    assert(font == FONT_PLAIN_VAR_6);
    
    return &text_font_var_6.metrics;
}

int32_t text_get_glyph_width(text_font_t font, char c)
{
    // haven't stubbed in font data for other fonts ATM
    assert(font == FONT_PLAIN_VAR_6);
    
    if(c < 32 || c >= 128) {
        return text_font_var_6.metrics.em_width;
    }
    else {
        return text_font_var_6.glyphs[c - 32][0];
    }
}


int32_t text_get_text_width(text_font_t font, char const* text)
{
    int32_t width = -text_font_var_6.metrics.glyph_spacing;
    
    // haven't stubbed in font data for other fonts ATM
    assert(font == FONT_PLAIN_VAR_6);
    
    if(*text == 0) {
        return 0;
    }
    
    for(char const* p = text; *p != 0; ++p) {
        char c = *p;
        if(c < 32 || c >= 128) {
            width += text_font_var_6.metrics.em_width;
        }
        else {
            width += text_font_var_6.glyphs[c - 32][0];
        }
        width += text_font_var_6.metrics.glyph_spacing;
    }
    
    return width;
}


int32_t text_draw_glyph(text_font_t font, array_t* dest_array, int32_t x, int32_t y, char c, rgba_t color)
{
    int32_t array_width = dest_array->width;
    int32_t array_height = dest_array->height;
    rgba_t* array_data = dest_array->data;
    uint8_t const* glyph_scanlines;
    int32_t glyph_width;
    int32_t glyph_clipped_width;
    int32_t glyph_clipped_height = text_font_var_6.metrics.height;
    int32_t initial_x;
    int32_t initial_y;
    int32_t scanline_offset;
    
    // haven't stubbed in font data for other fonts ATM
    assert(font == FONT_PLAIN_VAR_6);
    
    if(c >= 32 && c < 128) {
        glyph_scanlines = text_font_var_6.glyphs[c - 32] + 1;
        glyph_width = text_font_var_6.glyphs[c - 32][0];
    }
    else {
        glyph_scanlines = text_font_var_6.glyphs[rand() % 96] + 1;
        glyph_width = text_font_var_6.metrics.em_width;
    }
    
    if(x >= array_width || x <= -glyph_width ||
        y >= array_height || y <= -text_font_var_6.metrics.height)
    {
        return glyph_width;
    }
    
    if(x >= 0) {
        initial_x = x;
        scanline_offset = 0;
        glyph_clipped_width = glyph_width;
    }
    else {
        initial_x = 0;
        scanline_offset = -x;
        glyph_clipped_width = glyph_width + x;
    }
    
    if(initial_x + glyph_clipped_width > array_width) {
        glyph_clipped_width = array_width - initial_x;
    }
    
    if(y >= 0) {
        initial_y = y;
        glyph_clipped_height = text_font_var_6.metrics.height;
    }
    else {
        initial_y = 0;
        glyph_scanlines += -y;
        glyph_clipped_height = text_font_var_6.metrics.height + y;
    }
    
    if(initial_y + glyph_clipped_height > array_height) {
        glyph_clipped_height = array_height - initial_y;
    }
    
    assert(array_width > 0);
    assert(array_height > 0);
    assert(glyph_clipped_width > 0); // else we should have early-outed already
    assert(glyph_clipped_height > 0);
    
    for(int j = 0; j < glyph_clipped_height; ++j) {
        uint32_t scanline = glyph_scanlines[j];
        scanline = scanline >> scanline_offset;
        for(int i = 0; i < glyph_clipped_width; ++i) {
            if(scanline == 0) {
                break;
            }
            if(scanline & 1) {
                rgba_combine_source_alpha(&array_data[(initial_y + j) * array_width + initial_x + i], color);
            }
            scanline = scanline >> 1;
        }
    }
    
    return glyph_width;
}


int32_t text_draw_text(text_font_t font, array_t* dest_array, int32_t x, int32_t y, char const* text, rgba_t color)
{
    int32_t current_width = 0;
    
    // haven't stubbed in font data for other fonts ATM
    assert(font == FONT_PLAIN_VAR_6);
    
    if(*text == 0) {
        return 0;
    }
    
    for(char const* p = text; *p != 0; ++p) {
        current_width += text_draw_glyph(font, dest_array, x + current_width, y, *p, color);
        current_width += text_font_var_6.metrics.glyph_spacing;
    }
    
    return current_width - text_font_var_6.metrics.glyph_spacing;
}


/*
effect_joe_text_scroll_glyph_t effect_joe_text_scroll_font_plain_4x6[EFFECT_JOE_TEXT_SCROLL_CHARSET_END - EFFECT_JOE_TEXT_SCROLL_CHARSET_START] = { 
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x02, 0x02, 0x02, 0x00, 0x02, 0x00}},
    {.scanlines = {0x05, 0x05, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x06, 0x0F, 0x06, 0x0F, 0x06, 0x00}},
    {.scanlines = {0x02, 0x06, 0x02, 0x03, 0x02, 0x00}},
    {.scanlines = {0x03, 0x05, 0x02, 0x05, 0x06, 0x00}},
    {.scanlines = {0x02, 0x05, 0x06, 0x0D, 0x0E, 0x00}},
    {.scanlines = {0x04, 0x02, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x04, 0x02, 0x02, 0x02, 0x04, 0x00}},
    {.scanlines = {0x01, 0x02, 0x02, 0x02, 0x01, 0x00}},
    {.scanlines = {0x00, 0x02, 0x07, 0x07, 0x02, 0x00}},
    {.scanlines = {0x00, 0x02, 0x07, 0x02, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x02, 0x01}},
    {.scanlines = {0x00, 0x00, 0x07, 0x00, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x02, 0x00}},
    {.scanlines = {0x00, 0x04, 0x02, 0x02, 0x01, 0x00}},
    {.scanlines = {0x02, 0x05, 0x07, 0x05, 0x02, 0x00}},
    {.scanlines = {0x02, 0x03, 0x02, 0x02, 0x07, 0x00}},
    {.scanlines = {0x03, 0x04, 0x02, 0x01, 0x07, 0x00}},
    {.scanlines = {0x03, 0x04, 0x03, 0x04, 0x03, 0x00}},
    {.scanlines = {0x05, 0x05, 0x07, 0x04, 0x04, 0x00}},
    {.scanlines = {0x07, 0x01, 0x03, 0x04, 0x03, 0x00}},
    {.scanlines = {0x02, 0x01, 0x03, 0x05, 0x02, 0x00}},
    {.scanlines = {0x07, 0x04, 0x02, 0x02, 0x02, 0x00}},
    {.scanlines = {0x02, 0x05, 0x02, 0x05, 0x02, 0x00}},
    {.scanlines = {0x02, 0x05, 0x06, 0x04, 0x03, 0x00}},
    {.scanlines = {0x00, 0x00, 0x02, 0x00, 0x02, 0x00}},
    {.scanlines = {0x00, 0x00, 0x02, 0x00, 0x02, 0x01}},
    {.scanlines = {0x04, 0x02, 0x01, 0x02, 0x04, 0x00}},
    {.scanlines = {0x00, 0x07, 0x00, 0x07, 0x00, 0x00}},
    {.scanlines = {0x01, 0x02, 0x04, 0x02, 0x01, 0x00}},
    {.scanlines = {0x03, 0x04, 0x02, 0x00, 0x02, 0x00}},
    {.scanlines = {0x02, 0x05, 0x05, 0x01, 0x06, 0x00}},
    {.scanlines = {0x02, 0x02, 0x05, 0x07, 0x05, 0x00}},
    {.scanlines = {0x03, 0x05, 0x03, 0x05, 0x03, 0x00}},
    {.scanlines = {0x06, 0x01, 0x01, 0x01, 0x06, 0x00}},
    {.scanlines = {0x03, 0x05, 0x05, 0x05, 0x03, 0x00}},
    {.scanlines = {0x07, 0x01, 0x03, 0x01, 0x07, 0x00}},
    {.scanlines = {0x07, 0x01, 0x03, 0x01, 0x01, 0x00}},
    {.scanlines = {0x06, 0x01, 0x05, 0x05, 0x06, 0x00}},
    {.scanlines = {0x05, 0x05, 0x07, 0x05, 0x05, 0x00}},
    {.scanlines = {0x07, 0x02, 0x02, 0x02, 0x07, 0x00}},
    {.scanlines = {0x06, 0x04, 0x04, 0x04, 0x03, 0x00}},
    {.scanlines = {0x05, 0x05, 0x03, 0x05, 0x05, 0x00}},
    {.scanlines = {0x01, 0x01, 0x01, 0x01, 0x07, 0x00}},
    {.scanlines = {0x05, 0x07, 0x07, 0x05, 0x05, 0x00}},
    {.scanlines = {0x04, 0x05, 0x07, 0x05, 0x01, 0x00}},
    {.scanlines = {0x07, 0x05, 0x05, 0x05, 0x07, 0x00}},
    {.scanlines = {0x03, 0x05, 0x03, 0x01, 0x01, 0x00}},
    {.scanlines = {0x07, 0x05, 0x05, 0x07, 0x07, 0x04}},
    {.scanlines = {0x03, 0x05, 0x03, 0x05, 0x05, 0x00}},
    {.scanlines = {0x06, 0x01, 0x02, 0x04, 0x03, 0x00}},
    {.scanlines = {0x07, 0x02, 0x02, 0x02, 0x02, 0x00}},
    {.scanlines = {0x05, 0x05, 0x05, 0x05, 0x07, 0x00}},
    {.scanlines = {0x05, 0x05, 0x05, 0x02, 0x02, 0x00}},
    {.scanlines = {0x05, 0x05, 0x07, 0x07, 0x05, 0x00}},
    {.scanlines = {0x05, 0x05, 0x02, 0x05, 0x05, 0x00}},
    {.scanlines = {0x05, 0x05, 0x07, 0x02, 0x02, 0x00}},
    {.scanlines = {0x07, 0x04, 0x02, 0x01, 0x07, 0x00}},
    {.scanlines = {0x07, 0x01, 0x01, 0x01, 0x07, 0x00}},
    {.scanlines = {0x00, 0x01, 0x02, 0x02, 0x04, 0x00}},
    {.scanlines = {0x07, 0x04, 0x04, 0x04, 0x07, 0x00}},
    {.scanlines = {0x02, 0x05, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x0F, 0x00}},
    {.scanlines = {0x02, 0x04, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x00, 0x03, 0x06, 0x05, 0x06, 0x00}},
    {.scanlines = {0x01, 0x01, 0x03, 0x05, 0x03, 0x00}},
    {.scanlines = {0x00, 0x00, 0x06, 0x01, 0x06, 0x00}},
    {.scanlines = {0x04, 0x04, 0x06, 0x05, 0x06, 0x00}},
    {.scanlines = {0x00, 0x06, 0x07, 0x01, 0x06, 0x00}},
    {.scanlines = {0x04, 0x02, 0x07, 0x02, 0x02, 0x00}},
    {.scanlines = {0x00, 0x06, 0x05, 0x06, 0x04, 0x03}},
    {.scanlines = {0x00, 0x01, 0x03, 0x05, 0x05, 0x00}},
    {.scanlines = {0x00, 0x02, 0x00, 0x02, 0x02, 0x00}},
    {.scanlines = {0x00, 0x04, 0x00, 0x04, 0x04, 0x03}},
    {.scanlines = {0x00, 0x01, 0x05, 0x03, 0x05, 0x00}},
    {.scanlines = {0x03, 0x02, 0x02, 0x02, 0x02, 0x00}},
    {.scanlines = {0x00, 0x00, 0x07, 0x07, 0x05, 0x00}},
    {.scanlines = {0x00, 0x00, 0x03, 0x05, 0x05, 0x00}},
    {.scanlines = {0x00, 0x00, 0x07, 0x05, 0x07, 0x00}},
    {.scanlines = {0x00, 0x00, 0x03, 0x05, 0x03, 0x01}},
    {.scanlines = {0x00, 0x00, 0x06, 0x05, 0x0E, 0x04}},
    {.scanlines = {0x00, 0x00, 0x07, 0x01, 0x01, 0x00}},
    {.scanlines = {0x00, 0x06, 0x03, 0x04, 0x03, 0x00}},
    {.scanlines = {0x02, 0x07, 0x02, 0x02, 0x04, 0x00}},
    {.scanlines = {0x00, 0x00, 0x05, 0x05, 0x06, 0x00}},
    {.scanlines = {0x00, 0x00, 0x05, 0x05, 0x02, 0x00}},
    {.scanlines = {0x00, 0x00, 0x05, 0x07, 0x07, 0x00}},
    {.scanlines = {0x00, 0x00, 0x05, 0x02, 0x05, 0x00}},
    {.scanlines = {0x00, 0x00, 0x05, 0x07, 0x04, 0x03}},
    {.scanlines = {0x00, 0x07, 0x02, 0x01, 0x07, 0x00}},
    {.scanlines = {0x06, 0x02, 0x03, 0x02, 0x06, 0x00}},
    {.scanlines = {0x02, 0x02, 0x02, 0x02, 0x02, 0x00}},
    {.scanlines = {0x03, 0x02, 0x06, 0x02, 0x03, 0x00}},
    {.scanlines = {0x0A, 0x05, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
};
*/

/*
effect_joe_text_scroll_glyph_t effect_joe_text_scroll_font_c64_8x8[EFFECT_JOE_TEXT_SCROLL_CHARSET_END - EFFECT_JOE_TEXT_SCROLL_CHARSET_START] = { 
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00}},
    {.scanlines = {0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x66, 0x66, 0xFF, 0x66, 0xFF, 0x66, 0x66, 0x00}},
    {.scanlines = {0x18, 0x7C, 0x06, 0x3C, 0x60, 0x3E, 0x18, 0x00}},
    {.scanlines = {0x46, 0x66, 0x30, 0x18, 0x0C, 0x66, 0x62, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x3C, 0x1C, 0xE6, 0x66, 0xFC, 0x00}},
    {.scanlines = {0x60, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00}},
    {.scanlines = {0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00}},
    {.scanlines = {0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00}},
    {.scanlines = {0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C}},
    {.scanlines = {0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00}},
    {.scanlines = {0x00, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x76, 0x6E, 0x66, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x18, 0x18, 0x1C, 0x18, 0x18, 0x18, 0x7E, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x60, 0x30, 0x0C, 0x06, 0x7E, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x60, 0x38, 0x60, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x60, 0x70, 0x78, 0x66, 0xFE, 0x60, 0x60, 0x00}},
    {.scanlines = {0x7E, 0x06, 0x3E, 0x60, 0x60, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x06, 0x3E, 0x66, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x7E, 0x66, 0x30, 0x18, 0x18, 0x18, 0x18, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x66, 0x7C, 0x60, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0C}},
    {.scanlines = {0x70, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x70, 0x00}},
    {.scanlines = {0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00}},
    {.scanlines = {0x0E, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0E, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x60, 0x30, 0x18, 0x00, 0x18, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x76, 0x76, 0x06, 0x46, 0x3C, 0x00}},
    {.scanlines = {0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}},
    {.scanlines = {0x3E, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3E, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x1E, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1E, 0x00}},
    {.scanlines = {0x7E, 0x06, 0x06, 0x1E, 0x06, 0x06, 0x7E, 0x00}},
    {.scanlines = {0x7E, 0x06, 0x06, 0x1E, 0x06, 0x06, 0x06, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x06, 0x76, 0x66, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}},
    {.scanlines = {0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00}},
    {.scanlines = {0x78, 0x30, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00}},
    {.scanlines = {0x66, 0x36, 0x1E, 0x0E, 0x1E, 0x36, 0x66, 0x00}},
    {.scanlines = {0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7E, 0x00}},
    {.scanlines = {0xC6, 0xEE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0x00}},
    {.scanlines = {0x66, 0x6E, 0x7E, 0x7E, 0x76, 0x66, 0x66, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x3E, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x70, 0x00}},
    {.scanlines = {0x3E, 0x66, 0x66, 0x3E, 0x1E, 0x36, 0x66, 0x00}},
    {.scanlines = {0x3C, 0x66, 0x06, 0x3C, 0x60, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}},
    {.scanlines = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00}},
    {.scanlines = {0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00}},
    {.scanlines = {0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00}},
    {.scanlines = {0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00}},
    {.scanlines = {0x7E, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x7E, 0x00}},
    {.scanlines = {0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00}},
    {.scanlines = {0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x00}},
    {.scanlines = {0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00}},
    {.scanlines = {0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18}},
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}},
    {.scanlines = {0x06, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x3C, 0x60, 0x7C, 0x66, 0x7C, 0x00}},
    {.scanlines = {0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3E, 0x00}},
    {.scanlines = {0x00, 0x00, 0x3C, 0x06, 0x06, 0x06, 0x3C, 0x00}},
    {.scanlines = {0x00, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x7C, 0x00}},
    {.scanlines = {0x00, 0x00, 0x3C, 0x66, 0x7E, 0x06, 0x3C, 0x00}},
    {.scanlines = {0x00, 0x70, 0x18, 0x7C, 0x18, 0x18, 0x18, 0x00}},
    {.scanlines = {0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x3E}},
    {.scanlines = {0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x00}},
    {.scanlines = {0x00, 0x18, 0x00, 0x1C, 0x18, 0x18, 0x3C, 0x00}},
    {.scanlines = {0x00, 0x60, 0x00, 0x60, 0x60, 0x60, 0x60, 0x3C}},
    {.scanlines = {0x00, 0x06, 0x06, 0x36, 0x1E, 0x36, 0x66, 0x00}},
    {.scanlines = {0x00, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00}},
    {.scanlines = {0x00, 0x00, 0x66, 0xFE, 0xFE, 0xD6, 0xC6, 0x00}},
    {.scanlines = {0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x00}},
    {.scanlines = {0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00}},
    {.scanlines = {0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x06}},
    {.scanlines = {0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60}},
    {.scanlines = {0x00, 0x00, 0x3E, 0x66, 0x06, 0x06, 0x06, 0x00}},
    {.scanlines = {0x00, 0x00, 0x7C, 0x06, 0x3C, 0x60, 0x3E, 0x00}},
    {.scanlines = {0x00, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x70, 0x00}},
    {.scanlines = {0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00}},
    {.scanlines = {0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00}},
    {.scanlines = {0x00, 0x00, 0xC6, 0xD6, 0xFE, 0x7C, 0x6C, 0x00}},
    {.scanlines = {0x00, 0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x00}},
    {.scanlines = {0x00, 0x00, 0x66, 0x66, 0x66, 0x7C, 0x30, 0x1E}},
    {.scanlines = {0x00, 0x00, 0x7E, 0x30, 0x18, 0x0C, 0x7E, 0x00}},
    {.scanlines = {0x38, 0x0C, 0x0C, 0x06, 0x0C, 0x0C, 0x38, 0x00}},
    {.scanlines = {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}},
    {.scanlines = {0x1C, 0x30, 0x30, 0x60, 0x30, 0x30, 0x1C, 0x00}},
    {.scanlines = {0x00, 0x4C, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {.scanlines = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
};
*/
