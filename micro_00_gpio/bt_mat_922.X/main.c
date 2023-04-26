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

#include "system_config.h"
#include "project_defines.h"
#include "FM_Lcd_Easy.h"

/*
 * Macros
 */

/*
 * Nota:
 * Para esta sección del código está la librería de project_defines.h pero
 * para fines de explicación y entendimiento lo dejaremos acá, si usas este 
 * código y quieres acomodarlo o modificarlo sientete en la libertad ade poder 
 * hacerlo.
 * 
 * Nota 2:
 * 
 * El puerto usado será el PUERTO D, del cual los primeros 4 bits
 * serán puertos de entrada de datos procedentes del 74LS922
 * y el quinto bit ubicado en el PUERTO D4 será el enable de datos.
 */

#define Matrix_74LS922_Data_Port    PORTD
#define Matrix_74LS922_Data_Tris    TRISD
#define Matrix_74LS922_Data_Ansel   ANSELD
#define Matrix_74LS922_Enable_Pin   4
#define Matrix_74LS922_Mask         (1 << Matrix_74LS922_Enable_Pin)

char character_matrix_buttons[16] = {'1', '7', '4', '*',
                                     '3', '9', '6', '#',
                                     'A', 'C', 'B', 'D',
                                     '2', '8', '5', '0'};


/*
 * Declaracion de funciones
 */

void Matrix_74LS922_Init (void);
void Read_Matrix_74LS922_Push (void);
void Init_User_Led (void);
void Internal_Oscillator_Init (void);

/*
 * Main
 */
int main(void) 
{
    Internal_Oscillator_Init(); // Iniciamos el reloj interno a 16Mhz
    FM_Lcd_Easy_Init();         // Iniciamos el LCD
    Matrix_74LS922_Init();      // Iniciamos los pines del 74LS922
    FM_Lcd_Set_Cursor(ROW_1, COL_1);
    FM_Lcd_Send_String("Lect. Matrix 4x4");
    while(1)
    {
        FM_Lcd_Set_Cursor(ROW_2, COL_1);
        FM_Lcd_Send_String("BTN: ");
        Read_Matrix_74LS922_Push();
        User_Led_Toggle();
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Read_Matrix_74LS922_Push (void)
{
    uint8_t data_readed;
    if(Matrix_74LS922_Data_Port & (1 << Matrix_74LS922_Enable_Pin))
    {
        data_readed = Matrix_74LS922_Data_Port & 0x0F;
        FM_Lcd_Send_Uint8(data_readed);
        FM_Lcd_Send_String(" : ");
        FM_Lcd_Send_Character(character_matrix_buttons[data_readed]);
    }
}

void Matrix_74LS922_Init (void)
{
    Matrix_74LS922_Data_Ansel = 0x00; // Pines digitales
    Matrix_74LS922_Data_Tris = 0x1F; // Ponemos 0b00011111   
}

void Init_User_Led (void)
{
    User_Led_Tris &= ~User_Led_Mask; // Le como salida
    User_Led_Off(); // Led apagado
}
void Internal_Oscillator_Init (void)
{
    _clock_hfintosc_params_t my_clock_params;
    my_clock_params.divisor_clock = clock_div_1;
    my_clock_params.frecuencia_clock = freq_clk_16MHZ;
    FM_Hfintosc_Init(&my_clock_params);
}