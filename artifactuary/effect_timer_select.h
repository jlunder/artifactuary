#ifndef EFFECT_TIMER_SELECT_H_INCLUDED
#define EFFECT_TIMER_SELECT_H_INCLUDED


#include "artifactuary.h"


effect_t* effect_timer_select_create();

bool effect_timer_select_get_shuffle(effect_t* effect);
void effect_timer_select_set_shuffle(effect_t* effect, bool shuffle);

int32_t effect_timer_select_add_subeffect(effect_t* effect, effect_t* subeffect, char const* subeffect_name);
char const* effect_timer_select_get_subeffect_name(effect_t* effect, int32_t subeffect_id);
int32_t effect_timer_select_get_current_subeffect(effect_t* effect);

void effect_timer_select_next_subeffect(effect_t* effect);
void effect_timer_select_previous_subeffect(effect_t* effect);
void effect_timer_select_goto_subeffect(effect_t* effect, int32_t subeffect_id);

int32_t effect_timer_select_get_advance_ms(effect_t* effect);
void effect_timer_select_set_advance_ms(effect_t* effect, int32_t advance_ms); // 0 == stop
int32_t effect_timer_select_get_crossfade_ms(effect_t* effect);
void effect_timer_select_set_crossfade_ms(effect_t* effect, int32_t crossfade_ms);


#endif
