#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED


#include "artifactuary.h"


// pixel type used by arrays: RGBA 8:8:8:8
typedef union {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } c;
    uint32_t rgba;
} rgba_t;


// pixel array type: this is the basic image and framebuffer format 
typedef struct {
    int32_t width;
    int32_t height;
    rgba_t* data;
} array_t;


// initialize the array subsystem: sets up some internal stuff
void array_init(void);
void array_shutdown(void);

// allocate and initialize an array (image) of the specified dimensions on
// the heap. the initial image data is undefined
void array_alloc(array_t* array, int32_t width, int32_t height);
// destroy and free an array allocated by array_alloc
// note that some arrays are managed manually, particularly the framebuffer arrays
// declared in artifactuary.h. don't free those via this function
void array_free(array_t* array);

// composite two arrays (of the same dimensions) using the alpha in the
// source array. target pixels will be:
//   target_pixel * (1 - source_pixel.alpha) + source_pixel * source_pixel.alpha
void array_composite_source_alpha(array_t* target, array_t* source);

// composite two arrays (of the same dimensions) using the alpha in the
// source array, but selecting pixels from one image or the other instead of
// fading. target pixels will be:
//   source_pixel.alpha >= threshold ? source_pixel : target_pixel
void array_composite_source_alpha_threshold(array_t* target, array_t* source, uint8_t threshold);

// composite two arrays (of the same dimensions) using an alpha supplied as a
// parameter, ignoring alpha in the arrays. target pixels will be:
//   target_pixel * (1 - alpha) + source_pixel * alpha
void array_composite_explicit_alpha(array_t* target, array_t* source, uint8_t alpha);

// composite two arrays (of the same dimensions) by summing. target pixels
// will be:
//   min(target_pixel + source_pixel, rgba(255, 255, 255, 255))
void array_composite_add(array_t* target, array_t* source);


#endif

