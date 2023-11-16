/*
Melopero Sensei Micropython library. 

Authors: Luca Davidian, Leonardo La Rocca, Francesco Marchetti

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release November 2023

*/


/*
TODO
ok_void drawPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
ok_void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue);
ok_void setTextFont(enum font_name name, enum font_size size);
ok_float getTemperature();

void drawSprite(const uint16_t sprite[], int x, int y, uint16_t width, uint16_t height, uint16_t transparency = 0xE037 0x30FF00);
ButtonState getButtonState(uint8_t button);

bool isButtonPressed(uint8_t button);
void enableButtonInterrupt(bool enable);
bool getButtonIRQState(uint8_t button);
uint8_t getInputInterruptRegister();
void playNote(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time);
void setLightMin();
void setLightMax(); 
uint8_t getLightLevel();
uint8_t getBatteryLevel(); 


virtual void setup() {}
virtual void update() {}    
virtual void draw() {}
void run();
*/

#include "sensei.h"

//Methods


MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei___del___obj, MeloperoSensei___del__);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_set_text_color_obj, 1, MeloperoSensei_set_text_color);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_set_display_color_obj, 1, MeloperoSensei_set_display_color);
MP_DEFINE_CONST_FUN_OBJ_VAR(MeloperoSensei_clear_screen_obj, 1, MeloperoSensei_clear_screen);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_write_text_obj, 1, MeloperoSensei_write_text);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_update_display_obj, MeloperoSensei_update_display);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_rect_obj, 1, MeloperoSensei_draw_rect);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_fill_rect_obj, 1, MeloperoSensei_draw_fill_rect);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_line_obj, 1, MeloperoSensei_draw_line);
MP_DEFINE_CONST_FUN_OBJ_KW(MeloperoSensei_draw_pixel_obj, 1, MeloperoSensei_draw_pixel);
MP_DEFINE_CONST_FUN_OBJ_3(MeloperoSensei_set_text_font_obj, MeloperoSensei_set_text_font);
MP_DEFINE_CONST_FUN_OBJ_1(MeloperoSensei_get_cpu_temp_obj, MeloperoSensei_get_cpu_temp);

//Bindings of methods

STATIC const mp_rom_map_elem_t MeloperoSensei_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write_text), MP_ROM_PTR(&MeloperoSensei_write_text_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&MeloperoSensei___del___obj) },
    { MP_ROM_QSTR(MP_QSTR_update_display), MP_ROM_PTR(&MeloperoSensei_update_display_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_cpu_temp), MP_ROM_PTR(&MeloperoSensei_get_cpu_temp_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear_screen), MP_ROM_PTR(&MeloperoSensei_clear_screen_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_text_color), MP_ROM_PTR(&MeloperoSensei_set_text_color_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_text_font), MP_ROM_PTR(&MeloperoSensei_set_text_font_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_display_color), MP_ROM_PTR(&MeloperoSensei_set_display_color_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_rect), MP_ROM_PTR(&MeloperoSensei_draw_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_fill_rect), MP_ROM_PTR(&MeloperoSensei_draw_fill_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_line), MP_ROM_PTR(&MeloperoSensei_draw_line_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_pixel), MP_ROM_PTR(&MeloperoSensei_draw_pixel_obj) },
   
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
    { MP_ROM_QSTR(MP_QSTR_TEXT_VERY_SMALL), MP_ROM_INT(TEXT_VERY_SMALL) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_SMALL), MP_ROM_INT(TEXT_SMALL) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_REGULAR), MP_ROM_INT(TEXT_REGULAR) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_BIG), MP_ROM_INT(TEXT_BIG) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_MONO), MP_ROM_INT(TEXT_MONO) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_MONO_BOLD), MP_ROM_INT(TEXT_MONO_BOLD) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_SANS), MP_ROM_INT(TEXT_SANS) },
    { MP_ROM_QSTR(MP_QSTR_TEXT_SANS_BOLD), MP_ROM_INT(TEXT_SANS_BOLD) },

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
