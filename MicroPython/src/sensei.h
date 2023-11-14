/*
Melopero Sensei Micropython library. 

Authors: Francesco Marchetti, Luca Davidian



Copyright 2023 Melopero S.r.l. - www.melopero.com

First release March 2023

*/

//include MicroPython API
#include "py/runtime.h"


//extern of class definition
extern const mp_obj_type_t MeloperoSensei_type;

//extern of class methods
extern void MeloperoSensei_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind);
extern  mp_obj_t MeloperoSensei_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args);
extern mp_obj_t MeloperoSensei___del__(mp_obj_t self_in);
extern mp_obj_t MeloperoSensei_write_text(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_set_text_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_set_display_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_clear_screen(size_t n_args, const mp_obj_t *args);
extern mp_obj_t MeloperoSensei_update_display(mp_obj_t self_in);


// extern mp_obj_t MeloperoSensei_set_pixel(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
// extern mp_obj_t MeloperoSensei_set_matrix(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);


// extern mp_obj_t MeloperoSensei_enable_repeated_start(mp_obj_t self_in);
// extern mp_obj_t MeloperoSensei_disable_repeated_start(mp_obj_t self_in);
// extern mp_obj_t MeloperoSensei_stop_message(mp_obj_t self_in);
// extern mp_obj_t MeloperoSensei_set_direction(mp_obj_t self_in, mp_obj_t direction);

// extern mp_obj_t MeloperoSensei_led_init(mp_obj_t self_in);
// extern mp_obj_t MeloperoSensei_led_on(mp_obj_t self_in);
// extern mp_obj_t MeloperoSensei_led_off(mp_obj_t self_in);
// extern mp_obj_t MeloperoSensei_led_toggle(mp_obj_t self_in);

