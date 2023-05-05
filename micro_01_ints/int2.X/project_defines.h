/* 
 * File:   project_defines.h
 * Author: Administrador
 *
 * Created on 3 de mayo de 2023, 06:32 PM
 */

#ifndef PROJECT_DEFINES_H
#define	PROJECT_DEFINES_H

#include <xc.h>
#include <pic18.h>
/*
 * Definiciones del proyecto
 */

/* Macros del led */
#define Led_Sys_Gpio    3
#define Led_Sys_Tris    TRISF
#define Led_Sys_Lat     LATF
#define Led_Sys_On()    Led_Sys_Lat &= ~(1 << Led_Sys_Gpio);
#define Led_Sys_Off()   Led_Sys_Lat |= (1 << Led_Sys_Gpio);
#define Led_Sys_Tog()   Led_Sys_Lat ^= (1 << Led_Sys_Gpio);

/* Macros del pin generador de interrupciones */
#define Generat_Int_Gpio    3
#define Generat_Int_Tris    TRISC
#define Generat_Int_Lat     LATC
#define Generat_Int_Off()   Generat_Int_Lat &= ~(1 << Generat_Int_Gpio);
#define Generat_Int_Tog()   Generat_Int_Lat ^= (1 << Generat_Int_Gpio);

/* Macros de la entrada de la interrupcion */
#define Interrupt_Pin_Gpio  2
#define Interrupt_Pin_Tris  TRISB
#define Interrupt_Pin_Ansel ANSELB

#endif	/* PROJECT_DEFINES_H */

