/*
Melopero Sensei Micropython library. 

Authors: Francesco Marchetti, Luca Davidian



Copyright 2023 Melopero S.r.l. - www.melopero.com

First release October 2023

*/

#include "sensei.h"

//Methods


MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei___del___obj, MeloperoSensei___del__);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_set_text_color_obj, 1, MeloperoSensei_set_text_color);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_set_display_color_obj, 1, MeloperoSensei_set_display_color);
MP_DEFINE_CONST_FUN_OBJ_VAR(MeloperoSensei_clear_screen_obj, 1, MeloperoSensei_clear_screen);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_write_text_obj, 1, MeloperoSensei_write_text);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_update_display_obj, MeloperoSensei_update_display);






// MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_set_pixel_obj, 1, MeloperoSensei_set_pixel);
// MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_set_matrix_obj, 1, MeloperoSensei_set_matrix);
// MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_enable_repeated_start_obj, MeloperoSensei_enable_repeated_start);
// MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_disable_repeated_start_obj, MeloperoSensei_disable_repeated_start);
// MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_stop_message_obj, MeloperoSensei_stop_message);
// MP_DEFINE_CONST_FUN_OBJ_2(MeloperoSensei_set_direction_obj, MeloperoSensei_set_direction);
// MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_led_init_obj, MeloperoSensei_led_init);
// MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_led_on_obj, MeloperoSensei_led_on);
// MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_led_off_obj, MeloperoSensei_led_off);
// MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_led_toggle_obj, MeloperoSensei_led_toggle);



//Bindings of methods

STATIC const mp_rom_map_elem_t MeloperoSensei_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write_text), MP_ROM_PTR(&MeloperoSensei_write_text_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&MeloperoSensei___del___obj) },
    { MP_ROM_QSTR(MP_QSTR_update_display), MP_ROM_PTR(&MeloperoSensei_update_display_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear_screen), MP_ROM_PTR(&MeloperoSensei_clear_screen_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_text_color), MP_ROM_PTR(&MeloperoSensei_set_text_color_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_display_color), MP_ROM_PTR(&MeloperoSensei_set_display_color_obj) },
   
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
