/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 24 de abril de 2023
 */

/*
 * Includes
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "system_config.h"

/*
 * Macros
 */

#define Led_Sys_Gpio        3
#define Led_Sys_Tris        TRISF
#define Led_Sys_Lat         LATF

#define Rut_Led_On()        Led_Sys_Lat &= ~(1 << Led_Sys_Gpio);
#define Rut_Led_Off()        Led_Sys_Lat |= (1 << Led_Sys_Gpio);

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000UL
#endif

#define Percent_Limit_Fake_Pwm      25

/*
 * Declaracion de funciones
 */

void Clock_Local_Init (void);

/*
 * Main
 */
int main(void) 
{
    Clock_Local_Init(); // Iniciamos el reloj interno
    /* Led del sistema como salida */
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    /* Prendemos el LED */
    Led_Sys_Lat &= ~(1 << Led_Sys_Gpio);
    uint8_t counter = 0;
    uint8_t fake_pwm_max = 0;
    while(1)
    {
        if(counter <= fake_pwm_max)
        {
            Rut_Led_Off();
        }
        else 
        {
            Rut_Led_On();
        }
        if(counter++ == 100)
        {
            counter = 0;
            fake_pwm_max++;
            if(fake_pwm_max == Percent_Limit_Fake_Pwm) fake_pwm_max = 0;
        }
        __delay_ms(1);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Clock_Local_Init (void)
{
    _clock_hfintosc_params_t my_clock_param;
    my_clock_param.divisor_clock = clock_div_1;
    my_clock_param.frecuencia_clock = freq_clk_16MHZ;
    FM_Hfintosc_Init(&my_clock_param); // Pasamos los datos de conf.
}