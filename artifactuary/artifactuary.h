#ifndef ARTIFACTUARY_H_INCLUDED
#define ARTIFACTUARY_H_INCLUDED


#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// these defines map out number and dimensions of the light panels on the
// sculpture

/*
#define ARTIFACTUARY_NUM_ARRAYS 5

// this is the little wide panel over the buildings
#define ARTIFACTUARY_BACKDROP 0
// this is the leftmost (mid-height) building
#define ARTIFACTUARY_BUILDING_B 1
// this is the off-height panel on the leftmost building
#define ARTIFACTUARY_BUILDING_B2 2
// this is the middle (shortest) building
#define ARTIFACTUARY_BUILDING_C 3
// this is the rightmost (tallest) building
#define ARTIFACTUARY_BUILDING_A 4

// change resolutions of the panels here -- mind that you must adhere to
// certain constraints when setting the sizes of the panels:
//  - the backdrop panel must have an even number of rows
//  - all building panels must have odd numbers of rows
#define ARTIFACTUARY_BACKDROP_WIDTH 30
#define ARTIFACTUARY_BACKDROP_HEIGHT 4

#define ARTIFACTUARY_BUILDING_B_WIDTH 24
#define ARTIFACTUARY_BUILDING_B_HEIGHT 20

#define ARTIFACTUARY_BUILDING_B2_WIDTH 10
#define ARTIFACTUARY_BUILDING_B2_HEIGHT 1

#define ARTIFACTUARY_BUILDING_C_WIDTH 30
#define ARTIFACTUARY_BUILDING_C_HEIGHT 20

#define ARTIFACTUARY_BUILDING_A_WIDTH 30
#define ARTIFACTUARY_BUILDING_A_HEIGHT 20

#define ARTIFACTUARY_NUM_PIXELS \
    (ARTIFACTUARY_BACKDROP_WIDTH * ARTIFACTUARY_BACKDROP_HEIGHT + \
     ARTIFACTUARY_BUILDING_B_WIDTH * ARTIFACTUARY_BUILDING_B_HEIGHT + \
     ARTIFACTUARY_BUILDING_B2_WIDTH * ARTIFACTUARY_BUILDING_B2_HEIGHT + \
     ARTIFACTUARY_BUILDING_C_WIDTH * ARTIFACTUARY_BUILDING_C_HEIGHT + \
     ARTIFACTUARY_BUILDING_A_WIDTH * ARTIFACTUARY_BUILDING_A_HEIGHT)
*/

#define ARTIFACTUARY_NUM_ARRAYS 3

// this is the leftmost array
#define ARTIFACTUARY_BUILDING_A 0
// this is the middle (offwidth) array
#define ARTIFACTUARY_BUILDING_B 1
// this is the rightmost array
#define ARTIFACTUARY_BUILDING_C 2

// change resolutions of the panels here
#define ARTIFACTUARY_BUILDING_A_WIDTH 30
#define ARTIFACTUARY_BUILDING_A_HEIGHT 20

#define ARTIFACTUARY_BUILDING_B_WIDTH 24
#define ARTIFACTUARY_BUILDING_B_HEIGHT 20

#define ARTIFACTUARY_BUILDING_C_WIDTH 30
#define ARTIFACTUARY_BUILDING_C_HEIGHT 20


#define ARTIFACTUARY_NUM_PIXELS \
    (ARTIFACTUARY_BUILDING_A_WIDTH * ARTIFACTUARY_BUILDING_A_HEIGHT + \
     ARTIFACTUARY_BUILDING_B_WIDTH * ARTIFACTUARY_BUILDING_B_HEIGHT + \
     ARTIFACTUARY_BUILDING_C_WIDTH * ARTIFACTUARY_BUILDING_C_HEIGHT)


#include "array.h"
#include "effect.h"


// the framebuffers for the various sculpture panels
extern array_t artifactuary_arrays[ARTIFACTUARY_NUM_ARRAYS];

// the data used by the panels' framebuffers, all concatenated together
extern rgba_t artifactuary_array_data[ARTIFACTUARY_NUM_PIXELS];

// this table contains the mapping between the electrical position of the
// lights and the
extern int32_t artifactuary_array_data_mapping[ARTIFACTUARY_NUM_PIXELS];

/*
extern float artifactuary_time_of_day;
extern int32_t artifactuary_day;
extern int32_t artifactuary_time_of_day_hour;
extern int32_t artifactuary_time_of_day_minute;
extern int32_t artifactuary_time_of_day_second;
*/


void artifactuary_init(void);
void artifactuary_process(int64_t total_time_ns, int64_t frame_time_ns);


#endif
