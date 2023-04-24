/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 23 de abril de 2023, 06:54 PM
 */

/*
 * Includes
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "project_defines.h"
#include "system_config.h"

/*
 * Macros
 */

#define User_Boton_Gpio     4
#define User_Boton_Tris     TRISB
#define User_Boton_Port     PORTB
#define User_Boton_Ansel    ANSELB
#define User_Boton_Wpu      WPUB

/*
 * Declaracion de funciones
 */

void Init_User_Boton (void);
void Init_User_Disp(void);

/*
 * Main
 */
int main(void) 
{
    Clock_Init(); // Iniciamos el reloj interno
    Init_User_Disp(); // Iniciamos el GPIO
    Init_User_Boton(); // Iniciamos el boton
    int8_t index = 0;
    while(1)
    {
        User_Display_7_Seg_Lat = num_comb_disp[index];
        __delay_ms(500);
        if(!(User_Boton_Port & (1 << User_Boton_Gpio))) 
        {
            index++;
            if (index > 9) index = 0;
        }
        else
        {
            index--;
            if (index < 0) index = 9;
        }
        
        
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_User_Disp(void)
{
    User_Display_7_Seg_Ansel = 0x00; // Puertos digitales
    User_Display_7_Seg_Tris = 0x00; // Puerto como salida
}

void Init_User_Boton (void)
{
    User_Boton_Tris |= (1 << User_Boton_Gpio); // Pin como entrada
    User_Boton_Ansel &= ~(1 << User_Boton_Gpio); // Pin digital
    User_Boton_Wpu |= (1 << User_Boton_Gpio); // Pull-up enabled
}