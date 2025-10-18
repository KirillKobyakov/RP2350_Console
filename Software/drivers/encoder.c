#include <rp2350/resets.h>
#include <rp2350/sio.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pads_bank0.h>
#include "gpio_irq.h"
#include <stdbool.h>
#include <stdint.h>
#include "interrupt.h"

#define GPIO_RESETS (RESETS_RESET_IO_BANK0_MASK | RESETS_RESET_PADS_BANK0_MASK)

#define ENCODER_1_A      (1<<3)
#define ENCODER_1_B      (1<<4)
#define ENCODER_1_BUTTON (1<<5)

#define ENCODER_2_A (1<<7)
#define ENCODER_2_B (1<<8)
#define ENCODER_2_C (1<<9)

static _Bool enc_1_button_state;
static _Bool enc_2_button_state;

static void encoder1_button_callback ( void );
static void encoder2_button_callback ( void );

const uint8_t io_pads_configure =
    PADS_BANK0_GPIO0_OD(1)          | 
    PADS_BANK0_GPIO0_IE(1)          |
    PADS_BANK0_GPIO0_DRIVE(0)       |
    PADS_BANK0_GPIO0_PUE(1)         |
    PADS_BANK0_GPIO0_PDE(0)         |
    PADS_BANK0_GPIO0_SCHMITT(0)     |
    PADS_BANK0_GPIO0_SLEWFAST(0);

const uint8_t io_bank_configure = 
    IO_BANK0_GPIO0_CTRL_IRQOVER(0)  |
    IO_BANK0_GPIO0_CTRL_INOVER(0)   |
    IO_BANK0_GPIO0_CTRL_OEOVER(0)   |
    IO_BANK0_GPIO0_CTRL_OUTOVER(0)  |
    IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

void configure_encoder1 ( void ) {

    resets.reset_clr = GPIO_RESETS;
	while( !(resets.reset_done & GPIO_RESETS) )
		continue;

    pads_bank0.gpio3 = io_pads_configure;
    pads_bank0.gpio4 = io_pads_configure;
    pads_bank0.gpio5 = io_pads_configure;

    io_bank0.gpio3_ctrl = io_bank_configure;
    io_bank0.gpio4_ctrl = io_bank_configure;
    io_bank0.gpio5_ctrl = io_bank_configure;

    register_gpio_irq_callback(encoder1_button_callback);

    io_bank0.proc0_inte0 |=
        (IO_BANK0_PROC0_INTE0_GPIO5_EDGE_HIGH_MASK |
         IO_BANK0_PROC0_INTE0_GPIO5_EDGE_LOW_MASK );
}

_Bool get_encoder_1_A ( void ) {
    return (sio.gpio_in & ENCODER_1_A);
}

_Bool get_encoder_1_B ( void ) {
    return (sio.gpio_in & ENCODER_1_B);
}

_Bool get_encoder_1_button ( void ) {
    return enc_1_button_state;
}

static void encoder1_button_callback ( void ){
    uint8_t events = get_irq_status_for_pin(5);
    if( !events )
		return;
	if( events & IRQ_EDGE_HIGH )
		enc_1_button_state=true;
	if( events & IRQ_EDGE_LOW )
		enc_1_button_state=false;
}

void configure_encoder2 ( void ) {

    resets.reset_clr = GPIO_RESETS;
	while( !(resets.reset_done & GPIO_RESETS) )
		continue;

    pads_bank0.gpio7  = io_pads_configure;
    pads_bank0.gpio8  = io_pads_configure;
    pads_bank0.gpio9  = io_pads_configure;

    io_bank0.gpio7_ctrl  = io_bank_configure;
    io_bank0.gpio8_ctrl  = io_bank_configure;
    io_bank0.gpio9_ctrl  = io_bank_configure;

    register_gpio_irq_callback(encoder2_button_callback);

    io_bank0.proc0_inte1 |=
        (IO_BANK0_PROC0_INTE1_GPIO9_EDGE_HIGH_MASK |
         IO_BANK0_PROC0_INTE1_GPIO9_EDGE_LOW_MASK );
}

_Bool get_encoder_2_A ( void ) {
    return (sio.gpio_in & ENCODER_2_A);
}

_Bool get_encoder_2_B ( void ) {
    return (sio.gpio_in & ENCODER_2_B);
}

_Bool get_encoder_2_button ( void ) {
    return enc_2_button_state;
}

static void encoder2_button_callback ( void ){
    uint8_t events = get_irq_status_for_pin(9);
    if( !events )
		return;
	if( events & IRQ_EDGE_HIGH )
		enc_2_button_state=true;
	if( events & IRQ_EDGE_LOW )
		enc_2_button_state=false;
}