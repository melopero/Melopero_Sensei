/*
Melopero Sensei Micropython library. 

Authors: Luca Davidian, Leonardo La Rocca, Francesco Marchetti

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release November 2023

*/


#include "sensei.h"

//Methods


MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei___del___obj, MeloperoSensei___del__);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_set_text_color_obj, 1, MeloperoSensei_set_text_color);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_set_display_color_obj, 1, MeloperoSensei_set_display_color);
MP_DEFINE_CONST_FUN_OBJ_VAR(MeloperoSensei_clear_display_obj, 1, MeloperoSensei_clear_display);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_write_text_obj, 1, MeloperoSensei_write_text);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_update_display_obj, MeloperoSensei_update_display);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_rect_obj, 1, MeloperoSensei_draw_rect);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_fill_rect_obj, 1, MeloperoSensei_draw_fill_rect);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_line_obj, 1, MeloperoSensei_draw_line);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_pixel_obj, 1, MeloperoSensei_draw_pixel);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_sprite_obj, 1, MeloperoSensei_draw_sprite);
MP_DEFINE_CONST_FUN_OBJ_3(MeloperoSensei_set_text_font_obj, MeloperoSensei_set_text_font);
MP_DEFINE_CONST_FUN_OBJ_2(MeloperoSensei_is_button_pressed_obj, MeloperoSensei_is_button_pressed);
MP_DEFINE_CONST_FUN_OBJ_2(MeloperoSensei_get_button_irq_state_obj, MeloperoSensei_get_button_irq_state);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_get_cpu_temp_obj, MeloperoSensei_get_cpu_temp);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_get_interrupt_register_obj, MeloperoSensei_get_interrupt_register);
MP_DEFINE_CONST_FUN_OBJ_2(MeloperoSensei_enable_button_interrupt_obj, MeloperoSensei_enable_button_interrupt);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_play_note_obj, 1, MeloperoSensei_play_note);
MP_DEFINE_CONST_FUN_OBJ_2(MeloperoSensei_get_button_state_obj, MeloperoSensei_get_button_state);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_read_battery_obj, MeloperoSensei_read_battery);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_read_light_obj, MeloperoSensei_read_light);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_set_light_min_obj, MeloperoSensei_set_light_min);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_set_light_max_obj, MeloperoSensei_set_light_max);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_touch_init_obj, MeloperoSensei_touch_init);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_get_touch_obj, MeloperoSensei_get_touch);

//Bindings of methods

STATIC const mp_rom_map_elem_t MeloperoSensei_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write_text), MP_ROM_PTR(&MeloperoSensei_write_text_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&MeloperoSensei___del___obj) },
    { MP_ROM_QSTR(MP_QSTR_update_display), MP_ROM_PTR(&MeloperoSensei_update_display_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_cpu_temp), MP_ROM_PTR(&MeloperoSensei_get_cpu_temp_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear_display), MP_ROM_PTR(&MeloperoSensei_clear_display_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_text_color), MP_ROM_PTR(&MeloperoSensei_set_text_color_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_text_font), MP_ROM_PTR(&MeloperoSensei_set_text_font_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_display_color), MP_ROM_PTR(&MeloperoSensei_set_display_color_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_rect), MP_ROM_PTR(&MeloperoSensei_draw_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_fill_rect), MP_ROM_PTR(&MeloperoSensei_draw_fill_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_line), MP_ROM_PTR(&MeloperoSensei_draw_line_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_pixel), MP_ROM_PTR(&MeloperoSensei_draw_pixel_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_sprite), MP_ROM_PTR(&MeloperoSensei_draw_sprite_obj) },
    { MP_ROM_QSTR(MP_QSTR_is_button_pressed), MP_ROM_PTR(&MeloperoSensei_is_button_pressed_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_button_irq_state), MP_ROM_PTR(&MeloperoSensei_get_button_irq_state_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_interrupt_register), MP_ROM_PTR(&MeloperoSensei_get_interrupt_register_obj) },
    { MP_ROM_QSTR(MP_QSTR_enable_button_interrupt), MP_ROM_PTR(&MeloperoSensei_enable_button_interrupt_obj) },
    { MP_ROM_QSTR(MP_QSTR_play_note), MP_ROM_PTR(&MeloperoSensei_play_note_obj) },
    { MP_ROM_QSTR(MP_QSTR_enable_get_button_state), MP_ROM_PTR(&MeloperoSensei_get_button_state_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_battery), MP_ROM_PTR(&MeloperoSensei_read_battery_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_light), MP_ROM_PTR(&MeloperoSensei_read_light_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_light_min), MP_ROM_PTR(&MeloperoSensei_set_light_min_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_light_max), MP_ROM_PTR(&MeloperoSensei_set_light_max_obj) },
    { MP_ROM_QSTR(MP_QSTR_touch_init), MP_ROM_PTR(&MeloperoSensei_touch_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_touch), MP_ROM_PTR(&MeloperoSensei_get_touch_obj) },

};
STATIC MP_DEFINE_CONST_DICT(MeloperoSensei_locals_dict, MeloperoSensei_locals_dict_table);

/***** Class Definition *****/
#ifdef MP_DEFINE_CONST_OBJ_TYPE

MP_DEFINE_CONST_OBJ_TYPE(
    MeloperoSensei_type,
    MP_QSTR_MeloperoSensei,
    MP_TYPE_FLAG_NONE,
    make_new, MeloperoSensei_make_new,
    print, MeloperoSensei_print,
    locals_dict, (mp_obj_dict_t*)&MeloperoSensei_locals_dict
);
#else

const mp_obj_type_t MeloperoSensei_type = {
    { &mp_type_type },
    .name = MP_QSTR_MeloperoSensei,
    .print = MeloperoSensei_print,
    .make_new = MeloperoSensei_make_new,
    .locals_dict = (mp_obj_dict_t*)&MeloperoSensei_locals_dict,
};
#endif

/***** Globals Table *****/
STATIC const mp_map_elem_t melopero_sensei_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_melopero_sensei) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_MeloperoSensei), (mp_obj_t)&MeloperoSensei_type },

    //text font
    { MP_ROM_QSTR(MP_QSTR_TEXT_VERY_SMALL), MP_ROM_INT(TEXT_VERY_SMALL) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_SMALL), MP_ROM_INT(TEXT_SMALL) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_REGULAR), MP_ROM_INT(TEXT_REGULAR) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_BIG), MP_ROM_INT(TEXT_BIG) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_MONO), MP_ROM_INT(TEXT_MONO) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_MONO_BOLD), MP_ROM_INT(TEXT_MONO_BOLD) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_SANS), MP_ROM_INT(TEXT_SANS) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_SANS_BOLD), MP_ROM_INT(TEXT_SANS_BOLD) },

    //input
    { MP_ROM_QSTR(MP_QSTR_JOYSTICK_UP), MP_ROM_INT(JOYSTICK_UP) },
    { MP_ROM_QSTR(MP_QSTR_JOYSTICK_DOWN), MP_ROM_INT(JOYSTICK_DOWN) },
    { MP_ROM_QSTR(MP_QSTR_JOYSTICK_LEFT), MP_ROM_INT(JOYSTICK_LEFT) },
    { MP_ROM_QSTR(MP_QSTR_JOYSTICK_RIGHT), MP_ROM_INT(JOYSTICK_RIGHT) },
    { MP_ROM_QSTR(MP_QSTR_JOYSTICK_CENTER), MP_ROM_INT(JOYSTICK_CENTER) },
    { MP_ROM_QSTR(MP_QSTR_BUTTON_A), MP_ROM_INT(BUTTON_A) },
    { MP_ROM_QSTR(MP_QSTR_BUTTON_B), MP_ROM_INT(BUTTON_B) },
   

};
STATIC MP_DEFINE_CONST_DICT(mp_module_melopero_sensei_globals, melopero_sensei_globals_table);

/***** Module Definition *****/
const mp_obj_module_t melopero_sensei_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_melopero_sensei_globals,
};

#if MICROPY_VERSION <= 70144
MP_REGISTER_MODULE(MP_QSTR_melopero_sensei, melopero_sensei_user_cmodule, MODULE_MeloperoSensei_ENABLED);
#else
MP_REGISTER_MODULE(MP_QSTR_melopero_sensei, melopero_sensei_user_cmodule);
#endif
