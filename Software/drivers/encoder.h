#ifndef ENCODER_H
#define ENCODER_H

#include <stdbool.h>
#include <stdint.h>

void configure_encoder1 ( void );
_Bool get_encoder_1_A ( void );
_Bool get_encoder_1_B ( void );
_Bool get_encoder_1_button ( void );

void configure_encoder2 ( void );
_Bool get_encoder_2_A ( void );
_Bool get_encoder_2_B ( void );
_Bool get_encoder_2_button ( void );


#endif