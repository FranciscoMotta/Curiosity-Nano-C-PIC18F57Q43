/* 
 * File:   newmain.c
 * Author: jmott
 *
 * Created on 22 de mayo de 2023, 09:35 AM
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

#define dms(x)  __delay_ms(x)

/*
 * Declaracion de funciones
 */

void FM_Internal_Oscillator_Config (void);
void FM_Gpio_Config (void);
/*
 * Main
 */

int main(void) 
{
    FM_Internal_Oscillator_Config();
    FM_Gpio_Config();
    while(true)
    {
        Led_Sys_Tog();
        dms(150);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void FM_Gpio_Config (void)
{
    /* Se configura el led como salida */
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    /* Se apaga el LED */
    Led_Sys_Off();
}

void FM_Internal_Oscillator_Config (void)
{
    _clock_hfintosc_params_t my_clock;
    
    /* Configuramos el reloj interno */
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_4MHZ;
    
    /* Pasamos los parámetros de config */
    FM_Hfintosc_Init(&my_clock);
}