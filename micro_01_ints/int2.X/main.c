/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 4 de mayo de 2023, 08:10 PM
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

#include "system_config.h"
#include "project_defines.h"

/*
 * Rutinas de servicio a interrupciones
 */

void __interrupt(irq(80)) External_Int2 (void)
{
    if(PIR10bits.INT2IF && PIE10bits.INT2IE)
    {
        Led_Sys_Tog();
        /* Se limpia la bandera */
        PIR10bits.INT2IF = 0; 
    }
}

/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);
void Init_Gpio_Application (void);
void Init_External_Interrupt (void);

/*
 * Main
 */
int main(void) 
{
    Init_Internal_Oscillator(); // Conf. clock
    Init_Gpio_Application(); // Conf. Gpio
    Init_External_Interrupt(); // Conf. Ints
    while(true)
    {
        Generat_Int_Tog();
        __delay_ms(10);                    
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_External_Interrupt (void)
{
    /* Configurar PIR10 y PIE10 */
    PIR10bits.INT2IF = 0; // Limpiamos la bandera
    PIE10bits.INT2IE = 1; // Habilitamos la interrupcion
    
    /* Configuramos los registros generales INTCON0 e INTCON1 */
    INTCON0bits.IPEN = 1; // Prioridades desactivadas
    INTCON0bits.GIE = 1; // Interrupciones globales activadas
    INTCON0bits.INT2EDG = 0; // Int2 activada por flanco de bajada
}

void Init_Gpio_Application (void)
{
    /* Configurar RF3 como salida */
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
    
    /* Configurar RC3 como salida */
    Generat_Int_Tris &= ~(1 << Generat_Int_Gpio);
    Generat_Int_Off();
    
    /* Configurar RB2 como entrada */
    Interrupt_Pin_Ansel &= ~(1 << Interrupt_Pin_Gpio);
    Interrupt_Pin_Tris |= (1 << Interrupt_Pin_Gpio);
}

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1; // clock / 1
    my_clock.frecuencia_clock = freq_clk_48MHZ; // clock = 48Mhz
    /* Se pasa la configuracion */
    FM_Hfintosc_Init(&my_clock);
}



