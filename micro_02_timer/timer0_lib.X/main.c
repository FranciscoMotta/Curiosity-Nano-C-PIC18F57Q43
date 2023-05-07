/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 6 de mayo de 2023, 06:42 PM
 */

/*
 * Includes
 */

#include <xc.h>
#include <pic18.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "FM_Timer0.h"
#include "project_defines.h"
#include "system_config.h"

/*
 * Macros
 */

/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);
void Init_Timer_0 (void);
/*
 * Main
 */
int main(void) 
{
    Init_Internal_Oscillator();
    Init_Timer_0();
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_Timer_0 (void)
{
    _timer0_params_config_t my_timer0;
    my_timer0.timer0_bits = TMR0_16bits;
    my_timer0.timer0_clock_source = TMR0_Fosc_div_4;
    my_timer0.timer0_counter_sync = TMR0_Counter_Sync_FOSC4;
    my_timer0.timer0_enable = TMR0_Enable;
    my_timer0.timer0_prescaler = TMR0_Prescaler_1_8;
    my_timer0.timer0_postecaler = TMR0_Post_1_1;
    
    /* Se pasan los parámetros */
    FM_Hfintosc_Init(&my_timer0);
}

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_4MHZ;
    /* Se pasan los parámetros */
    FM_Hfintosc_Init(&my_clock);
}