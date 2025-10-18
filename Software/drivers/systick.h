// Programming Assignment 5 - Kirill Kobyakov
// Header file for the interrupt systick driver which has the function defintions

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdbool.h>
#include <stdint.h>

void configure_systick(uint32_t systick_period_us);
_Bool system_tick();
void systick_handler();
_Bool register_systick_callback( void (*p_fn)() );
uint64_t get_system_time();

#endif
