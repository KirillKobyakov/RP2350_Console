#include "oled_spi.h"
#include "systick.h"
#include <interrupt.h>
#include "led.h"
#include "encoder.h"

#define LOOP_PERIOD_US 1000

typedef enum { BOTH_HIGH, NO_HIGH, A_HIGH, B_HIGH} enc_state_type;
typedef enum { SKETCH, COLOR, SIZE } paint_mode_type;


uint8_t encoder_1_counter ( void );
uint8_t encoder_2_counter ( void );

void main ( void ) {
    
    __disable_irq();
    configure_systick(LOOP_PERIOD_US);
    configure_oled_spi();
    configure_led();
    configure_encoder1();
    configure_encoder2();
    __enable_irq();

    setup_oled();
    turn_on_led();
   clear_display();

    uint16_t valx, valy;
    _Bool enc_1_button_state, enc_1_button_state_reg;
    _Bool enc_2_button_state, enc_2_button_state_reg;
    paint_mode_type app_state = SKETCH;

    while(1) {

        if ( !system_tick() )
            continue;


        enc_1_button_state = get_encoder_1_button(); 
        enc_2_button_state = get_encoder_2_button();

        valx = encoder_1_counter();
        valy = encoder_2_counter();

        switch(app_state) {
          case SKETCH:
            draw_box(valx, valy, 2);
            break;
          case COLOR: // TODO: Implement way to just track encoder turns
            break;
          case SIZE: // TODO: Implement way to just track encoder turns
            break;
          default:
            break;
        }
        if()
        {
            clear_display();
        } else {
            draw_box(valx, valy, 2);  
//            draw_pixel(valx, valy);
        }

        enc_1_button_state_reg = enc_1_button_state;
        enc_2_button_state_reg = enc_2_button_state;
    }
}

uint8_t encoder_1_counter ( void ) {

    static _Bool enc_1_a_state = 0;
    static _Bool enc_1_b_state = 0;
    static uint16_t enc_1_count = 120;

    static enc_state_type state = BOTH_HIGH;
    static enc_state_type next_state;

    enc_1_a_state = get_encoder_1_A();
    enc_1_b_state = get_encoder_1_B();

    switch (state)
    {
        case BOTH_HIGH:
            if ( enc_1_b_state == 0 && enc_1_a_state == 1)
            {
                enc_1_count = ((enc_1_count+1) < 240) ? (enc_1_count + 1) : 240;
                next_state = A_HIGH;
            }
            else if ( enc_1_b_state == 1 && enc_1_a_state == 0)
            {
                enc_1_count = ((enc_1_count-1) > 0) ? (enc_1_count - 1) : 0;
                next_state = B_HIGH;
            }
        break;
        case A_HIGH:
            if ( enc_1_b_state == 0 && enc_1_a_state == 0)
            {
                next_state = NO_HIGH;
            }
            else if (enc_1_b_state == 1 && enc_1_a_state == 1)
            {
                next_state = BOTH_HIGH;
            }
        break;
        case B_HIGH:
            if ( enc_1_b_state == 1 && enc_1_a_state == 1)
            {
                next_state = BOTH_HIGH;
            }
            else if (enc_1_b_state == 0 && enc_1_a_state == 0)
            {
                next_state = NO_HIGH;
            }
        break;
        case NO_HIGH:
            if ( enc_1_b_state == 1 && enc_1_a_state == 0)
            {
                next_state = B_HIGH;
            }
            else if (enc_1_b_state == 0 && enc_1_a_state == 1)
            {
                next_state = A_HIGH;
            }
        break;
    }

    state = next_state;
    return enc_1_count;
}


uint8_t encoder_2_counter ( void ) {

    static _Bool enc_2_a_state = 0;
    static _Bool enc_2_b_state = 0;
    static uint16_t enc_2_count = 160;

    static enc_state_type state = BOTH_HIGH;
    static enc_state_type next_state;

    enc_2_a_state = get_encoder_2_A();
    enc_2_b_state = get_encoder_2_B();

    switch (state)
    {
        case BOTH_HIGH:
            if ( enc_2_b_state == 0 && enc_2_a_state == 1)
            {
                enc_2_count = ((enc_2_count+1) < 320) ? (enc_2_count + 1) : 320;
                next_state = A_HIGH;
            }
            else if ( enc_2_b_state == 1 && enc_2_a_state == 0)
            {
                enc_2_count = ((enc_2_count-1) > 0) ? (enc_2_count - 1) : 0;
                next_state = B_HIGH;
            }
        break;
        case A_HIGH:
            if ( enc_2_b_state == 0 && enc_2_a_state == 0)
            {
                next_state = NO_HIGH;
            }
            else if (enc_2_b_state == 1 && enc_2_a_state == 1)
            {
                next_state = BOTH_HIGH;
            }
        break;
        case B_HIGH:
            if ( enc_2_b_state == 1 && enc_2_a_state == 1)
            {
                next_state = BOTH_HIGH;
            }
            else if (enc_2_b_state == 0 && enc_2_a_state == 0)
            {
                next_state = NO_HIGH;
            }
        break;
        case NO_HIGH:
            if ( enc_2_b_state == 1 && enc_2_a_state == 0)
            {
                next_state = B_HIGH;
            }
            else if (enc_2_b_state == 0 && enc_2_a_state == 1)
            {
                next_state = A_HIGH;
            }
        break;
    }

    state = next_state;
    return enc_2_count;
}
