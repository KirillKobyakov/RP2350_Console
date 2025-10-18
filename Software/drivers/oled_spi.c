// OLED Screen Driver which uses SPI and specific commands to either perform    
// certain changes to the display or send data
#include "oled_spi.h"
#include "systick.h"
#include "dma_driver.h"
#include <rp2350/spi.h>
#include <rp2350/resets.h>
#include <rp2350/sio.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pads_bank0.h>
#include <rp2350/clocks.h>
#include <stdbool.h>
#include <stdint.h>

static void set_data_mode ( void );
static void set_command_mode ( void );
static void write_oled (uint8_t input);
static void oled_send_data(uint8_t d);
static void oled_send_command(uint8_t c);
static void oled_send_command_with_argument(uint8_t c, uint8_t a);


#define SPI1_RESETS  ( RESETS_RESET_SPI1_MASK )
#define GPIO_RESETS ( RESETS_RESET_IO_BANK0_MASK | \
                      RESETS_RESET_PADS_BANK0_MASK )
#define RESETS_OLED (SPI1_RESETS | GPIO_RESETS )

#define MAX_X_PIXEL 240
#define MAX_Y_PIXEL 320


static uint8_t display_RAM [8][128] = { 0 };


void configure_oled_spi ( void ) {
    clocks.clk_peri_ctrl = CLOCKS_CLK_PERI_CTRL_ENABLE(1) |
                           CLOCKS_CLK_PERI_CTRL_AUXSRC(0); // used to be 4


    resets.reset_clr = RESETS_OLED;
    while( (resets.reset_done & RESETS_OLED) != RESETS_OLED);


    const uint32_t pads_bank0_gpio_out =
        PADS_BANK0_GPIO0_OD(0)      |
        PADS_BANK0_GPIO0_IE(0)      |
        PADS_BANK0_GPIO0_DRIVE(0)   |
        PADS_BANK0_GPIO0_PUE(0)     |
        PADS_BANK0_GPIO0_PDE(0)     |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0) ;


    const uint32_t io_bank0_gpio_ctrl =
        IO_BANK0_GPIO0_CTRL_IRQOVER(0) |
        IO_BANK0_GPIO0_CTRL_INOVER(0)  |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)  |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0) |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(1);


    const uint32_t io_bank0_gpio_ctrl_dc =
        IO_BANK0_GPIO0_CTRL_IRQOVER(0) |
        IO_BANK0_GPIO0_CTRL_INOVER(0)  |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)  |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0) |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(5);


    pads_bank0.gpio11 = pads_bank0_gpio_out;
    pads_bank0.gpio12 = pads_bank0_gpio_out;
    pads_bank0.gpio13 = pads_bank0_gpio_out;
    pads_bank0.gpio14 = pads_bank0_gpio_out;
    pads_bank0.gpio15 = pads_bank0_gpio_out;


    io_bank0.gpio11_ctrl = io_bank0_gpio_ctrl_dc;
    io_bank0.gpio12_ctrl = io_bank0_gpio_ctrl_dc;
    io_bank0.gpio13_ctrl = io_bank0_gpio_ctrl;
    io_bank0.gpio14_ctrl = io_bank0_gpio_ctrl;
    io_bank0.gpio15_ctrl = io_bank0_gpio_ctrl;


    spi1.sspcr0 =
        SPI1_SSPCR0_SCR(0) |
        SPI1_SSPCR0_SPH(1) |
        SPI1_SSPCR0_SPO(1) |
        SPI1_SSPCR0_FRF(0) |
        SPI1_SSPCR0_DSS(7);

    // Clock divider set to 10 to see on oscilloscope, could be 4 for the screen TODO
    spi1.sspcpsr = 4;


    spi1.sspcr1 =
        SPI1_SSPCR1_SOD(0) |
        SPI1_SSPCR1_MS(0)  |
        SPI1_SSPCR1_SSE(0) |
        SPI1_SSPCR1_LBM(0);


    spi1.sspcr1_set = SPI1_SSPCR1_SSE(1);


    sio.gpio_oe_set = (1<<12) | (1<<11);
}


static void set_data_mode ( void ) {
    sio.gpio_out_set = (1<<12);
}


static void set_command_mode ( void ) {
    sio.gpio_out_clr = (1<<12);
}


static void write_oled (uint8_t input) {
  //  while((spi1.sspsr & 1<<1) == 0);
    spi1.sspdr = input & 0xff;
    while(spi1.sspsr & 1<<4);
}


static void oled_send_data(uint8_t d) {
    set_data_mode();
    write_oled(d);
}


static void oled_send_command(uint8_t c) {
    set_command_mode();
    write_oled(c);
}


static void oled_send_command_with_argument(uint8_t c, uint8_t a) {
    oled_send_command(c);
    oled_send_data(a);
}


void setup_oled ( void ) {
    sio.gpio_out_clr = (1<<11); 


    while ( !system_tick() );


    while ( !system_tick() );


    sio.gpio_out_set = (1<<11); 


    while ( !system_tick() );
    
    oled_send_command_with_argument(0x36, 0x00);
    oled_send_command_with_argument(0x3A, 0x05);

    oled_send_command(0xB2);
    oled_send_data(0x0B);
    oled_send_data(0x0B);
    oled_send_data(0x00);
    oled_send_data(0x33);
    oled_send_data(0x35);

    oled_send_command_with_argument(0xB7, 0x11);

    oled_send_command_with_argument(0xBB, 0x35);

    oled_send_command_with_argument(0xC0, 0x2C);

    oled_send_command_with_argument(0xC2, 0x01);

    oled_send_command_with_argument(0xC3, 0x0D);
 
    oled_send_command_with_argument(0xC4, 0x20);

    oled_send_command_with_argument(0xC6, 0x13);

    oled_send_command(0xD0);
    oled_send_data(0xA4);
    oled_send_data(0xA1);

    oled_send_command_with_argument(0xD6, 0xA1);

    oled_send_command(0xE0);
    oled_send_data(0xF0);
    oled_send_data(0x06);
    oled_send_data(0x0B);
    oled_send_data(0x0A);
    oled_send_data(0x09);
    oled_send_data(0x26);
    oled_send_data(0x29);
    oled_send_data(0x33);
    oled_send_data(0x41);
    oled_send_data(0x18);
    oled_send_data(0x16);
    oled_send_data(0x15);
    oled_send_data(0x29);
    oled_send_data(0x2D);

    oled_send_command(0xE1);
    oled_send_data(0xF0);
    oled_send_data(0x04);
    oled_send_data(0x08);
    oled_send_data(0x08);
    oled_send_data(0x07);
    oled_send_data(0x03);
    oled_send_data(0x28);
    oled_send_data(0x32);
    oled_send_data(0x40);
    oled_send_data(0x3B);
    oled_send_data(0x19);
    oled_send_data(0x18);
    oled_send_data(0x2A);
    oled_send_data(0x2E);

    oled_send_command(0xE4);
    oled_send_data(0x25);
    oled_send_data(0x00);
    oled_send_data(0x00);

    oled_send_command(0x21);

    oled_send_command(0x11);
    while( !system_tick() );
    oled_send_command(0x29);

}

void draw_pixel (uint16_t cursor_x, uint16_t cursor_y) {
  oled_send_command(0x2A);
  oled_send_data(cursor_x >> 8);
  oled_send_data(cursor_x & 0xFF);
  oled_send_data(cursor_x >> 8);
  oled_send_data(cursor_x && 0xFF);

  oled_send_command(0x2B);
  oled_send_data(cursor_y >> 8);
  oled_send_data(cursor_y & 0xFF);
  oled_send_data(cursor_y >> 8);
  oled_send_data(cursor_y && 0xFF);



  oled_send_command(0x2C);
  oled_send_data(0xFF);
  oled_send_data(0xFF);
}

void draw_box ( uint16_t cursor_x, uint16_t cursor_y, uint8_t cursor_size) {
  uint16_t box_x_low = (cursor_x - cursor_size) >= 0 ? (cursor_x - cursor_size) : 0;
  uint16_t box_x_hi  = (cursor_x + cursor_size) <= MAX_X_PIXEL ? (cursor_x + cursor_size) : MAX_X_PIXEL;

  uint16_t box_y_low = (cursor_y - cursor_size) >= 0 ? (cursor_y - cursor_size) : 0;
  uint16_t box_y_hi  = (cursor_y + cursor_size) <= MAX_Y_PIXEL ? (cursor_y + cursor_size) : MAX_Y_PIXEL;

  // Column Set - CASET(0x2A)
  oled_send_command(0x2A);
  oled_send_data((box_x_low >> 8)); // Top 8 bits of low x bound
  oled_send_data((box_x_low & 0xFF)); // Bottom 8 bits of low x bound
  oled_send_data((box_x_hi >> 8)); // Top 8 bits of high x bound
  oled_send_data((box_x_hi & 0xFF)); // Bottom 8 bits of high x bound
  
  // Row Set - RASET(0x2B)
  oled_send_command(0x2B);
  oled_send_data((box_y_low >> 8)); // Top 8 bits of low y bound
  oled_send_data((box_y_low & 0xFF)); // Bottom 8 bits of low y bound
  oled_send_data((box_y_hi >> 8)); // Top 8 bits of high y bound
  oled_send_data((box_y_hi & 0xFF)); // Bottom 8 bits of high y bound
  
  // Get number of pixel data to send
  uint16_t cols = box_x_hi - box_x_low;
  uint16_t rows = box_y_hi - box_y_low;
  uint32_t num  = cols * rows;

  // RAM write - RAMWR(0x2C)
  oled_send_command(0x2C);
  while (num > 0) {
    oled_send_data(0xFC);
    oled_send_data(0x3F);
    num--;
  }
}

void clear_display ( void ) {
  // Set Columns Limits
  oled_send_command(0x2A);
  oled_send_data(0x00);
  oled_send_data(0x00);
  oled_send_data(MAX_X_PIXEL >> 8);
  oled_send_data(MAX_X_PIXEL & 0xFF);

  // Set Row limits
  oled_send_command(0x2B);
  oled_send_data(0x00);
  oled_send_data(0x00);
  oled_send_data(MAX_Y_PIXEL >> 8);
  oled_send_data(MAX_Y_PIXEL & 0xFF);

  uint32_t pixels = MAX_X_PIXEL * MAX_Y_PIXEL;
  oled_send_command(0x2C);
  while (pixels > 0) {
    oled_send_data(0x00);
    oled_send_data(0x00);
    pixels--;
  }
  //oled_send_command(0x00);
}

void update_display ( void ) {
    for (uint8_t page = 0; page < 8; page++) {
        oled_send_command(0xB0 | page);       
        oled_send_command(0x02);               
        oled_send_command(0x10);              


        for (uint8_t col = 0; col < 128; col++) {
            oled_send_data(display_RAM[page][col]); 
        }
    }
}


void oled_clear ( void ) {
    memset ( display_RAM, 0, 8*128);
    update_display();
}


void set_pixel ( uint8_t row, uint8_t col)
{
    if (col >= 128 || row >= 64)
        return;
    display_RAM[row/8][col] |= 1<<(row & 0x7);

  
}


void clear_pixel ( uint8_t row, uint8_t col )
{
    if (col >= 128 || row >= 64)
        return;
    display_RAM[row/8][col] &= ~(1<<(row & 0x7));
}

