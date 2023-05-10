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
#define _BAUD_RATE_DES  9600UL
/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);
void Init_Global_Interrupt (void);
void Init_Gpio_System (void);
void Init_Uart3 (void);
void Uart3_Tx_Byte (char dato);
void Uart3_TX_String (char *string);

/*
 * Main
 */
int main(void) 
{
    /* Se configura el reloj interno */
    Init_Internal_Oscillator();
    /* Se configuran las interrupciones */
    Init_Global_Interrupt();
    /* Se configura el gpio del sistema */
    Init_Gpio_System();
    /* Se configura el UART3 */
    Init_Uart3();
    /* Se configura los PPS (Periferal Pin Select) */
    // Considerando que RF1 -> Rx3 y RF0 -> Tx3
    // PPS Entrada
    U3RXPPS = 0x29; // Conectado a RF1 UART3 RX
    // PPS Salida
    RF0PPS = 0x26; // Conectado a RF0 UART3 TX
    Uart3_TX_String("Hola! - UART3 - PIC18F57Q43 \n\r");
    while(true)
    {
        Led_Sys_Tog();
        __delay_ms(250);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Uart3_TX_String (char *string)
{
    uint16_t string_index = 0;
    while(string[string_index] != '\0')
    {
        Uart3_Tx_Byte(string[string_index]);
        string_index ++;
    }
}

void Uart3_Tx_Byte (char dato)
{
    U3TXB = dato;
    while(!(PIR9 & (1 << _PIR9_U3TXIF_POSITION)));
}

void Init_Uart3 (void)
{
    /* Limpiamos los registros */
    
    U3TXB = 0x00;
    U3RXB = 0x00;
    
    U3CON0 = 0x00;
    U3CON1 = 0x00;
    
    /* Configuramos  U3CON0 */
    U3CON0 &= ~(1 << _U3CON0_BRGS_POSITION); // Normal Speed
    U3CON0 &= ~(1 << _U3CON0_ABDEN_POSITION); // No autobaud
    U3CON0 |= (1 << _U3CON0_TXEN_POSITION); // Tx enabled
    U3CON0 |= (1 << _U3CON0_RXEN_POSITION); // Rx enabled
    U3CON0 |= (0b0000 << _U3CON0_MODE0_POSITION); // 8bits no parity
    
    /* Configuramos U3CON1 */
    U3CON1 |= (1 << _U3CON1_ON_POSITION); // Serial port on
    U3CON1 &= ~(1 << _U3CON1_WUE_POSITION); // Receive normal op
    
    /* Se configuran el U3CON2 */
    //U3CON2 |= (1 << _U3CON2_TXPOL_POSITION); // 0 en idle
    
    /* Codigo de cálculo del valor del registros U3BRG */
    
    if(U3CON0 & (1 << _U3CON0_BRGS_POSITION))
    {
        /* Modo alta velocidad */
        U3BRG = (((_FOSC)/(4 * _BAUD_RATE_DES)) - 1);
    }
    else 
    {
        /* Modo normal */
        U3BRG = (((_FOSC)/(16 * _BAUD_RATE_DES)) - 1);
    }
    
    /* Configuramos U3BRG */
    //U3BRG  = 25; // <-- SI FUNCIONA!! 
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
    
    /* Se configura los pines del UART3 */
    UART3_TX_TRIS &= ~(1 << UART3_TX_GPIO);
    UART3_TX_LAT &= ~(1 << UART3_TX_GPIO);
    
    UART3_RX_ANSEL &= ~(1 << UART3_RX_GPIO); // Pin digital
    UART3_RX_TRIS |= (1 << UART3_RX_GPIO);
}

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_4MHZ;
    /* Pasamos los parámetros */
    FM_Hfintosc_Init(&my_clock);
}