#include "controls.h"

SceCtrlData pad;

bool cross_pressed = false;
bool circle_pressed = false;
bool triangle_pressed = false;
bool square_pressed = false;
bool down_pressed = false;
bool right_pressed = false;
bool up_pressed = false;
bool left_pressed = false;
bool lt_pressed = false;
bool rt_pressed = false;
bool start_pressed = false;
bool select_pressed = false;

bool cross_released = false;
bool circle_released = false;
bool triangle_released = false;
bool square_released = false;
bool down_released = false;
bool right_released = false;
bool up_released = false;
bool left_released = false;
bool lt_released = false;
bool rt_released = false;
bool start_released = false;
bool select_released = false;

bool cross_held = false;
bool circle_held = false;
bool triangle_held = false;
bool square_held = false;
bool down_held = false;
bool right_held = false;
bool up_held = false;
bool left_held = false;
bool lt_held = false;
bool rt_held = false;
bool start_held = false;
bool select_held = false;

int lx;
int ly;
int rx;
int ry;

void init_keys() {
    memset(&pad, 0, sizeof(pad));
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
}
void update_keys() {
    sceCtrlPeekBufferPositive(0, &pad, 1);

    update_single_key(SCE_CTRL_CROSS, &cross_pressed, &cross_held, &cross_released);
    update_single_key(SCE_CTRL_CIRCLE, &circle_pressed, &circle_held, &circle_released);
    update_single_key(SCE_CTRL_TRIANGLE, &triangle_pressed, &triangle_held, &triangle_released);
    update_single_key(SCE_CTRL_SQUARE, &square_pressed, &square_held, &square_released);
    update_single_key(SCE_CTRL_DOWN, &down_pressed, &down_held, &down_released);
    update_single_key(SCE_CTRL_RIGHT, &right_pressed, &right_held, &right_released);
    update_single_key(SCE_CTRL_UP, &up_pressed, &up_held, &up_released);
    update_single_key(SCE_CTRL_LEFT, &left_pressed, &left_held, &left_released);
    update_single_key(SCE_CTRL_LTRIGGER, &lt_pressed, &lt_held, &lt_released);
    update_single_key(SCE_CTRL_RTRIGGER, &triangle_pressed, &rt_held, &rt_released);
    update_single_key(SCE_CTRL_START, &start_pressed, &start_held, &start_released);
    update_single_key(SCE_CTRL_SELECT, &select_pressed, &select_held, &select_released);
}

void update_single_key(SceCtrlButtons button, bool *pressed, bool *held, bool *released) {
    *released = false;
    if(pad.buttons & button && *pressed) {
        *held = true;
        *pressed = false;
    }

    if(pad.buttons & button && !*held) {
        *pressed = true;
    }

    if(!(pad.buttons & button) && *pressed) {
        *released = true;
    }
    if(!(pad.buttons & button) && *held) {
        *released = true;
    }
    if(!(pad.buttons & button)) {
        *pressed = false;
        *held = false;
    }
}

void reset_keys() {
    cross_pressed = false;
    circle_pressed = false;
    triangle_pressed = false;
    square_pressed = false;
    down_pressed = false;
    right_pressed = false;
    up_pressed = false;
    left_pressed = false;
    lt_pressed = false;
    rt_pressed = false;
    start_pressed = false;
    select_pressed = false;

    cross_released = false;
    circle_released = false;
    triangle_released = false;
    square_released = false;
    down_released = false;
    right_released = false;
    up_released = false;
    left_released = false;
    lt_released = false;
    rt_released = false;
    start_released = false;
    select_released = false;

    cross_held = false;
    circle_held = false;
    triangle_held = false;
    square_held = false;
    down_held = false;
    right_held = false;
    up_held = false;
    left_held = false;
    lt_held = false;
    rt_held = false;
    start_held = false;
    select_held = false;
}

extern void read_joy_sticks() {
    SceCtrlData pad;

    sceCtrlPeekBufferPositive(0, &pad, 1);

    lx = pad.lx;
    ly = pad.ly;
    rx = pad.rx;
    ry = pad.ry;
}
