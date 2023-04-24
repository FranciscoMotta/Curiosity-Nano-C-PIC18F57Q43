/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 24 de abril de 2023
 */

/*
 * Includes
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "system_config.h"

/*
 * Macros
 */

#define Led_Sys_Gpio        3
#define Led_Sys_Tris        TRISF
#define Led_Sys_Lat         LATF

/*
 * Declaracion de funciones
 */

/*
 * Main
 */
int main(void) 
{
    /* Led del sistema como salida */
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    /* Prendemos el LED */
    Led_Sys_Lat &= ~(1 << Led_Sys_Gpio);
    while(1);
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */