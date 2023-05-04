/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 05 de mayo de 2023
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

#include "system_config.h"
#include "project_defines.h"

/*
 * Funciones de servicio a interrupciones
 */

/*
 * Si colocamos: irq(default) se generan latencia,
 * debido a que se tiene que preguntar por todos los 
 * vectores de interrupción por cual gatilló la acción.
 * 
 * Si colocamos: irq(IRQ_INT0) no genera latencia debido
 * a que ya sabe a donde ir cuando se genere la interrupcion
 */
void __interrupt(irq(8)) External_Int0 (void)
{
    if(PIE1bits.INT0IE && PIR1bits.INT0IF)
    {
        Led_Sys_Tog(); // Hacemos el Toggle del led
        PIR1bits.INT0IF = 0; // Limpiamos la bandera
    }
}

/*
 * Declaracion de funciones
 */

void Local_Osc_Init (void);
void Pines_Config (void);
void Interrupts_Init (void);

/*
 * Main
 */
int main(void) 
{
    Local_Osc_Init(); // Iniciamos el reloj del sistema
    Pines_Config(); // Configuramos los pines del sistema
    Interrupts_Init(); // Configuramos las interrupciones
    while(1)
    {
        Generat_Int_Tog(); // Hacemos el toggle de generación de ints
        __delay_ms(300);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Local_Osc_Init (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1; // División entre 0
    my_clock.frecuencia_clock = freq_clk_48MHZ; // Freq de 48Mhz
    FM_Hfintosc_Init(&my_clock); // Pasamos los parámetros del clock
}


void Pines_Config (void)
{
    /* Led del sistema */
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
    
    /* Generador de interrupciones */
    Generat_Int_Tris &= ~(1 << Generat_Int_Gpio);
    Generat_Int_Off();
    
    /* Configuramos el pin de interrupciones */
    // Se coloca el pin como digital
    Interrupt_Pin_Ansel &= ~(1 << Interrupt_Pin_Gpio); 
    Interrupt_Pin_Tris |= (1 << Interrupt_Pin_Gpio);
}


void Interrupts_Init (void)
{
    /*
     * Notas:
     *  - Los registros IPR: Hacen referencia a los registros 
     * de control de las prioridades del las interrupciones 
     * del sistema.
     * 
     * - Los registros PIE: Hacen referencia a los registros 
     * de control de los Enables de las interrupciones del
     * sistema.
     * 
     * - Los registros PIR: Hacen referencia a los registros 
     * de  control de las Flags de las interrupciones del 
     * sistema.
     */
    
    PIE1bits.INT0IE = 1; // Habilitamos el int0
    PIR1bits.INT0IF = 0; // Limpiamos la bandera del int0
    
    /* Usamos los registros INTCON0 e INTCON1 */
    INTCON0bits.INT0EDG = 1; // Int0 se activa por flanco de subida
    
    /* Configuramos los parámetros generales de configuraciones */
    INTCON0bits.IPEN = 0; // Desabilitamos las prioridades
    INTCON0bits.GIE  = 1; // Habilitamos las interrupciones globales
    
}