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
#include <stdbool.h>

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

void Matrix_74LS922_Init(void);
uint8_t Read_Matrix_74LS922_Push(void);
void Init_User_Led(void);
void Internal_Oscillator_Init(void);

/*
 * Nota del proyecto:
 * El presente proyecto trata acerca de implementar un sistema de 
 * "seguridad" básico en el cual se pide que se introduzca por 
 * medio del teclado matricial una contraseña para acceder a un
 * determinado ambiente o abrir una determinada caja fuerte.
 * 
 * La clave constará de 6 digitos que el usuario deberá introducir.
 */

char clave_ingresada[6] = "******";
char clave_guardada[6] = "123456";

uint8_t claves_index = 0;
bool safe_lock = false;

/*
 * Main
 */

int main(void) 
{
    Internal_Oscillator_Init(); // Iniciamos el reloj interno a 16Mhz
    FM_Lcd_Easy_Init(); // Iniciamos el LCD
    Matrix_74LS922_Init(); // Iniciamos los pines del 74LS922
    FM_Lcd_Send_Command(0x01); // Limpiamos el LCD
    FM_Lcd_Set_Cursor(ROW_1, COL_3);
    FM_Lcd_Send_String("Locker Pic: ");
    FM_Lcd_Set_Cursor(ROW_2, COL_2);
    FM_Lcd_Send_String("pass:  ");
    FM_Lcd_Set_Cursor(ROW_2, COL_9);
    FM_Lcd_Send_String(clave_ingresada);
    while (1) 
    {
        while(!safe_lock)
        {
            if(Matrix_74LS922_Data_Port & (1 << Matrix_74LS922_Enable_Pin))
            {
                uint8_t data_readed = Matrix_74LS922_Data_Port & 0x0F;
                while (Matrix_74LS922_Data_Port & (1 << Matrix_74LS922_Enable_Pin));
                clave_ingresada[claves_index] = character_matrix_buttons[data_readed];
                claves_index++;
                FM_Lcd_Set_Cursor(ROW_2, COL_9);
                FM_Lcd_Send_String(clave_ingresada);
                if (claves_index > 5) safe_lock = true;
            }
            User_Led_Toggle();
            __delay_ms(100);
        } 
        User_Led_Off();
        bool pass_ok = false;
        for(uint8_t compare_index = 0 ; compare_index < 6 ; compare_index++)
        {
            if(clave_guardada[compare_index] != clave_ingresada[compare_index])
            {
                pass_ok = true; // La clave es diferente
            }
        }
        
        FM_Lcd_Send_Command(0x01); // Limpiamos la pantalla
        FM_Lcd_Set_Cursor(ROW_1, COL_3);
        FM_Lcd_Send_String("Locker Pic: ");
        FM_Lcd_Set_Cursor(ROW_2, COL_3);
        if(pass_ok)
        {
            FM_Lcd_Send_String("CLAVE ERROR");  
        }
        else
        {
            FM_Lcd_Send_String("CLAVE BIEN"); 
        }
        while(1);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

uint8_t Read_Matrix_74LS922_Push(void) 
{
    uint8_t data_readed;
    uint8_t lock_ret;
    if (Matrix_74LS922_Data_Port & (1 << Matrix_74LS922_Enable_Pin)) 
    {
        data_readed = Matrix_74LS922_Data_Port & 0x0F;
        while (Matrix_74LS922_Data_Port & (1 << Matrix_74LS922_Enable_Pin));
        clave_ingresada[claves_index] = character_matrix_buttons[data_readed];
        if (claves_index == 6) return 1;
        claves_index++;
    }
    return 0;
}

void Matrix_74LS922_Init(void) 
{
    Matrix_74LS922_Data_Ansel = 0x00; // Pines digitales
    Matrix_74LS922_Data_Tris = 0x1F; // Ponemos 0b00011111   
}

void Init_User_Led(void) 
{
    User_Led_Tris &= ~User_Led_Mask; // Le como salida
    User_Led_Off(); // Led apagado
}

void Internal_Oscillator_Init(void) 
{
    _clock_hfintosc_params_t my_clock_params;
    my_clock_params.divisor_clock = clock_div_1;
    my_clock_params.frecuencia_clock = freq_clk_16MHZ;
    FM_Hfintosc_Init(&my_clock_params);
}