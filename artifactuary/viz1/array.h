#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED


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


#endif
