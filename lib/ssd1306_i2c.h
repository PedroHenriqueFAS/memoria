// #include <stdlib.h>
// #include "pico/stdlib.h"
// #include "hardware/i2c.h"

// #ifndef ssd1306_inc_h
// #define ssd1306_inc_h

// #define ssd1306_height 64 // Define a altura do display (32 pixels)
// #define ssd1306_width 128 // Define a largura do display (128 pixels)

// #define ssd1306_i2c_address _u(0x3C) // Define o endereço do i2c do display

// #define ssd1306_i2c_clock 400 // Define o tempo do clock (pode ser aumentado)

// // Comandos de configuração (endereços)
// #define ssd1306_set_memory_mode _u(0x20)
// #define ssd1306_set_column_address _u(0x21)
// #define ssd1306_set_page_address _u(0x22)
// #define ssd1306_set_horizontal_scroll _u(0x26)
// #define ssd1306_set_scroll _u(0x2E)

// #define ssd1306_set_display_start_line _u(0x40)

// #define ssd1306_set_contrast _u(0x81)
// #define ssd1306_set_charge_pump _u(0x8D)

// #define ssd1306_set_segment_remap _u(0xA0)
// #define ssd1306_set_entire_on _u(0xA4)
// #define ssd1306_set_all_on _u(0xA5)
// #define ssd1306_set_normal_display _u(0xA6)
// #define ssd1306_set_inverse_display _u(0xA7)
// #define ssd1306_set_mux_ratio _u(0xA8)
// #define ssd1306_set_display _u(0xAE)
// #define ssd1306_set_common_output_direction _u(0xC0)
// #define ssd1306_set_common_output_direction_flip _u(0xC0)

// #define ssd1306_set_display_offset _u(0xD3)
// #define ssd1306_set_display_clock_divide_ratio _u(0xD5)
// #define ssd1306_set_precharge _u(0xD9)
// #define ssd1306_set_common_pin_configuration _u(0xDA)
// #define ssd1306_set_vcomh_deselect_level _u(0xDB)

// #define ssd1306_page_height _u(8)
// #define ssd1306_n_pages (ssd1306_height / ssd1306_page_height)
// #define ssd1306_buffer_length (ssd1306_n_pages * ssd1306_width)

// #define ssd1306_write_mode _u(0xFE)
// #define ssd1306_read_mode _u(0xFF)

// struct render_area {
//     uint8_t start_column;
//     uint8_t end_column;
//     uint8_t start_page;
//     uint8_t end_page;

//     int buffer_length;
// };

// typedef struct {
//   uint8_t width, height, pages, address;
//   i2c_inst_t * i2c_port;
//   bool external_vcc;
//   uint8_t *ram_buffer;
//   size_t bufsize;
//   uint8_t port_buffer[2];
// } ssd1306_t;

// #endif

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define WIDTH 128
#define HEIGHT 64

typedef enum {
  SET_CONTRAST = 0x81,
  SET_ENTIRE_ON = 0xA4,
  SET_NORM_INV = 0xA6,
  SET_DISP = 0xAE,
  SET_MEM_ADDR = 0x20,
  SET_COL_ADDR = 0x21,
  SET_PAGE_ADDR = 0x22,
  SET_DISP_START_LINE = 0x40,
  SET_SEG_REMAP = 0xA0,
  SET_MUX_RATIO = 0xA8,
  SET_COM_OUT_DIR = 0xC0,
  SET_DISP_OFFSET = 0xD3,
  SET_COM_PIN_CFG = 0xDA,
  SET_DISP_CLK_DIV = 0xD5,
  SET_PRECHARGE = 0xD9,
  SET_VCOM_DESEL = 0xDB,
  SET_CHARGE_PUMP = 0x8D
} ssd1306_command_t;

typedef struct {
  uint8_t width, height, pages, address;
  i2c_inst_t *i2c_port;
  bool external_vcc;
  uint8_t *ram_buffer;
  size_t bufsize;
  uint8_t port_buffer[2];
} ssd1306_t;

void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
void ssd1306_config(ssd1306_t *ssd);
void ssd1306_command(ssd1306_t *ssd, uint8_t command);
void ssd1306_send_data(ssd1306_t *ssd);

void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value);
void ssd1306_fill(ssd1306_t *ssd, bool value);
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill);
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value);
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value);
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value);
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y);
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y);