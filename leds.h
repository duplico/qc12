/*
 * leds.h
 * (c) 2014 George Louthan
 * 3-clause BSD license; see license.md.
 */

#ifndef LEDS_H_
#define LEDS_H_

typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} rgbcolor_t;

typedef struct {
    int_fast32_t red;
    int_fast32_t green;
    int_fast32_t blue;
} rgbdelta_t;

typedef struct {
    const rgbcolor_t * colors;
    uint8_t len;
    char anim_name[12];
} tlc_animation_t;

extern rgbcolor_t rainbow2[];

extern uint8_t tlc_anim_mode;

extern const tlc_animation_t flag_rainbow;
extern const tlc_animation_t flag_bi;
extern const tlc_animation_t flag_pan;
extern const tlc_animation_t flag_trans;
extern const tlc_animation_t flag_ace;
extern const tlc_animation_t flag_ally;
extern const tlc_animation_t flag_leather;
extern const tlc_animation_t flag_bear;
extern const tlc_animation_t flag_blue;
extern const tlc_animation_t flag_lblue;
extern const tlc_animation_t flag_green;
extern const tlc_animation_t flag_red;
extern const tlc_animation_t flag_yellow;
extern const tlc_animation_t flag_pink;
#define FLAG_COUNT 14
extern const tlc_animation_t *flags[FLAG_COUNT];

extern const tlc_animation_t flag_ambient;

extern uint8_t tlc_is_ambient;

void init_tlc();

uint8_t tlc_test_loopback(uint8_t);

void tlc_set_gs();
void tlc_set_fun();
void tlc_stage_blank(uint8_t);
void tlc_start_anim(const tlc_animation_t *, uint8_t, uint8_t, uint8_t, uint8_t);
void tlc_stop_anim(uint8_t blank);
void tlc_set_ambient(uint8_t);
void tlc_display_ambient();

void tlc_timestep();

#endif /* LEDS_H_ */
