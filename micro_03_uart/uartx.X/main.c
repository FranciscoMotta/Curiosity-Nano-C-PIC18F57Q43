/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 7 de mayo de 2023, 05:00 PM
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

#include "project_defines.h"
#include "system_config.h"

/*
 * Macros
 */

#define _FOSC   _XTAL_FREQ
#define _BAUD_RATE_DES  9600
/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);
void Init_Global_Interrupt (void);
void Init_Gpio_System (void);
void Init_Uart3 (void);

/*
 * Main
 */
int main(void) 
{
    Init_Internal_Oscillator();
    Init_Global_Interrupt();
    Init_Gpio_System();
    Init_Uart3();
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

void Init_Uart3 (void)
{
    /* Limpiamos los registros */
    
    U3TXB = 0x00;
    U3RXB = 0x00;
    
    U3CON1 = 0x00;
    U3CON0 = 0x00;
    U3CON0 = 0x00;
    
    /* Configuramos  U3CON0 */
    U3CON0 &= ~(1 << _U3CON0_BRGS_POSITION); // Normal Speed
    U3CON0 &= ~(1 << _U3CON0_ABDEN_POSITION); // No autobaud
    U3CON0 |= (0b11 << _U3CON0_RXEN_POSITION); // TX and Rx enabled
    U3CON0 |= (0b0000 << _U3CON0_MODE0_POSITION); // 8bits no parity
    
    /* Configuramos U3CON1 */
    U3CON1 |= (1 << _U3CON1_ON_POSITION); // Serial port on
    U3CON1 &= ~(1 << _U3CON1_WUE_POSITION); // Receive normal op
    
    /* Configuramos U3BRG */
    
    uint16_t register_u3brg = 0;
    
    if (U3CON0 & (1 << _U3CON0_BRGS_POSITION))
    {
        register_u3brg = ((_FOSC)/(4 * _BAUD_RATE_DES)) - 1;
    }
    else
    {
        register_u3brg = ((_FOSC)/(16 * _BAUD_RATE_DES)) - 1;
    }
    U3BRG = register_u3brg;
}

void Init_Global_Interrupt (void)
{
    /* Se configura los registros INTCON0 */
    INTCON0 &= ~(1 << _INTCON0_IPEN_POSITION); // Sin prioridades
    INTCON0 |= (1 << _INTCON0_GIE_POSITION); // Interrupciones gloables activadas
    
    /* Se configura los registros PIE9 y PIR9 */
    PIE9 &= ~(1 << _PIE9_U3TXIE_POSITION); // Desahilitamos interrupción por transm
    PIE9 &= ~(1 << _PIE9_U3RXIE_POSITION); // Dehabilitamos interrupción por recep
    
    PIR9 &= ~(1 << _PIR9_U3TXIF_POSITION); // Limpiamos bandera de transm
    PIR9 &= ~(1 << _PIR9_U3RXIF_POSITION); // Limpiamos bandera de recep
}

void Init_Gpio_System (void)
{
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
}

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_4MHZ;
    /* Pasamos los parámetros */
    FM_Hfintosc_Init(&my_clock);
}