#include "microphone.h"
#include "mic-lib-for-pico/include/pico/pdm_microphone.h"
#include "mic-lib-for-pico/usb_microphone.h"
#include <stdint.h>

const struct pdm_microphone_config config = {
    // GPIO pin for the PDM DAT signal
    .gpio_data = 2,

    // GPIO pin for the PDM CLK signal
    .gpio_clk = 3,

    // PIO instance to use
    .pio = pio0,

    // PIO State Machine instance to use
    .pio_sm = 0,

    // sample rate in Hz
    .sample_rate = 16000,

    // number of samples to buffer
    .sample_buffer_size = SAMPLE_BUFFER_SIZE,
};

int16_t sample_buffer[SAMPLE_BUFFER_SIZE];
volatile int samples_read = 0;

void on_pdm_samples_ready(void)
{
    samples_read = pdm_microphone_read(sample_buffer, SAMPLE_BUFFER_SIZE);
}

void on_usb_microphone_tx_ready(void)
{
    usb_microphone_write(sample_buffer, sizeof(sample_buffer));
}

void microphone_init(void)
{
    if (pdm_microphone_init(&config) < 0)
        tight_loop_contents();

    pdm_microphone_set_samples_ready_handler(on_pdm_samples_ready);

    pdm_microphone_set_filter_max_volume(10);
    pdm_microphone_set_filter_volume(10);
    pdm_microphone_set_filter_gain(16);
}

void microphone_USB_init(void)
{
    usb_microphone_init();
    usb_microphone_set_tx_ready_handler(on_usb_microphone_tx_ready);
}

void microphone_deinit(void)
{
    pdm_microphone_deinit();
}

void microphone_USB_deinit(void)
{
    microphone_deinit();
}

void microphone_enable(bool enable)
{
    if (enable)
        pdm_microphone_start();
    else // !enable
        pdm_microphone_stop();
}

int microphone_get_samples_read(void)
{
    return samples_read;
}

int16_t microphone_get_sample(int index)
{
    return sample_buffer[index];
}

void microphone_USB_task(void)
{
    usb_microphone_task();
}