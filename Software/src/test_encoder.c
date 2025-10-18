#include "encoder.h"
#include "led.h"
#include "systick.h"
#include "usbcdc.h"
#include <interrupt.h>

#define LOOP_PERIOD_US 1000


void main ( void ) {
    
    __disable_irq();
    configure_systick(LOOP_PERIOD_US);
    configure_led();
    configure_encoder1();
    configure_usbcdc();
    __enable_irq();


    uint8_t cntr = 0;
    uint16_t print_cntr = 0;
    uint8_t last_val = 0;

	while(1)
	{
		__asm__("WFI");
		if( !system_tick() )
			continue;

        uint8_t val = get_encoder_1_A();

        if ( val == 1)
            turn_on_led();
        else
            turn_off_led();
        
        if(print_cntr++ >= 0x0100) {
            uint8_t val = get_encoder_1_A();
                usbcdc_putchar(val+48);

            if (last_val != val){
                usbcdc_putchar('\n');
                usbcdc_putchar('\r');
            }
            print_cntr = 0;
            last_val = val;
        }
	}
    return;
}



