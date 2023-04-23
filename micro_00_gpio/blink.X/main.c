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

/*
 * Definicion de macros
 */

#define User_Led_Sys        3
#define User_Led_Ansel      ANSELF
#define User_Led_Tris       TRISF
#define User_Led_Lat        LATF

/*
 * Declaracion de funciones
 */

void GPIO_Init (void);

/*
 * Main
 */
int main(void) 
{
    GPIO_Init();
    while (true)
    {
        // Prendemos el LED
        User_Led_Lat &= ~(1 << User_Led_Sys); // Mandamos un 0
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */


void GPIO_Init (void)
{
    User_Led_Ansel &= ~(1 << User_Led_Sys); // Pin digital
    User_Led_Tris &= ~(1 << User_Led_Sys); // Pin como salida
}