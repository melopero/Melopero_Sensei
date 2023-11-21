/*
Melopero Sensei Micropython library. 

Authors: Luca Davidian, Leonardo La Rocca, Francesco Marchetti

Copyright 2023 Melopero S.r.l. - www.melopero.com
First release November 2023

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



mp_obj_t MeloperoSensei_draw_rect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_posx, ARG_posy, ARG_width, ARG_height, ARG_red, ARG_green, ARG_blue};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_posx, MP_ARG_INT, {.u_int = 50} },
        { MP_QSTR_posy, MP_ARG_INT, {.u_int = 50} },
        { MP_QSTR_width, MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_height, MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_red, MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_green, MP_ARG_INT, {.u_int = 100} },
        { MP_QSTR_blue, MP_ARG_INT, {.u_int = 20} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    int posx = args[ARG_posx].u_int;
    int posy = args[ARG_posy].u_int;
    int width = args[ARG_width].u_int;
    int height = args[ARG_height].u_int;
    int red = args[ARG_red].u_int;
    int green = args[ARG_green].u_int;
    int blue = args[ARG_blue].u_int;
    
    self->sensei->drawRect(posx,posy,width, height, red, green, blue);

    return mp_const_none;
}

mp_obj_t MeloperoSensei_draw_fill_rect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_posx, ARG_posy, ARG_width, ARG_height, ARG_red, ARG_green, ARG_blue};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_posx, MP_ARG_INT, {.u_int = 50} },
        { MP_QSTR_posy, MP_ARG_INT, {.u_int = 50} },
        { MP_QSTR_width, MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_height, MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_red, MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_green, MP_ARG_INT, {.u_int = 100} },
        { MP_QSTR_blue, MP_ARG_INT, {.u_int = 20} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    int posx = args[ARG_posx].u_int;
    int posy = args[ARG_posy].u_int;
    int width = args[ARG_width].u_int;
    int height = args[ARG_height].u_int;
    int red = args[ARG_red].u_int;
    int green = args[ARG_green].u_int;
    int blue = args[ARG_blue].u_int;
    
    self->sensei->drawFillRect(posx,posy,width, height, red, green, blue);

    return mp_const_none;
}

//playNote(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time);
mp_obj_t MeloperoSensei_play_note(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_frequency, ARG_duration, ARG_volume, ARG_sweep_dir, ARG_sweep_time};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_frequency, MP_ARG_OBJ },
        { MP_QSTR_duration, MP_ARG_INT },
        { MP_QSTR_volume, MP_ARG_OBJ },
        { MP_QSTR_sweep_dir, MP_ARG_BOOL, {.u_bool = false} },
        { MP_QSTR_sweep_time, MP_ARG_OBJ },
     
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);


    uint32_t duration = (uint32_t)args[ARG_duration].u_int;
    float frequency = mp_obj_get_float(args[ARG_frequency].u_obj);
    float volume = mp_obj_get_float(args[ARG_volume].u_obj);
    bool sweep_direction = mp_obj_is_true(args[ARG_sweep_dir].u_obj);
    float sweep_time = mp_obj_get_float(args[ARG_sweep_time].u_obj);
    
    self->sensei->playNote(frequency, duration, volume, sweep_direction, sweep_time);

    return mp_const_none;
}

mp_obj_t MeloperoSensei_draw_line(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_startx, ARG_starty, ARG_endx, ARG_endy, ARG_red, ARG_green, ARG_blue};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_startx, MP_ARG_INT },
        { MP_QSTR_starty, MP_ARG_INT },
        { MP_QSTR_endx, MP_ARG_INT },
        { MP_QSTR_endy, MP_ARG_INT },
        { MP_QSTR_red, MP_ARG_INT },
        { MP_QSTR_green, MP_ARG_INT },
        { MP_QSTR_blue, MP_ARG_INT },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    int startx = args[ARG_startx].u_int;
    int starty = args[ARG_starty].u_int;
    int endx = args[ARG_endx].u_int;
    int endy = args[ARG_endy].u_int;
    int red = args[ARG_red].u_int;
    int green = args[ARG_green].u_int;
    int blue = args[ARG_blue].u_int;
    
    self->sensei->drawLine(startx,starty,endx,endy, red, green, blue);

    return mp_const_none;
}

mp_obj_t MeloperoSensei_draw_pixel(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_posx, ARG_posy, ARG_red, ARG_green, ARG_blue};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_posx, MP_ARG_INT, {.u_int = 50} },
        { MP_QSTR_posy, MP_ARG_INT, {.u_int = 50} },
        { MP_QSTR_red, MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_green, MP_ARG_INT, {.u_int = 100} },
        { MP_QSTR_blue, MP_ARG_INT, {.u_int = 20} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    int posx = args[ARG_posx].u_int;
    int posy = args[ARG_posy].u_int;
    int red = args[ARG_red].u_int;
    int green = args[ARG_green].u_int;
    int blue = args[ARG_blue].u_int;
    
    self->sensei->drawPixel(posx, posy, red, green, blue);

    return mp_const_none;
}

mp_obj_t MeloperoSensei_draw_sprite(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_buffer, ARG_posx, ARG_posy, ARG_width, ARG_height};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_buffer, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_posx, MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_posy, MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_width, MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_height, MP_ARG_INT, {.u_int = 0} },
      
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    // Convert bytearray to uint16_t*
    mp_buffer_info_t buf_info;
    mp_obj_t data_obj = args[ARG_buffer].u_obj;
    mp_get_buffer_raise(data_obj, &buf_info, MP_BUFFER_READ);

    uint16_t *data_ptr = (uint16_t *)buf_info.buf;
    uint16_t posx = args[ARG_posx].u_int;
    uint16_t posy = args[ARG_posy].u_int;
    uint16_t width = args[ARG_width].u_int;
    uint16_t height = args[ARG_height].u_int;

    
    
    self->sensei->drawSprite(data_ptr,posx,posy,width, height);

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

mp_obj_t MeloperoSensei_set_text_font(mp_obj_t self_in, mp_obj_t font, mp_obj_t size) {
    
    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    
      uint8_t f = mp_obj_get_int(font);
      uint8_t s = mp_obj_get_int(size);
    self->sensei->setTextFont((enum font_name) f,(enum font_size) s);

    return mp_const_none;
}

mp_obj_t MeloperoSensei_is_button_pressed(mp_obj_t self_in, mp_obj_t button) {
    
    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    
    uint8_t b =(uint8_t) mp_obj_get_int(button);
    bool result = false;
      
    result = self->sensei->isButtonPressed(b);

    return mp_obj_new_bool(result);
}

mp_obj_t MeloperoSensei_get_button_irq_state(mp_obj_t self_in, mp_obj_t button) {
    
    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    
    int b = mp_obj_get_int(button);
    bool result = false;
      
    result = self->sensei->getButtonIRQState(b);

    return mp_obj_new_bool(result);
}




mp_obj_t MeloperoSensei_get_button_state(mp_obj_t self_in, mp_obj_t button) {
    
    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

    
    int b = mp_obj_get_int(button);
    
      
    int result = self->sensei->getButtonState(b);

    return mp_obj_new_int(result);
}

mp_obj_t MeloperoSensei_enable_button_interrupt(mp_obj_t self_in, mp_obj_t enable) {
    
    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);

     bool c_enable = mp_obj_is_true(enable);
      
    self->sensei->enableButtonInterrupt(c_enable);

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

extern mp_obj_t MeloperoSensei_set_light_min(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    self->sensei->setLightMin();

    return mp_const_none;
}

extern mp_obj_t MeloperoSensei_set_light_max(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    self->sensei->setLightMax();

    return mp_const_none;
}

extern mp_obj_t MeloperoSensei_read_battery(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    uint8_t bat = self->sensei->getBatteryLevel();
    
    return mp_obj_new_int(bat);
}

extern mp_obj_t MeloperoSensei_read_light(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    uint8_t light = self->sensei->getLightLevel();
    
    return mp_obj_new_int(light);
}


extern mp_obj_t MeloperoSensei_touch_init(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    self->sensei->touch_init();

    return mp_const_none;
}

extern mp_obj_t MeloperoSensei_get_touch(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    uint16_t result = self->sensei->get_touch();
    
    return mp_obj_new_int(result);
}


extern mp_obj_t MeloperoSensei_get_cpu_temp(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    float temp = self->sensei->getTemperature();
    
    return mp_obj_new_float(temp);
}


extern mp_obj_t MeloperoSensei_get_interrupt_register(mp_obj_t self_in){

    _MeloperoSensei_obj_t *self = (_MeloperoSensei_obj_t*) MP_OBJ_TO_PTR(self_in);
    uint8_t reg = self->sensei->getInputInterruptRegister();
    
    return mp_obj_new_int_from_uint(reg);
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


 

