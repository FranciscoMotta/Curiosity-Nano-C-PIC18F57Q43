/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 04 de mayo de 2023
 * 
 */

/*
 * Includes
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000UL
#endif


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "project_defines.h"
#include "system_config.h"

/*
 * Rutinas de servicio a interrpciones
 */

void __interrupt(irq(48)) External_Int1 (void)
{
    if(PIE6bits.INT1IE && PIR6bits.INT1IF)
    {
        Led_Sys_Tog();
        PIR6bits.INT1IF = 0; // Limpiamos la bandera
    }
}
/*
 * Declaracion de funciones
 */

void Init_Internal_Clock (void);
void Init_Pins_Application (void);
void Init_External_Int1 (void);
/*
 * Main
 */
int main(void) 
{
    Init_Internal_Clock(); // Iniciamos el reloj interno
    Init_Pins_Application(); // Iniciamos los pines de la aplicacion
    Init_External_Int1(); // Iniciamos las configuraciones de int1
    while(true)
    {
        Generat_Int_Tog();
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_External_Int1 (void)
{
    /* Configuramos los registros PIE y PIR */
    PIE6bits.INT1IE = 1; // Habilitamos el enable de int1
    PIR6bits.INT1IF = 0; // Limpiamos la bandera de int1
    
    /* Configuramos los registros generales INTCON0 e INTCON1 */
    INTCON0bits.IPEN = 0; // Desactivamos las prioridades
    INTCON0bits.GIE = 1;  // Activamos las interrupciones globales
    INTCON0bits.INT1EDG = 1; // Interrupcion en flanco de subida
}

void Init_Pins_Application(void)
{
    /* Se configura el LED */
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
    
    /* Se configura el generador de interrupciones */
    Generat_Int_Tris &= ~(1 << Generat_Int_Gpio);
    Generat_Int_Off();
    
    /* Configuramos la entrada de la int1 */
    Interrupt_Pin_Tris |= (1 << Interrupt_Pin_Gpio);
    Interrupt_Pin_Ansel &= ~(1 << Interrupt_Pin_Gpio);
}

void Init_Internal_Clock (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1; // Dividimos entre 1
    my_clock.frecuencia_clock = freq_clk_48MHZ; // Freq 48Mhz
    FM_Hfintosc_Init(&my_clock); // Pasamos los parámetros configurados
}