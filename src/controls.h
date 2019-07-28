#ifndef _CONTROLS_
#define _CONTROLS_

#include "includes.h"

extern bool cross_pressed;
extern bool circle_pressed;
extern bool triangle_pressed;
extern bool square_pressed;
extern bool down_pressed;
extern bool right_pressed;
extern bool up_pressed;
extern bool left_pressed;
extern bool lt_pressed;
extern bool rt_pressed;
extern bool start_pressed;
extern bool select_pressed;

extern bool cross_released;
extern bool circle_released;
extern bool triangle_released;
extern bool square_released;
extern bool down_released;
extern bool right_released;
extern bool up_released;
extern bool left_released;
extern bool lt_released;
extern bool rt_released;
extern bool start_released;
extern bool select_released;

extern bool cross_held;
extern bool circle_held;
extern bool triangle_held;
extern bool square_held;
extern bool down_held;
extern bool right_held;
extern bool up_held;
extern bool left_held;
extern bool lt_held;
extern bool rt_held;
extern bool start_held;
extern bool select_held;

extern SceCtrlData pad;
extern void init_keys();
extern void update_keys();
extern void update_single_key(SceCtrlButtons button, bool *pressed, bool *held, bool *released);
extern void reset_keys();

extern int lx;
extern int ly;
extern int rx;
extern int ry;

extern void read_joy_sticks();

#endif
