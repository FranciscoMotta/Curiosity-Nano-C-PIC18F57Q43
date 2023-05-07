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
#define _XTAL_FREQ 4000000UL
#endif

#include <xc.h>
#include <pic18.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_config.h"
#include "project_defines.h"

// #define FCY     (1 /_XTAL_FREQ)

/*
 * Rutinas de servicio a interrupciones
 */

void __interrupt(irq(IRQ_TMR0)) Timer_Interrupt (void)
{
    if(PIR3bits.TMR0IF && PIE3bits.TMR0IE)
    {
        Led_Sys_Tog();
        TMR0H = 0x0B;
        TMR0L = 0xDC;
        /* Se limpia la bandera */
        PIR3bits.TMR0IF = 0; 
    }
}

/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);
void Init_Gpio_System (void);
void Init_Interrupt_Timer0 (void);
void Init_Timer0 (void);

/*
 * Main
 */
int main(void) 
{
    Init_Internal_Oscillator();
    Init_Gpio_System();
    Init_Interrupt_Timer0();
    Init_Timer0();
    while(true)
    {
       __nop();
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_Interrupt_Timer0 (void)
{
    INTCON0bits.GIE  = 1; // Interrupciones globales on
    INTCON0bits.IPEN = 0; // Sin prioridades
    
    /* Registros del timer 0 */
    PIE3bits.TMR0IE = 1; // Interrupciones del timer 0 on
    PIR3bits.TMR0IF = 0; // Bandera de las interrupciones 0 off
}

void Init_Timer0 (void)
{
    T0CON0bits.EN    = 1; // Timer 1 enabled
    T0CON0bits.MD16  = 1; // Timer a 16 bits
    T0CON0bits.OUTPS = 0b0000; // postcaler 1:1
    T0CON1bits.CS    = 0b010; // Clock FOSC / 4
    T0CON1bits.ASYNC = 0; // Timer 0 sinc. FOSC / 4
    T0CON1bits.CKPS  = 0b0011; // Prescaler a 1:8
    
    /* Cargamos el valor a los registros */
    // DEC = 3036 -> HEX = 0x0BDC
    TMR0H = 0x0B;
    TMR0L = 0xDC;
}

void Init_Gpio_System (void)
{
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
}

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock    = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_4MHZ;
    
    /* Pasamos los parámetros */
    FM_Hfintosc_Init(&my_clock);
}