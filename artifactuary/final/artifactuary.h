#ifndef ARTIFACTUARY_H_INCLUDED
#define ARTIFACTUARY_H_INCLUDED


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define ARRAY_STRIDE 128
#define ARRAY_WIDTH 110
#define ARRAY_HEIGHT 18


typedef union {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } c;
    uint32_t rgba;
} rgba_t;


extern rgba_t array_data[ARRAY_HEIGHT][ARRAY_STRIDE];


void artifactuary_init(void);
void artifactuary_process(float time);


#endif
