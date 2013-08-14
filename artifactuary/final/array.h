#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED


#include "artifactuary.h"


typedef union {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } c;
    uint32_t rgba;
} rgba_t;


typedef struct {
    int32_t width;
    int32_t height;
    rgba_t* data;
} array_t;


void array_init(void);

void array_alloc(array_t* array, int32_t width, int32_t height);
void array_free(array_t* array);

//inline void array_set_pixel(array_t* array, int32_t x, int32_t y, rgba_t color)
//{
//	array->data[y * array->width + x] = color;
//}

void array_composite_source_alpha(array_t* target, array_t* source);
void array_composite_source_alpha_threshold(array_t* target, array_t* source, uint8_t threshold);
void array_composite_explicit_alpha(array_t* target, array_t* source, uint8_t alpha);
void array_composite_add(array_t* target, array_t* source);


#endif

