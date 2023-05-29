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
void Init_ADC_Module (void);
void FM_Gpio_Config (void);
/*
 * Main
 */

int main(void) 
{
    /* Configuramos el oscilador interno */
    FM_Internal_Oscillator_Config();
    /* Configuramos el GPIO del sistema */
    FM_Gpio_Config();
    /* Configuramos el ADC */
    Init_ADC_Module();
    /* Bucle principal */
    while(true)
    {
        /* Iniciamos el ADC */
        ADCON0 |= (1 << _ADCON0_ON_POSITION);
        /* Esperamos a que termine la conversión */
        while(ADCON0 & (1 << _ADCON0_ON_POSITION));
        /* Guardamos el valor de 12bits de la lectura */
        uint16_t adc_read = (ADRESH << 8) | ADRESL;
        /* Convertimos lo leído a un valor e voltaje */
        float valor_adc = ((float)adc_read/4095.0)*5.0;
        
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_ADC_Module (void)
{
    /* Limpiamos los registros */
    ADCON0 = 0x00;
    ADCON1 = 0x00;
    ADCON2 = 0x00;
    ADCON3 = 0x00;
    
    /* Configuramos los registrs */
    ADCON0 &= ~(1 << _ADCON0_ON_POSITION); // Apagamos el ADC 
    ADCON0 &= ~(1 << _ADCON0_CONT_POSITION); // Conv. no continua
    ADCON0 &= ~(1 << _ADCON0_CS_POSITION); // Freq. Fosc
    /* La frecuencia de divide por ADCLK
     * considerando: ADClock = Fosc /(2 * (n + 1)) 
     * donde n es el valor que se carga en ADCLK,
     * ahora queremos dividir entre 4 e FOSC, por lo tanto
     * n = 1, entonces:
     */
    ADCLK = 0x01; // ADCLK = 1 / FOSC / 4
    /* La justificación hace referencia a la como deseamos
     * colocar los 12 bits del ADC en los registros del 
     * ADRESH y ADRESL, registros de 8 bits que componen
     * el registro ADRES de 16bits
     * Considerando:
     *         ADRESH                       ADRESL  
     * ------------------------   -------------------------
     * |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
     * -------------------------  -------------------------
     * 
     * FM = 1 - Justificado a la derecha
     *         ADRESH                       ADRESL  
     * -------------------------  -------------------------
     * |  |  |  |  | X| X| X| X|  | X| X| X| X| X| X| X| X|
     * -------------------------  -------------------------
     * 
     * FM = 0 - Justificado a la izquierda
     *        ADRESH                       ADRESL  
     * ------------------------   -------------------------
     * | X| X| X| X| X| X| X| X|  | X| X| X| X|  |  |  |  |
     * -------------------------  -------------------------
     */
    ADCON0 |= (1 << _ADCON0_FM_POSITION); // Right Justify
    
    /* Los registros ADCON1 ADCON2 ADCON3 son para el 
     * manejo de las características del módulo computacional
     * por ahora no lo usaremos */
    
    /* Elegimos los voltajes de referencia */
    ADREF &= ~(1 << _ADREF_NREF_POSITION); // NREF a VSS
    ADREF &= ~(0b11 << _ADREF_PREF0_POSITION); // PREF a VDD
    
    /* Elegimos el canal de conversión del ADCC */
    ADPCH = 0x00; // Elegimos la entrada analógica 0 del puerto A
    
    /* Como vamos a usar el RA0 como entrada analógica configuramos */
    TRISA |= (1 << _TRISA_TRISA0_POSITION); // RA0 como entrada
    ANSELA |= (1 << _ANSELA_ANSELA0_POSITION); // RA0 como análogo
    
    /* Los registros 
     * ADPRE = ADC Precharge Time Control Register
     * ADACQ = ADC Acquisition Time Control Register
     * No serán usados por ahora */
    ADPRE = 0x00;
    ADACQ = 0x00;
    
    /* Encendemos el ADC */
    ADCON0 |= (1 << _ADCON0_ON_POSITION); // Módulo ADC prendido
}

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