#include "effect_vlad_fire_0.h"


typedef struct effect_vlad_fire_0_state
{
    int32_t width;
    int32_t height;
    uint8_t* intensity;
} effect_vlad_fire_0_state_t;


void effect_vlad_fire_0_destroy(void* void_state);
void effect_vlad_fire_0_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns);


rgba_t effect_vlad_fire_0_palette[256];


effect_t* effect_vlad_fire_0_create(int32_t width, int32_t height)
{
    effect_t* effect = effect_alloc();
    effect_vlad_fire_0_state_t* state = (effect_vlad_fire_0_state_t*)malloc(sizeof (effect_vlad_fire_0_state_t));
    
    effect->void_state = state;
    effect->process = &effect_vlad_fire_0_process;
    effect->destroy = &effect_vlad_fire_0_destroy;
    
    state->width = width;
    state->height = height;
    state->intensity = (uint8_t*)malloc((width + 2) * (height + 2) * sizeof (uint8_t));
    
    memset(state->intensity, 0, (width + 2) * (height + 2) * sizeof (uint8_t));
    
    return effect;
}


void effect_vlad_fire_0_destroy(void* void_state)
{
    effect_vlad_fire_0_state_t* state = (effect_vlad_fire_0_state_t*)void_state;
    
    free(state->intensity);
    free(state);
}


void effect_vlad_fire_0_process(void* void_state, array_t* target_array, int64_t total_time_ns, int64_t frame_time_ns)
{
    int32_t width = target_array->width;
    int32_t height = target_array->height;
    int32_t intensity_width = width + 2;
    int32_t intensity_height = height + 2;
    rgba_t* data = target_array->data;
    effect_vlad_fire_0_state_t* state = (effect_vlad_fire_0_state_t*)void_state;
    uint8_t* intensity = state->intensity;
    
    assert(width == state->width);
    assert(height == state->height);
    
    // put random data (embers) in the last and second-last rows
    for(int x = 0; x < width + 2; x++)
    {
        int32_t randomOff = rand() % 3;
        
        if(randomOff == 0)
        {
            intensity[(intensity_height - 2) * intensity_width + x] = 64;
            intensity[(intensity_height - 1) * intensity_width + x] = 64;
        }
        else {
            intensity[(intensity_height - 2) * intensity_width + x] = rand() % 256;
            intensity[(intensity_height - 1) * intensity_width + x] = rand() % 224 + 32;
        }
    }
    
    for(int y = 0; y < intensity_height - 2; y++)
    {
        for(int x = 1; x < intensity_width - 1; x++) 
        {
            int32_t flameDampen = rand() % 256 + 128;
            int32_t flameValue =
                (int32_t)intensity[(y + 0) * intensity_width + x + 0] +
                (int32_t)intensity[(y + 1) * intensity_width + x - 1] +
                (int32_t)intensity[(y + 1) * intensity_width + x + 0] +
                (int32_t)intensity[(y + 1) * intensity_width + x + 1] +
                (int32_t)intensity[(y + 2) * intensity_width + x + 0];
            if(flameDampen > flameValue)
            {
                flameValue = 0;
            }
            intensity[(y + 0) * intensity_width + x + 0] = (uint8_t)(flameValue / 5);
        }
    }

    for(int x = 0; x < width; x++) 
    {
        for(int y = 0; y < height; y++)
        {
            data[y * width + x] = effect_vlad_fire_0_palette[intensity[y * intensity_width + x + 1]];
        }
    }
}


rgba_t effect_vlad_fire_0_palette[256] = {
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  0,   0,   0, 255}},
    {{  4,   0,   0, 255}},
    {{  8,   1,   0, 255}},
    {{ 12,   2,   0, 255}},
    {{ 16,   3,   0, 255}},
    {{ 20,   4,   0, 255}},
    {{ 24,   4,   0, 255}},
    {{ 28,   5,   0, 255}},
    {{ 32,   6,   0, 255}},
    {{ 36,   7,   0, 255}},
    {{ 40,   8,   0, 255}},
    {{ 44,   8,   0, 255}},
    {{ 48,   9,   0, 255}},
    {{ 52,  10,   0, 255}},
    {{ 56,  11,   0, 255}},
    {{ 60,  12,   0, 255}},
    {{ 64,  12,   0, 255}},
    {{ 68,  13,   0, 255}},
    {{ 72,  14,   0, 255}},
    {{ 76,  15,   0, 255}},
    {{ 80,  16,   0, 255}},
    {{ 84,  16,   0, 255}},
    {{ 88,  17,   0, 255}},
    {{ 92,  18,   0, 255}},
    {{ 96,  19,   0, 255}},
    {{100,  20,   0, 255}},
    {{104,  20,   0, 255}},
    {{108,  21,   0, 255}},
    {{112,  22,   0, 255}},
    {{116,  23,   0, 255}},
    {{120,  24,   0, 255}},
    {{124,  24,   0, 255}},
    {{128,  25,   0, 255}},
    {{131,  28,   0, 255}},
    {{134,  30,   1, 255}},
    {{137,  32,   2, 255}},
    {{140,  35,   3, 255}},
    {{144,  37,   4, 255}},
    {{147,  40,   4, 255}},
    {{150,  42,   5, 255}},
    {{153,  44,   6, 255}},
    {{156,  47,   7, 255}},
    {{160,  49,   8, 255}},
    {{163,  52,   8, 255}},
    {{166,  54,   9, 255}},
    {{169,  56,  10, 255}},
    {{172,  59,  11, 255}},
    {{176,  61,  12, 255}},
    {{179,  64,  12, 255}},
    {{182,  66,  13, 255}},
    {{185,  68,  14, 255}},
    {{188,  71,  15, 255}},
    {{192,  73,  16, 255}},
    {{195,  76,  16, 255}},
    {{198,  78,  17, 255}},
    {{201,  80,  18, 255}},
    {{204,  83,  19, 255}},
    {{208,  85,  20, 255}},
    {{211,  88,  20, 255}},
    {{214,  90,  21, 255}},
    {{217,  92,  22, 255}},
    {{220,  95,  23, 255}},
    {{224,  97,  24, 255}},
    {{227, 100,  24, 255}},
    {{230, 102,  25, 255}},
    {{230, 105,  26, 255}},
    {{231, 108,  27, 255}},
    {{231, 112,  28, 255}},
    {{232, 115,  28, 255}},
    {{232, 118,  29, 255}},
    {{232, 121,  30, 255}},
    {{233, 124,  31, 255}},
    {{233, 128,  32, 255}},
    {{234, 131,  32, 255}},
    {{234, 134,  33, 255}},
    {{234, 137,  34, 255}},
    {{235, 140,  35, 255}},
    {{235, 144,  36, 255}},
    {{236, 147,  36, 255}},
    {{236, 150,  37, 255}},
    {{236, 153,  38, 255}},
    {{237, 156,  39, 255}},
    {{237, 160,  40, 255}},
    {{238, 163,  40, 255}},
    {{238, 166,  41, 255}},
    {{238, 169,  42, 255}},
    {{239, 172,  43, 255}},
    {{239, 176,  44, 255}},
    {{239, 179,  44, 255}},
    {{240, 182,  45, 255}},
    {{240, 185,  46, 255}},
    {{241, 188,  47, 255}},
    {{241, 192,  48, 255}},
    {{241, 195,  48, 255}},
    {{242, 198,  49, 255}},
    {{242, 201,  50, 255}},
    {{243, 204,  51, 255}},
    {{243, 206,  57, 255}},
    {{244, 208,  64, 255}},
    {{244, 209,  70, 255}},
    {{244, 211,  76, 255}},
    {{245, 212,  83, 255}},
    {{245, 214,  89, 255}},
    {{246, 216,  96, 255}},
    {{246, 217, 102, 255}},
    {{246, 219, 108, 255}},
    {{247, 220, 115, 255}},
    {{247, 222, 121, 255}},
    {{248, 224, 128, 255}},
    {{248, 225, 134, 255}},
    {{248, 227, 140, 255}},
    {{249, 228, 147, 255}},
    {{249, 230, 153, 255}},
    {{250, 232, 160, 255}},
    {{250, 233, 166, 255}},
    {{250, 235, 172, 255}},
    {{251, 236, 179, 255}},
    {{251, 238, 185, 255}},
    {{252, 240, 192, 255}},
    {{252, 241, 198, 255}},
    {{252, 243, 204, 255}},
    {{253, 244, 211, 255}},
    {{253, 246, 217, 255}},
    {{254, 248, 224, 255}},
    {{254, 249, 230, 255}},
    {{254, 251, 236, 255}},
    {{255, 252, 243, 255}},
    {{255, 254, 249, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
    {{255, 255, 255, 255}},
};
