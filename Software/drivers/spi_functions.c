#include "spi_functions.h"
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


// Used to configure the clock used 
void spi0_config_clock ();
void spi1_config_clock ();


#define SPI1_RESETS  ( RESETS_RESET_SPI1_MASK )
#define GPIO_RESETS ( RESETS_RESET_IO_BANK0_MASK | \
                      RESETS_RESET_PADS_BANK0_MASK )
#define RESETS_OLED (SPI1_RESETS | GPIO_RESETS )



void spi1_config_clock () {
  clocks.clk_peri_ctrl = CLOCKS_CLK_PERI_CTRL_ENABLE(1) |
                         CLOCKS_CLK_PERI_CTRL_AUXSRC(2); // USB CLK - 48 MHz
  
  resets.reset_clr = RESETS_OLED;
  while ( (resets.reset_done & RESETS_OLED) != RESETS_OLED);

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





}
