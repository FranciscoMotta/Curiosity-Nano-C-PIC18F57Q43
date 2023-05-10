/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 8 de mayo de 2023, 06:54 PM
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
#include "FM_Uart3.h"

/*
 * Macros
 */

/*
 * Declaracion de funciones
 */

void Init_Internal_Oscillator (void);
void Init_Gpio_System (void);
void Init_Interrupt_Uart (void);
void Init_Uart3 (void);
/*
 * Main
 */
int main(void) 
{
    /* Se configura el reloj interno */
    Init_Internal_Oscillator();
    /* Se configura las interrupciones */
    Init_Interrupt_Uart();
    /* Se configura el uart3 */
    Init_Uart3();
    /* NOTA:
     * La parte de los PPS se hará fuera de
     * la librería del uart3.
     */
    // PPS Entrada
    U3RXPPS = 0x29; // Conectado a RF1 UART3 RX
    // PPS Salida
    RF0PPS = 0x26; // Conectado a RF0 UART3 TX
    /* Se configuran los pines del sistema */
    Init_Gpio_System();
    /* Bucle principal */
    while(true)
    {
        FM_Send_Uart3_String("HOLA UART3 PIC18F57Q43 - LIB\n\r");
        Led_Sys_Tog();
        __delay_ms(1000);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_Interrupt_Uart (void)
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
    /* Configuramos el led del sistema */
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Off();
    /* Configuramos los pines del UART3 */
    UART3_RX_ANSEL &= ~(1 << UART3_RX_GPIO);
    UART3_RX_TRIS |= (1 << UART3_RX_GPIO);
    
    UART3_TX_TRIS &= ~(1 << UART3_TX_GPIO);
    UART3_TX_LAT &= ~(1 << UART3_TX_GPIO);  
}

void Init_Uart3 (void)
{
    _my_uart3_config_params_t my_uart3;
    
    /* Configuramos el protocolo */
    my_uart3.auto_baud_mode = Auto_Baud_Disabled;
    my_uart3.baudios = Baud_Rate_9600BPS;
    my_uart3.hand_shake = Flow_Control_Off;
    my_uart3.mode_select_data = Asynchronous_8bits_Uart_Mode;
    my_uart3.op_mode_speed = Normal_Operation_Mode;
    my_uart3.port_enable = Uart3_Port_Enabled;
    my_uart3.rx_en = Receiver_Enabled;
    my_uart3.tx_en = Transmiter_Enabled;
    my_uart3.rx_pol = Rx_Polarity_High_Def;
    my_uart3.tx_pol = Tx_Polarity_High_Def;
    my_uart3.stop_bits = Transmiter_1_SB_Receiver_Verify_SB;
    my_uart3.wake_up = Receiver_Wake_Up_Disabled;
    /* Pasamos los parámetros configurados */
    FM_Uart3_Config(&my_uart3);
}

void Init_Internal_Oscillator (void)
{
    _clock_hfintosc_params_t my_clock;
    my_clock.divisor_clock = clock_div_1;
    my_clock.frecuencia_clock = freq_clk_4MHZ;
    
    /* Pasamos los parámetros */
    FM_Hfintosc_Init(&my_clock);
}