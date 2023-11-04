#include "audio.h"
#include "PWM.h"
#include "audio_fifo.h"
#include "pico/time.h"

#define PWM_SLICE      7
#define PWM_CHANNEL    1
#define BUZZER_PIN    15

static struct repeating_timer audio_repeating_timer;
bool audio_callback(repeating_timer_t *rt); 

void audio_init(void)
{
    PWM_config config = { PWM_SLICE, PWM_CHANNEL, BUZZER_PIN };
    PWM_init(&config);

    add_repeating_timer_ms(-1, audio_callback, NULL, &audio_repeating_timer);
}

static note n = {0.0f, 0.0f, 0.0f, false, 0.0f, 0};

void audio_play_note(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time)
{
    note n = { frequency, duration, volume, sweep_direction, sweep_time, 0};
    FIFO_insert(n);
}

// called every millisecond
bool audio_callback(repeating_timer_t *rt)
{
    if (n.elapsed >= n.duration)
    {
        if (!FIFO_is_empty())
        {
            n = FIFO_get();
            PWM_set_compare_value(0xFFFF >> 1); 
        }
        else
        {
            n.duration = 0;
            PWM_set_compare_value(0); 
        }
    }

    if (n.duration)
    {
        if (++n.elapsed < n.duration)
        {
            if (n.sweep_time)
                n.sweep_direction ? (n.frequency += 10.0f) : (n.frequency -= 10.0f);
            
            PWM_set_frequency(n.frequency);    // update pwm   
        }
    }

    return true;
}
