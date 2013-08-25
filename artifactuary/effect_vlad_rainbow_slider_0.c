#include "effect_vlad_rainbow_slider_0.h"

#include <math.h>


void effect_vlad_rainbow_slider_0_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


static rgba_t color_wheel(int32_t wheel_pos);


static inline double distancef(float a, float b, float c, float d)
{
    return sqrtf((a - c) * (a - c) + (b - d) * (b - d));
}


effect_t* effect_vlad_rainbow_slider_0_create(void)
{
    effect_t* effect = effect_alloc();
    
    // hack: pack an int randomizer into the state pointer
    effect->void_state = (void*)rand();
    effect->process = &effect_vlad_rainbow_slider_0_process;
    
    return effect;
}


void effect_vlad_rainbow_slider_0_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    int32_t width = target_array->width;
    int32_t height = target_array->height;
    rgba_t* data = target_array->data;
    float time = (float)((total_time_ns + (int64_t)void_state * 1000000LL) % 100000000000LL) / 1.0e9f;
    rgba_t blankColor = {{0, 0, 0, 0}};
    
    for(int32_t x = 0; x < width; x++)
    {
        for(int32_t y = 0; y < height; y++)
        {
            float sineValue = sin(0.2*(time+x));
            float calculatedValue = ((2*y)/(10))-1;
            
            // Change of range function (-1 - +1) -> (0 - 31)		        
            float shiftSine = ((31*(sineValue+1))/(2))+0;
            
            if(calculatedValue<sineValue-0.1) { 
                data[y * width + x] = blankColor;
            }
            else {
                // Rolling portion
                data[y * width + x] = color_wheel((int32_t)((y+time)*18)%768);
            }  
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


