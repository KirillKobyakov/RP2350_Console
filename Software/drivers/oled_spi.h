#ifndef OLED_SPI_H
#define OLED_SPI_H

#include <stdint.h>

void configure_oled_spi ( void );
void setup_oled ( void );
void update_display ( void );
void oled_clear ( void );
void set_pixel ( uint8_t row, uint8_t col);
void clear_pixel ( uint8_t row, uint8_t col);
void draw_box ( uint16_t cursor_x, uint16_t cursor_y, uint8_t cursor_size);
void clear_display ( void );

void draw_pixel (uint16_t cursor_x, uint16_t cursor_y); 
#endif
