/*
Melopero Sensei Micropython library. 

Authors: Luca Davidian, Leonardo La Rocca, Francesco Marchetti

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release November 2023

*/

//include MicroPython API
#include "py/runtime.h"


enum text_size
{
    TEXT_VERY_SMALL=0, 
    TEXT_SMALL, 
    TEXT_REGULAR, 
    TEXT_BIG,
};

enum text_font
{
    TEXT_MONO=0, 
    TEXT_MONO_BOLD, 
    TEXT_SANS, 
    TEXT_SANS_BOLD,
};

//extern of class definition
extern const mp_obj_type_t MeloperoSensei_type;

//extern of class methods
extern void MeloperoSensei_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind);
extern  mp_obj_t MeloperoSensei_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args);
extern mp_obj_t MeloperoSensei___del__(mp_obj_t self_in);
extern mp_obj_t MeloperoSensei_write_text(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_draw_rect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_draw_fill_rect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_draw_line(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_draw_pixel(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_draw_sprite(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_set_text_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_set_text_font(mp_obj_t self_in, mp_obj_t font, mp_obj_t size);
extern mp_obj_t MeloperoSensei_set_display_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t MeloperoSensei_clear_screen(size_t n_args, const mp_obj_t *args);
extern mp_obj_t MeloperoSensei_update_display(mp_obj_t self_in);
extern mp_obj_t MeloperoSensei_get_cpu_temp(mp_obj_t self_in);

