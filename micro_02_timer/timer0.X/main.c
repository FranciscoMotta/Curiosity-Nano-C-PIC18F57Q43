/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 6 de mayo de 2023, 01:32 PM
 */

/*
 * Includes
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000UL
#endif

#include <xc.h>
#include <pic18.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_config.h"
#include "project_defines.h"

/*
 * Macros
 */

/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);
void Init_Gpio_System (void);

/*
 * Main
 */
int main(void) 
{
    Init_Internal_Oscillator();
    Init_Gpio_System();
    while(true)
    {
        Led_Sys_Tog();
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_Gpio_System (void)
{
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
}

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_16MHZ;
    
    /* Pasamos los parámetros */
    FM_Hfintosc_Init(&my_clock);
}