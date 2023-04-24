/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 23 de abril de 2023, 03:55 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "system_config.h"

/*
 * Definicion de macros
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000UL
#endif

#define User_Led_Sys        3
#define User_Led_Ansel      ANSELF
#define User_Led_Tris       TRISF
#define User_Led_Lat        LATF

#define User_Bot_Sys        4
#define User_Bot_Ansel      ANSELB
#define User_Bot_Tris       TRISB
#define User_Bot_Port       PORTB
#define User_Bot_Wpu        WPUB

/*
 * Declaracion de funciones
 */

void GPIO_Init (void);

/*
 * Main
 */
int main(void) 
{
    Clock_Init(); // Oscilador interno configurado.
    GPIO_Init();
    while (true)
    {
        if(!(User_Bot_Port & (1 << User_Bot_Sys)))
        {
            // Prendemos el LED
            User_Led_Lat &= ~(1 << User_Led_Sys); // Mandamos un 0
            __delay_ms(10);
        }
        else 
        {
            User_Led_Lat |= (1 << User_Led_Sys); // Mandamos un 1
        }
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */


void GPIO_Init (void)
{
    /* Led */
    
    User_Led_Ansel &= ~(1 << User_Led_Sys); // Pin digital
    User_Led_Tris &= ~(1 << User_Led_Sys); // Pin como salida
    
    /* Boton */
    User_Bot_Ansel &= ~(1 << User_Bot_Sys); // Pin digital
    User_Bot_Tris |= (1 << User_Bot_Sys); // Pin como entrada
    User_Bot_Wpu  |= (1 << User_Bot_Sys); // pull-up activado
}