// SPI Drivers for the ST7789V2 IC
// Using the 4 line serial interface
//  - 1 line is for specifying if the data is a command or data
//  - 1 line is the actually command or data value
//
#include "st7789v2.h"
#include "systick.h"
#include <rp2350/spi.h>
#include <rp2350/resets.h>
#include <rp2350/sio.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pads_bank0.h>
#include <rp2350/clocks.h>
#include <stdbool.h>
#include <stdint.h>


static void set_date_mode    ( void );
static void set_command_mode ( void );

#define SPI1_RESETS ( RESETS_RESET_SPI1_MASK )
#define GPIO_RESETS ( RESETS_RESET_IO_BANK0_MASK | \
                      RESETS_RESET_PADS_BANK0_MASK )
#define LCD_RESETS  ( SPI1_RESETS | GPIO_RESETS )


// TODO - add in display RAM structure

const uint32_t pads_bank0_gpio_out =
  PADS_BANK0_GPIO0_ISO(0)      |
  PADS_BANK0_GPIO0_OD(0)       |
  PADS_BANK0_GPIO0_IE(0)       |
  PADS_BANK0_GPIO0_DRIVE(0)    |
  PADS_BANK0_GPIO0_PUE(0)      |
  PADS_BANK0_GPIO0_PDE(0)      |
  PADS_BANK0_GPIO0_SCHMITT(0)  |
  PADS_BANK0_GPIO0_SLEWFAST(0);
