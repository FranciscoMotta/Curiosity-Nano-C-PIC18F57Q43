/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 8 de mayo de 2023, 06:54 PM
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

#include "project_defines.h"
#include "system_config.h"

/*
 * Macros
 */

/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);

/*
 * Main
 */
int main(void) 
{
    Init_Internal_Oscillator();
    while(true)
    {
        
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_4MHZ;
    
    /* Pasamos los parámetros */
    FM_Hfintosc_Init(&my_clock);
}