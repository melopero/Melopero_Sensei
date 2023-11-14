/*
Melopero Cookie RP2040 Micropython library. 

Authors: Francesco Marchetti, Luca Davidian

Get your Cookie RP2040 here: 
http://melopero.com/melopero-cookie-rp2040

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release March 2023

*/


#include "../../C++/src/MeloperoSensei.hpp"
#include <cstdio>
#include <string>



extern "C" {
    
    #include "sensei.h"
    #include "py/builtin.h"
    #include "py/objstr.h"


//variables struct
typedef struct _MeloperoSensei_obj_t {

    mp_obj_base_t base;
    MeloperoSensei* sensei;
    char *buf;

}_MeloperoSensei_obj_t;


//Print
void MeloperoSensei_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind){

    (void)kind; //Unused input parameter
    //_MeloperoSensei_obj_t *self = MP_OBJ_TO_PTR2(self_in, _MeloperoSensei_obj_t);
    mp_print_str(print, "MeloperoSensei()");
}


//Constructor
mp_obj_t MeloperoSensei_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    
    
    _MeloperoSensei_obj_t *self = m_new_obj_with_finaliser(_MeloperoSensei_obj_t);
    char *buf = m_new(char, 240 * 240 * 2);
    MeloperoSensei *sensei = new MeloperoSensei(buf);
    self->base.type = &MeloperoSensei_type;
    self->sensei = sensei;
    //self->buf = buf;
    
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t MeloperoSensei___del__(mp_obj_t self_in) {

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    self->sensei->~MeloperoSensei();

    return mp_const_none;
}


//Methods
mp_obj_t MeloperoSensei_write_text(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_text, ARG_posx, ARG_posy};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_text, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_posx, MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_posy, MP_ARG_INT, {.u_int = 20} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    mp_obj_t message_in = args[ARG_text].u_obj;

    if(!mp_obj_is_str_or_bytes(message_in)) mp_raise_TypeError("message must be a string");
    GET_STR_DATA_LEN(message_in, str, str_len);
    std::string message((const char*)str);
   
    int posx = args[ARG_posx].u_int;
    int posy = args[ARG_posy].u_int;
    
    self->sensei->print(posx, posy, message.c_str());

    return mp_const_none;
}



mp_obj_t MeloperoSensei_set_text_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_r, ARG_g, ARG_b};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_r, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_b, MP_ARG_REQUIRED | MP_ARG_INT },
        
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    
    int r = args[ARG_r].u_int;
    int g = args[ARG_g].u_int;
    int b = args[ARG_b].u_int;

    self->sensei->setTextColor(r,g,b);
    return mp_const_none;
    
}



mp_obj_t MeloperoSensei_set_display_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_r, ARG_g, ARG_b};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_r, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_b, MP_ARG_REQUIRED | MP_ARG_INT },
       
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    
    int r = args[ARG_r].u_int;
    int g = args[ARG_g].u_int;
    int b = args[ARG_b].u_int;

    
    self->sensei->clearScreen(r,g,b);
    return mp_const_none;
    
}



mp_obj_t MeloperoSensei_clear_screen(size_t n_args, const mp_obj_t *args) {

  
    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[0]);
    if(n_args>1)
    {
        enum{ARG_self,ARG_r, ARG_g, ARG_b};
        uint8_t r = mp_obj_get_int(args[ARG_r]);
        uint8_t g = mp_obj_get_int(args[ARG_g]);
        uint8_t b = mp_obj_get_int(args[ARG_b]);

        self->sensei->clearScreen(r,g,b);
        return mp_const_none;
    }

    self->sensei->clearScreen(0,0,0);
    return mp_const_none;
}


extern mp_obj_t MeloperoSensei_update_display(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    self->sensei->presentScreen();

    return mp_const_none;
}








/*



mp_obj_t MeloperoSensei_set_matrix(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_r, ARG_g, ARG_b, ARG_brightness};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_r, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_brightness, MP_ARG_REQUIRED | MP_ARG_OBJ},
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    
    int r = args[ARG_r].u_int;
    int g = args[ARG_g].u_int;
    int b = args[ARG_b].u_int;
    float brightness = mp_obj_get_float(args[ARG_brightness].u_obj);
    
    uint32_t color = self->cookie->urgb_u32(r,g,b,brightness);
    uint32_t pixels[25];
    
    for(int i=0; i<25; i++)
    {
        pixels[i]=color;
    }

    self->cookie->set_matrix(pixels);
    return mp_const_none;
}



mp_obj_t MeloperoSensei_enable_repeated_start(mp_obj_t self_in) {

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->set_repeated_start(true);
    return mp_const_none;
}

mp_obj_t MeloperoSensei_disable_repeated_start(mp_obj_t self_in) {

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->set_repeated_start(false);
    return mp_const_none;
}




mp_obj_t MeloperoSensei_set_direction(mp_obj_t self_in, mp_obj_t direction) {

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    //uint8_t dir = mp_obj_get_int(direction);
    int dir = mp_obj_get_int(direction);
    if((dir <0) | (dir >1)) 
    {
        return mp_const_none;
    }
    self->cookie->set_direction(static_cast<Direction>(dir));
    return mp_const_none;
}




mp_obj_t MeloperoSensei_set_rgb_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_r, ARG_g, ARG_b, ARG_brightness};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_r, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_b, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_brightness, MP_ARG_OBJ, { .u_obj = mp_const_none }},
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    
    int r = args[ARG_r].u_int;
    int g = args[ARG_g].u_int;
    int b = args[ARG_b].u_int;

     if (args[ARG_brightness].u_obj != mp_const_none)
     {
        float brightness = mp_obj_get_float(args[ARG_brightness].u_obj);
        self->cookie->set_rgb_color(r,g,b,brightness);
        return mp_const_none;

     }
    
    self->cookie->set_rgb_color(r,g,b);
    return mp_const_none;
    
}


mp_obj_t MeloperoSensei_led_init(mp_obj_t self_in) {

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->led_init();
    return mp_const_none;
}

mp_obj_t MeloperoSensei_led_on(mp_obj_t self_in) {

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->led_on();
    return mp_const_none;
}

mp_obj_t MeloperoSensei_led_off(mp_obj_t self_in) {

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->led_off();
    return mp_const_none;
}

mp_obj_t MeloperoSensei_led_toggle(mp_obj_t self_in) {

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    
    self->cookie->led_toggle();
    return mp_const_none;
}
    
*/


}//extern C


 

