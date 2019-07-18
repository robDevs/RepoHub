#ifndef _VITA_KEYBOARD_
#define _VITA_KEYBOARD_

#include "includes.h"

#define IME_DIALOG_RESULT_NONE 0
#define IME_DIALOG_RESULT_RUNNING 1
#define IME_DIALOG_RESULT_FINISHED 2
#define IME_DIALOG_RESULT_CANCELED 3

extern int ime_dialog_running;
extern int ime_dialog_option;
extern int ime_init_apputils;

extern uint16_t ime_title_utf16[SCE_IME_DIALOG_MAX_TITLE_LENGTH];
extern uint16_t ime_initial_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
extern uint16_t ime_input_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
extern uint8_t ime_input_text_utf8[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];

extern void utf16_to_utf8(uint16_t *src, uint8_t *dst);

extern void utf8_to_utf16(uint8_t *src, uint16_t *dst);

extern int init_ime_dialog(char *title, const char *initial_text, int max_text_length, int type, int option);

extern uint8_t *get_ime_dialog_input_text_utf8(void);

extern int update_ime_dialog(void);

extern char *vita_keyboard_get(char *title, const char *initial_text, int max_len, int multiline);

#endif
