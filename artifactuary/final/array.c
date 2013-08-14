#include "array.h"


uint8_t array_alpha_table[256][256];
uint8_t array_one_minus_alpha_table[256][256];


void array_init(void)
{
    for(int32_t j = 0; j < 256; ++j) {
        for(int32_t i = 0; i < 256; ++i) {
            array_alpha_table[j][i] = (j * i + 127) / 255;
            array_one_minus_alpha_table[j][i] = ((255 - j) * i + 128) / 255;
        }
    }
}


void array_alloc(array_t* array, int32_t width, int32_t height)
{
    assert(width > 0);
    assert(height > 0);
    
    array->data = (rgba_t*)malloc(width * height * sizeof (rgba_t));
    assert(array->data != NULL);
    array->width = width;
    array->height = height;
}


void array_free(array_t* array)
{
    assert(array->data != NULL);
    assert(array->width > 0);
    assert(array->height > 0);
    
    free(array->data);
    array->data = NULL;
    array->width = -1;
    array->height = -1;
}


void array_composite_source_alpha(array_t* target, array_t* source)
{
    int32_t size = target->width * target->height;
    
    assert(target->width == source->width);
    assert(target->height == source->height);
    
    for(int32_t i = 0; i < size; ++i) {
        uint8_t* one_minus_alpha_table = array_one_minus_alpha_table[source->data[i].c.a];
        uint8_t* alpha_table = array_alpha_table[source->data[i].c.a]
        
        target->data[i].c.r = one_minus_alpha_table[target->data[i].c.r] + alpha_table[source_data[i].c.r];
        target->data[i].c.g = one_minus_alpha_table[target->data[i].c.g] + alpha_table[source_data[i].c.g];
        target->data[i].c.b = one_minus_alpha_table[target->data[i].c.b] + alpha_table[source_data[i].c.b];
        target->data[i].c.a = 255;
    }
}


void array_composite_source_alpha_threshold(array_t* target, array_t* source, uint8_t threshold)
{
    int32_t size = target->width * target->height;
    
    assert(target->width == source->width);
    assert(target->height == source->height);
    
    for(int32_t i = 0; i < size; ++i) {
        if(source->data[i].c.a >= threshold) {
            target->data[i] = source->data[i];
        }
    }
}


void array_composite_explicit_alpha(array_t* target, array_t* source, uint8_t alpha)
{
    uint8_t* one_minus_alpha_table = array_one_minus_alpha_table[alpha];
    uint8_t* alpha_table = array_alpha_table[alpha]
    
    int32_t size = target->width * target->height;
    
    assert(target->width == source->width);
    assert(target->height == source->height);
    
    for(int32_t i = 0; i < size; ++i) {
        target->data[i].c.r = one_minus_alpha_table[target->data[i].c.r] + alpha_table[source_data[i].c.r];
        target->data[i].c.g = one_minus_alpha_table[target->data[i].c.g] + alpha_table[source_data[i].c.g];
        target->data[i].c.b = one_minus_alpha_table[target->data[i].c.b] + alpha_table[source_data[i].c.b];
        target->data[i].c.a = 255;
    }
}


void array_composite_add(array_t* target, array_t* source)
{
    int32_t size = target->width * target->height;
    
    assert(target->width == source->width);
    assert(target->height == source->height);
    
    for(int32_t i = 0; i < size; ++i) {
        int32_t sum;
        
        sum = (int32_t)target->data[i].c.r + source->data[i].c.r;
        target.c.r = sum > 255 ? 255 : (uint8_t)sum;
        sum = (int32_t)target->data[i].c.g + source->data[i].c.g;
        target.c.g = sum > 255 ? 255 : (uint8_t)sum;
        sum = (int32_t)target->data[i].c.b + source->data[i].c.b;
        target.c.b = sum > 255 ? 255 : (uint8_t)sum;
    }
}


