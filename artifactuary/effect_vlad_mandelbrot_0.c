#include "effect_vlad_mandelbrot_0.h"

#include <math.h>


void effect_vlad_mandelbrot_0_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


static rgba_t color_wheel(int32_t wheel_pos);


static inline double distancef(float a, float b, float c, float d)
{
    return sqrtf((a - c) * (a - c) + (b - d) * (b - d));
}


effect_t* effect_vlad_mandelbrot_0_create(void)
{
    effect_t* effect = effect_alloc();
    
    // hack: pack an int randomizer into the state pointer
    effect->void_state = (void*)rand();
    effect->process = &effect_vlad_mandelbrot_0_process;
    
    return effect;
}


void effect_vlad_mandelbrot_0_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    int32_t width = target_array->width;
    int32_t height = target_array->height;
    rgba_t* data = target_array->data;
    float time = (float)((total_time_ns + (int64_t)void_state * 1000000LL) % 100000000000LL) / 1.0e9f;
    
    int32_t ImageHeight = height;
    int32_t ImageWidth = width;
    int32_t MinRe = -2.0;
    int32_t MaxRe = 1.0;
    int32_t MinIm = -1.2;
    int32_t MaxIm = MinIm+(MaxRe-MinRe)*ImageHeight/ImageWidth;
    int32_t Re_factor = (MaxRe-MinRe)/(ImageWidth-1);
    int32_t Im_factor = (MaxIm-MinIm)/(ImageHeight-1);
    int32_t MaxIterations = 30;
    
    for(int32_t y = 0; y < height; y++)
    {
        double c_im = MaxIm - y*Im_factor - time;
        
        for(int32_t x = 0; x < width; x++)
        {
          double c_re = MinRe + x*Re_factor;
  
          double Z_re = c_re, Z_im = c_im;
          bool isInside = true;
          for(unsigned n=0; n<MaxIterations; ++n)
          {
              double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
              if(Z_re2 + Z_im2 > 4)
              {
                  isInside = false;
                  break;
              }
              Z_im = 2*Z_re*Z_im + c_im;
              Z_re = Z_re2 - Z_im2 + c_re;
              
          }
          
         data[y * width + x] = color_wheel((int32_t)Z_im);
        
        }    
    }

}


static rgba_t color_wheel(int32_t wheel_pos)
{
    rgba_t color;
    
    color.c.a = 255;
    
    switch((wheel_pos >> 8) & 0x3)
    {
    case 0:
        color.c.r = 255 - (wheel_pos & 0xFF); // red down
        color.c.g = wheel_pos & 0xFF; // green up
        color.c.b = 0; // blue off
        break;
    case 1:
        color.c.g = 255 - (wheel_pos & 0xFF); // green down
        color.c.b = wheel_pos & 0xFF; // blue up
        color.c.r = 0; // red off
        break;
    case 2:
        color.c.b = 255 - (wheel_pos & 0xFF); // blue down 
        color.c.r = wheel_pos & 0xFF; // red up
        color.c.g = 0; //green off
        break;
    case 3:
        // shouldn't happen but just in case
        color.c.b = 0;
        color.c.r = 255;
        color.c.g = 0;
        break;
    }
    return color;
}


