/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 23 de abril de 2023
 */

/*
 * Includes
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "project_defines.h"
#include "system_config.h"
#include "FM_Lcd_Easy.h"

/*
 * Macros
 */

/*
 * Declaracion de funciones
 */

void Internal_Clock_Init (void);
void User_Led_Init (void);
/*
 * Main
 */
int main(void) 
{
    Internal_Clock_Init(); // Iniciamos el reloj interno a 16Mhz
    FM_Lcd_Easy_Init(); // Iniciamos el LCD en conf 4bits
    FM_Lcd_Set_Cursor(ROW_1, COL_1); 
    FM_Lcd_Send_String("Lect. Matrix 4x4");
    while(1)
    {
        User_Led_Toggle();
        __delay_ms(150);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void User_Led_Init (void)
{
    User_Led_Tris &= ~User_Led_Mask;
    User_Led_Off(); // Apagamos el LED
}

void Internal_Clock_Init (void)
{
    _clock_hfintosc_params_t my_clock_params;
    my_clock_params.divisor_clock = clock_div_1;
    my_clock_params.frecuencia_clock = freq_clk_16MHZ;
    FM_Hfintosc_Init(&my_clock_params);
}