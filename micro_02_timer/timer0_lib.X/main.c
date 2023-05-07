/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 6 de mayo de 2023, 06:42 PM
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000UL
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

#include "FM_Timer0.h"
#include "project_defines.h"
#include "system_config.h"

/*
 * Rutina de servicio a interrupciones
 */

void __interrupt(irq(IRQ_TMR0)) Timer_Interrupt(void) 
{
    if ((PIE3 & (1 << _PIE3_TMR0IE_POSITION)) && (PIR3 & (1 << _PIR3_TMR0IF_POSITION))) 
    {
        TMR0H = 0x0B; // Bits MSB
        TMR0L = 0xDC; // Bits LSB

        Led_Sys_Tog();

        PIR3 &= ~(1 << _PIR3_TMR0IF_POSITION); // Limpiamos la bandera
    }
}

/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator(void);
void Init_Timer_0(void);
void Init_Gpio_System(void);
void Init_Timer_Interrupts(void);

/*
 * Main
 */

int main(void) 
{
    /* Se configura el oscilador interno */
    Init_Internal_Oscillator();
    /* Se configura las interrupciones del timer */
    Init_Timer_Interrupts();
    /* Se configura el timer 0 */
    Init_Timer_0();
    /* Se configura los leds del sistema */
    Init_Gpio_System();

    /* Bucle */
    while (true) 
    {
        __nop();
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_Timer_Interrupts(void) 
{
    /* Registro INTCON0 */
    INTCON0 &= ~(1 << _INTCON0_IPEN_POSITION); // Deshabilitamos prioridades
    INTCON0 |= (1 << _INTCON0_GIE_POSITION); // Interrupciones globales ON

    /* Configuramos los registros PIEX y PIRX */
    PIE3 |= (1 << _PIE3_TMR0IE_POSITION); // Habilitamos int. timer 0
    PIR3 &= (1 << _PIR3_TMR0IF_POSITION); // Apagamos la bandera
}

void Init_Gpio_System(void) 
{
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
}

void Init_Timer_0(void) 
{
    _timer0_params_config_t my_timer0;
    my_timer0.timer0_enable = TMR0_Enable;
    my_timer0.timer0_bits = TMR0_16bits;
    my_timer0.timer0_clock_source = TMR0_Fosc_div_4;
    my_timer0.timer0_counter_sync = TMR0_Counter_Sync_FOSC4;
    my_timer0.timer0_prescaler = TMR0_Prescaler_1_8;
    my_timer0.timer0_postecaler = TMR0_Post_1_1;

    /* Calculamos el valor del registro TMR0H :
     * Para lo cual consideramos lo siguiente:
     * Prescaler = 1:8
     * FOSC = 4MHZ -> TCY = 4 / FOSC
     * Tiempo = 500ms
     * Timer = 16bits
     * 
     * tiempo = TCY*Prescaler*(65536 - carga)
     * 
     * carga =  65536 - ((Tiempo)/((4/FOSC) * Prescaler))
     * carga = 65536 - ((500ms)/((4 / 4Mhz) * 8))
     * carga = 65536 - ((500*10^-3)/((4 / 4*10^6) * 8))
     * carga = 3036 DEC
     * 
     * Carga = 0x0BDC hex 
     */
    
    TMR0H = 0x0B; // Bits MSB
    TMR0L = 0xDC; // Bits LSB
    
    /* Se pasan los parámetros */
    FM_Timer0_Init(&my_timer0);
}

void Init_Internal_Oscillator(void) 
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_4MHZ;
    /* Se pasan los parámetros */
    FM_Hfintosc_Init(&my_clock);
}