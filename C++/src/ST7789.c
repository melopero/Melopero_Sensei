#include "ST7789.h"
#include "SPI.h"
#include "hardware/gpio.h"
#include "pico/time.h"

/**** ST7735 command list ****/

/* system function commands */
#define NOP       0x00  /* no operation */
#define SWRESET   0x01  /* software reset */
#define SLPIN     0x10  /* sleep in */
#define SLPOUT    0x11  /* sleep out */
#define PTLON     0x12  /* partial display mode ON */
#define NORON     0x13  /* normal display mode ON */
#define INVOFF    0x20  /* display inversion OFF */
#define INVON     0x21  /* display inversion ON */
#define GAMSET    0x26  /* set gamma curve */
#define DISPOFF   0x28  /* display OFF */
#define DISPON    0x29  /* display ON */
#define CASET     0x2A  /* column address set */
#define RASET     0x2B  /* row address set */
#define RAMWR     0x2C  /* memory write */
#define RGBSET    0x2D  /* color setting */
#define PTLAR     0x30  /* partial area */
#define TEOFF     0x34  /* tearing effect line OFF */
#define TEON      0x35  /* tearing effect line ON */
#define MADCTL    0x36  /* memory data access control */
#define COLMOD    0x3A  /* pixel format (12bit,16bit,18bit) */

/* panel function commands */
#define FRMCTR1   0xB1  /* frame rate control (normal mode/full colors) */
#define FRMCTR2   0xB2  /* frame rate control (idle mode/8 colors) */
#define FRMCTR3   0xB3  /* frame rate control (partial mode(full colors) */
#define INVCTR    0xB4  /* display inversion control */
#define PWRCTR1   0xC0  /* power control 1 */
#define PWRCTR2   0xC1  /* power control 2 */
#define PWRCTR3   0xC2  /* power control 3 (normal mode/full colors) */
#define PWRCTR4   0xC3  /* power control 4 (idle mode/8 colors) */
#define PWRCTR5   0xC4  /* power control 5 (partial mode/full colors) */

/* memory data address control (MADCTL) bits */
#define MX          6U  /* invert x */
#define MY          7U  /* invert y */
#define MV          5U  /* exchange x and y */
#define ML          4U  /* vertical refresh order */
#define MH          2U  /* horizontal refresh order */
#define RGB         3U  /* RGB-BGR order */

/* pixel format parameter */
#define BIT_12      3   /* 12 bit pixel format */
#define BIT_16      5   /* 16 bit pixel format */
#define BIT_18      6   /* 18 bit pixel format */

/* tearing effect mode */
#define TE_MODE1    0   // V-blanking info only
#define TE_MODE2    1   // V-blanking and H-blanking info

#define COMMAND()          gpio_put(DC, 0)
#define DATA()             gpio_put(DC, 1)
#define RESET_LOW()        gpio_put(RST, 0)
#define RESET_HIGH()       gpio_put(RST, 1)

#define DC                       14
#define RST                       0
#define BL                       16
#define TE                        1
#define HORIZONTAL                0

void ST7789_init()
{
	gpio_init(DC);
    gpio_set_dir(DC, GPIO_OUT);
    
	gpio_init(RST);
    gpio_set_dir(RST, GPIO_OUT);

	gpio_init(BL);
    gpio_set_dir(BL, GPIO_OUT);
    gpio_put(BL, 1);

	gpio_init(TE);
    gpio_set_dir(TE, GPIO_IN);

	RESET_LOW();
	sleep_us(1);
	RESET_HIGH();

	ST7789_send_command(SWRESET, 120);
	ST7789_send_command(SLPOUT, 120);
	//ST7789_send_command(COLMOD, 0);
	//ST7789_send_data(BIT_16);
	ST7789_send_command_with_data(COLMOD, (uint8_t[]){BIT_16}, 1, 0);
	ST7789_send_command(INVON, 0);
	ST7789_send_command(NORON, 0);
	ST7789_send_command_with_data(TEON, (uint8_t[]){TE_MODE1}, 1, 0);
	ST7789_set_memory_address_control(0, HORIZONTAL, HORIZONTAL);
	ST7789_clear_screen(0x00, 0x00, 0x00);
	ST7789_send_command(DISPON, 0);
}

void ST7789_set_memory_address_control(uint8_t x_mirror, uint8_t y_mirror, uint8_t xy_exchange)
{
	//ST7789_send_command(MADCTL, 0);

	uint8_t control = 0;
	if (x_mirror)
		control |= 1U << MX;
	if (y_mirror)
		control |= 1U << MY;
	if (xy_exchange)
		control |= 1U << MV;

	//ST7789_send_data(control);

	ST7789_send_command_with_data(MADCTL, (uint8_t[]){control}, 1, 0);
}

void ST7789_send_command(uint8_t command, uint8_t delay)
{
	COMMAND();
	SPI_write(&command, 1);
	if (delay)
		sleep_ms(delay);
}

void ST7789_send_command_with_data(uint8_t command, uint8_t *data, uint8_t len, uint8_t delay)
{
	COMMAND();
	SPI_write(&command, 1);
	if (delay)
		sleep_ms(delay);

	DATA();
	SPI_write(data, len);
}

void ST7789_send_data(uint8_t data)
{
	DATA();
	SPI_write(&data, 1);
}

void ST7789_send_data_buffer(uint8_t *data, uint32_t len)
{
	DATA();
	SPI_write(data, len);
}

void ST7789_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	// ST7789_send_command(CASET, 0);
	// ST7789_send_data(x0 >> 8);
	// ST7789_send_data(x0);
	// ST7789_send_data(x1 >> 8);
	// ST7789_send_data(x1);

	// ST7789_send_command(RASET, 0);
	// ST7789_send_data(y0 >> 8);
	// ST7789_send_data(y0);
	// ST7789_send_data(y1 >> 8);
	// ST7789_send_data(y1);

	ST7789_send_command_with_data(CASET, (uint8_t[]){x0 >> 8, x0, x1 >> 8, x1}, 4, 0);
	ST7789_send_command_with_data(RASET, (uint8_t[]){y0 >> 8, y0, y1 >> 8, y1}, 4, 0);

	ST7789_send_command(RAMWR, 0);
}

void ST7789_clear_screen(uint8_t red, uint8_t green, uint8_t blue)
{
	ST7789_set_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
	
	for (int i = 0 ; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++)
		ST7789_push_color(red, green, blue);
}

void ST7789_push_color(uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t color = (uint16_t)(red & 0xF8) << 8 | (uint16_t)(green & 0xFC) << 3 | blue >> 3;

	//ST7789_send_data((uint8_t)(color >> 8));
	//ST7789_send_data((uint8_t)color);
	ST7789_send_data_buffer((uint8_t[]){(uint8_t)(color >> 8), (uint8_t)color}, 2);
}

void ST7789_draw_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	if (x < 0 || x > DISPLAY_WIDTH - 1 || y < 0 || y > DISPLAY_HEIGHT - 1)
		return;

	ST7789_set_window(x, y, x, y);
	ST7789_push_color(red, green, blue);
}

void ST7789_push_framebuffer(uint16_t *framebuffer)
{
	ST7789_set_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);   // TODO: set window once only
	ST7789_send_data_buffer((uint8_t*)framebuffer, DISPLAY_WIDTH * DISPLAY_HEIGHT * 2);
}

void ST7789_wait_VSYNC()
{
	bool in_VSYNC = gpio_get(TE);

	while (in_VSYNC)
		in_VSYNC = gpio_get(TE);

	while (!in_VSYNC)
		in_VSYNC = gpio_get(TE);
}



