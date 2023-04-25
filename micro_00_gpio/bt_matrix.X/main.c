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
 * Nota: Para este ejercicio estamos colocando la
 * matriz de botones en el puerto D de la tarjeta
 * de desarrollo. Además se tiene que tomar en 
 * cuenta que las columnas están en los 4 bits MSB
 * y las filas se encuentras en los 4 bits LSB
 */

#define Matrix_Buttons_Tris     TRISD
#define Matrix_Buttons_Port     PORTD
#define Matrix_Buttons_Lat      LATD
#define Matrix_Buttons_Ansel    ANSELD
#define Matrix_Buttons_Wpu      WPUD

#define Mask_Rows               0x0F   
#define Mask_Columns            0xF0

// Filas como entradas - Columnas como salidas
#define Rows_As_Input()         Matrix_Buttons_Tris = Mask_Rows; 

// Columnas como entradas - Filas como salidas
#define Columns_As_Input()      Matrix_Buttons_Tris = Mask_Columns;

/*
 * Declaracion de funciones
 */

void Internal_Clock_Init (void);
void User_Led_Init (void);

/*
 * Funciones de la matriz de botones 
 */

char character_matrix_buttons[16] = {'1', '2', '3', 'A',
                                     '4', '5', '6', 'B',
                                     '7', '8', '9', 'C',
                                     '*', '0', '#', 'D'};

char User_Matrix_Decode_Raw_Readed (uint8_t raw_value);
void User_Matrix_Buttons_Init (void);
uint8_t User_Matrix_Buttons_Read (void);


/*
 * Main
 */

int main(void) 
{
    Internal_Clock_Init(); // Iniciamos el reloj interno a 16Mhz
    FM_Lcd_Easy_Init(); // Iniciamos el LCD en conf 4bits
    User_Matrix_Buttons_Init(); // Iniciamos la conf. de la matriz de botones
    FM_Lcd_Set_Cursor(ROW_1, COL_1); 
    FM_Lcd_Send_String("Lect. Matrix 4x4");
    FM_Lcd_Set_Cursor(ROW_2, COL_1);
    FM_Lcd_Send_Uint8(123);
    while(1)
    {
        FM_Lcd_Set_Cursor(ROW_2, COL_1);
        FM_Lcd_Send_String("RAW: ");
        uint8_t raw_data = User_Matrix_Buttons_Read();
        FM_Lcd_Send_Uint8(raw_data);
        FM_Lcd_Send_String(" CHAR: ");
        FM_Lcd_Send_Character(User_Matrix_Decode_Raw_Readed(raw_data));
        User_Led_Toggle();
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

char User_Matrix_Decode_Raw_Readed (uint8_t raw_value)
{
    char hexa_value = 0x00;
    switch(raw_value)
    {
        case 238:
            hexa_value = '1';
            break;
        case 222:
            hexa_value = '2';
            break;
        case 190:
            hexa_value = '3';
            break;
        case 126:
            hexa_value = 'A';
            break;
        case 237:
            hexa_value = '4';
            break;
        case 221:
            hexa_value = '5';
            break;
        case 189:
            hexa_value = '6';
            break;
        case 125:
            hexa_value = 'B';
            break;
        case 235:
            hexa_value = '7';
            break;
        case 219:
            hexa_value = '8';
            break;
        case 187:
            hexa_value = '9';
            break;
        case 123:
            hexa_value = 'C';
            break;
        case 231:
            hexa_value = '*';
            break;
        case 215:
            hexa_value = '0';
            break;
        case 183:
            hexa_value = '#';
            break;
        case 119:
            hexa_value = 'D';
            break;
        default:
            __nop();
            break;
    }
    return hexa_value;
}

uint8_t User_Matrix_Buttons_Read (void)
{
    // 1.- Activamos las resistencias pull-up
    Matrix_Buttons_Wpu = 0xFF; // Resistencias Pull-up enabled
    
    // 2.- Ponemos las filas como salidas 
    Rows_As_Input();
    
    // 3.- Ponemos los 4bits MSB en 0 lógico
    Matrix_Buttons_Lat = 0x0F; // 4 bits msb en 0v
    Rows_As_Input();
    
    // 4.- Esperamos a leer un 0 por los 4 bits LSB
    while(Matrix_Buttons_Port == 0x0F);
    
    // 5.- Salvamos el valor del puerto y limpiamos el resto
    uint8_t read_rows = Matrix_Buttons_Port & 0x0F;
    
    // 6.- Columnas como entradas - Filas como salida
    Columns_As_Input();
    
    // 7.- Ponemos los 4bits LSB como 0 lógico 
    Matrix_Buttons_Lat = 0xF0; // 4 bits lsb en 0v
    Columns_As_Input();
    
    // 8.- Esperamos al 0 lógico
    while (Matrix_Buttons_Port == 0xF0);
    
    // 9.- Salvamos el valor del puerto y limpiamos el resto
    uint8_t read_cols = Matrix_Buttons_Port & 0xF0;
    Rows_As_Input();
    
    // Construimos el dato de retorno de la lectura
    uint8_t push_button = 0;
    push_button = read_cols | read_rows;
    return push_button;
}

void User_Matrix_Buttons_Init (void)
{
    // Necesario para entradas digitales
    Matrix_Buttons_Ansel = 0x00; // Pines del puerto como digitales 
    // Activamos las resistencias de pull-up
    Matrix_Buttons_Wpu = 0xFF; // Resistencias Pull-up enabled
}

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