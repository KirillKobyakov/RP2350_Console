// Programming Assignment 5 - Kirill Kobyakov
// Includes a driver for systick which is interrupt based
// Has callbacks to toggle an LED every 500 systicks and a callback to 
// update the systime

#include "systick.h"
#include "interrupt.h"
#include <stdbool.h>
#include <stdint.h>
#include <rp2350/m33.h>
#include <rp2350/ticks.h>

#ifndef NUM_CALLBACKS
#define NUM_CALLBACKS 5
#endif

#define USE_SYSTICK_IRQ 1

static void (*callback[NUM_CALLBACKS])();
static void set_system_time();

static _Bool systick_has_fired;
static uint32_t num_callbacks;
static uint64_t system_time;

void configure_systick(uint32_t systick_period_us)
{
	register_systick_callback( set_system_time );
	
	ticks.proc0_ctrl_set = TICKS_PROC0_CTRL_ENABLE_MASK;
	ticks.proc0_cycles = 1;
	m33.syst_rvr= systick_period_us;
	m33.syst_cvr=0;
	m33.shpr3 = (m33.shpr3 & ~M33_SHPR3_PRI_15_3_MASK ) 
		| M33_SHPR3_PRI_15_3(0);

	m33.syst_csr = M33_SYST_CSR_CLKSOURCE(0)
	             | M33_SYST_CSR_ENABLE_MASK
				 | M33_SYST_CSR_TICKINT(USE_SYSTICK_IRQ);
	
}

_Bool system_tick()
{
	_Bool retval = systick_has_fired;
	systick_has_fired = false;
	return retval;
}

void __attribute__((interrupt)) SYSTICK_Handler()
{
	systick_has_fired = true;
	for(uint32_t i=0; i<num_callbacks; i++)
		callback[i]();
}

_Bool register_systick_callback( void (*p_fn)() )
{
	if( NUM_CALLBACKS == num_callbacks )
		return false;
	callback[num_callbacks++]=p_fn;
	return true;

}

static void set_system_time()
{
	system_time++;
}

uint64_t get_system_time()
{
	uint64_t retval;
	__disable_irq();
	retval = system_time;
	__enable_irq();
	return retval;
}