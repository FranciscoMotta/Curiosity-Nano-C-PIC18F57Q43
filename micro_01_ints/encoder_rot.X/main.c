/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 5 de mayo de 2023, 06:59 PM
 */


#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000UL
#endif

/*
 * Includes
 */

#include <xc.h>
#include <pic18.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_config.h"
#include "project_defines.h"

/*
 * Variables globales
 */

uint8_t system_flags = 0x00;

/*
 * Rutinas de servicio a interrupciones
 */

void __interrupt(irq(IRQ_INT0)) External_Int (void)
{
    if(PIR1bits.INT0IF && PIE1bits.INT0IE)
    {
        if(Pin_Felt_Encoder_Port && (1 << Pin_Felt_Encoder_Gpio))
        {
            ClockWise_Led_Lat |= (1 << ClockWise_Led_Gpio);
            Anti_ClockWise_Led_Lat &= ~(1 << Anti_ClockWise_Led_Gpio);
        }
        else
        {
            ClockWise_Led_Lat &= ~(1 << ClockWise_Led_Gpio);
            Anti_ClockWise_Led_Lat |= (1 << Anti_ClockWise_Led_Gpio);
        }
        __delay_ms(25);
        PIR1bits.INT0IF = 0; // Limpiamos la bandera
    }
}

/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);
void Init_System_Gpio (void);
void Init_External_Int (void);

/*
 * Main
 */
int main(void) 
{
    Init_Internal_Oscillator();
    Init_System_Gpio();
    Init_External_Int();
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

void Init_External_Int (void)
{
    /* Configuramos los registros PIR y PIE */
    PIR1bits.INT0IF = 0; // Limpiamos la bandera
    PIE1bits.INT0IE = 1; // Habilitamos la int0
    
    /* Configuramos los registros generales */
    INTCON0bits.IPEN = 0; // Prioridades desactivadas
    INTCON0bits.GIE = 1; // Interrupciones globales activadas
    INTCON0bits.INT0EDG = 0; // Sensibilidad por falling edge
}

void Init_System_Gpio (void)
{
    /* Configuramos el led del sistema */
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
    
    /* Se configura la entrada de interrupcion */
    Interrupt_Pin_Ansel &= ~(1 << Interrupt_Pin_Gpio);
    Interrupt_Pin_Tris |= (1 << Interrupt_Pin_Gpio);
    
    /* Se configura la entrada del sentido */
    Pin_Felt_Encoder_Ansel &= ~(1 << Pin_Felt_Encoder_Gpio);
    Pin_Felt_Encoder_Tris |= (1 << Pin_Felt_Encoder_Gpio);
    
    /* Se configuran los leds del sistema */
    TRISF &= ~((1 << ClockWise_Led_Gpio) | (1 << Anti_ClockWise_Led_Gpio));
    LATF  &= ~((1 << ClockWise_Led_Gpio) | (1 << Anti_ClockWise_Led_Gpio)); 
}

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_16MHZ;
    
    /* Pasamos los parámetros */
    FM_Hfintosc_Init(&my_clock);
}