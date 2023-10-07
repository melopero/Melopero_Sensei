#include "audio.h"
#include "PWM.h"
#include "fifo.h"
#include "pico/time.h"

#define PWM_SLICE      7
#define PWM_CHANNEL    1
#define BUZZER_PIN    15

static struct repeating_timer audio_repeating_timer;
bool audio_callback(repeating_timer_t *rt); 

static FIFO *audio_FIFO;

void audio_init(void)
{
    PWM_config config = { PWM_SLICE, PWM_CHANNEL, BUZZER_PIN };
    PWM_init(&config);

    audio_FIFO = FIFO_new(100);

    add_repeating_timer_ms(-1, audio_callback, NULL, &audio_repeating_timer);
}

void audio_deinit(void)
{
    FIFO_destroy(audio_FIFO);
}

static note n = {0.0f, 0.0f, 0.0f, false, 0.0f, 0};

void audio_play_note(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time)
{
    note n = { frequency, duration, volume, sweep_direction, sweep_time, 0};
    FIFO_insert(audio_FIFO, n);
}

// called every millisecond
bool audio_callback(repeating_timer_t *rt)
{
    if (n.elapsed >= n.duration)
        if (!FIFO_is_empty(audio_FIFO))
        {
            n = FIFO_get(audio_FIFO);
            PWM_set_compare_value(0xFFFF >> 1); 
        }
        else
        {
            n.duration = 0;
            PWM_set_compare_value(0); 
        }

    if (n.duration)
        if (++n.elapsed < n.duration)
        {
            if (n.sweep_time)
                n.sweep_direction ? (n.frequency += 10.0f) : (n.frequency -= 10.0f);
            
            PWM_set_frequency(n.frequency);    // update pwm   
        }

    return true;
}



// // state 
//   voice_t  _voice;
//   uint32_t _ms;
//   uint32_t _frequency;
//   uint32_t _start_frequency;
//   uint32_t _duration;
//   uint32_t _volume = 100;

// struct voice_t {
//     int32_t  bend; // pitch bend (hz)
//     uint32_t bend_ms; // bend speed (ms)
//     uint32_t attack, decay, sustain, release; // envelope (ms, ms, %, ms)
//     uint32_t reverb; // effects (ms)
//     uint32_t noise, distort; // effects (strength 0..100)
//   };

// // set state
//   void play(voice_t voice,
//     uint32_t frequency, uint32_t duration, uint32_t volume) {
//     _frequency = frequency;
//     _start_frequency = frequency;
//     _duration = duration;
//     _volume = volume;
//     _voice = voice;
//     _ms = 0;
//   }


//   void _update_audio() {
//     // pitch bend
//     if(_voice.bend && _voice.bend_ms) {
//       _frequency = _start_frequency + (_voice.bend * (_ms / _voice.bend_ms));
//     }
//     uint32_t sample = audio_sample(_ms++);
//     _play_note(_frequency, sample);
//   }

//    uint8_t audio_sample(uint32_t ms) {
//     // calculate full duration including release and reverb
//     uint32_t full_duration = _duration + _voice.release + _voice.reverb;
//     uint32_t sample = 0;
//     if(ms < full_duration) {
//       // skip samples for distortion effect
//       if(_voice.distort) {
//         ms -= (ms % _voice.distort);
//       }

//       // grab volume as start point for sample
//       sample = _volume;

//       // apply envelop to waveform sample
//       uint32_t env = 0;
//       if(ms < std::min(_duration, _voice.attack)) {
//         env = (ms << 8) / _voice.attack;
//       } else if (ms < std::min(_duration, _voice.attack + _voice.decay)) {
//         env = (1 << 8) - (((((ms - _voice.attack) << 8) / _voice.decay) * ((1 << 8) - ((_voice.sustain << 8) / 100))) >> 8);
//       } else if (ms < _duration) {
//         env = (_voice.sustain << 8) / 100;
//       } else if (ms < _duration + _voice.release) {
//         uint32_t r = ((ms - _duration) << 8) / _voice.release;
//         env = (((1 << 8) - r) * ((_voice.sustain << 8) / 100)) >> 8;
//       }
//       sample *= env;
//       sample >>= 8;

//       // apply noise
//       if(_voice.noise) {
//         int32_t n = (std::rand() % _voice.noise) - (_voice.noise / 2);
//         sample = int32_t(sample) + n < 0 ? 0 : (int32_t(sample) + n > 100 ? 100 : sample + n);
//       }

//       if(_voice.reverb && ms > _voice.reverb) {
//         uint8_t rs = audio_sample(ms - _voice.reverb);
//         sample = ((sample * ms) / full_duration) + ((rs * (full_duration - ms)) / full_duration);
//       }
//     }

//     return sample;
//   }

//   void _play_note(uint32_t f, uint32_t v) {
//     // adjust the clock divider to achieve this desired frequency
//     #ifndef NO_OVERCLOCK
//       float clock = 250000000.0f;
//     #else
//       float clock = 125000000.0f;
//     #endif

//     float pwm_divider = clock / _audio_pwm_wrap / f;
//     pwm_set_clkdiv(pwm_gpio_to_slice_num(AUDIO), pwm_divider);
//     pwm_set_wrap(pwm_gpio_to_slice_num(AUDIO), _audio_pwm_wrap);

//     // work out usable range of volumes at this frequency. the piezo speaker
//     // isn't driven in a way that can control volume easily however if we're
//     // clever with the duty cycle we can ensure that the ceramic doesn't have
//     // time to fully deflect - effectively reducing the volume.
//     //
//     // through experiment it seems that constraining the deflection period of
//     // the piezo to between 0 and 1/10000th of a second gives reasonable control
//     // over the volume. the relationship is non linear so we also apply a
//     // correction curve which is tuned so that the result sounds reasonable.
//     uint32_t max_count = (f * _audio_pwm_wrap) / 10000;

//     // the change in volume isn't linear - we correct for this here
//     float curve = 1.8f;
//     uint32_t level = (pow((float)(v) / 100.0f, curve) * max_count);
//     pwm_set_gpio_level(AUDIO, level);
//   }