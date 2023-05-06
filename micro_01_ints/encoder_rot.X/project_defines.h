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

/* Macros del pin codificador de sentido */
#define Pin_Felt_Encoder_Gpio    2
#define Pin_Felt_Encoder_Tris    TRISA
#define Pin_Felt_Encoder_Ansel   ANSELA
#define Pin_Felt_Encoder_Port    PORTA


/* Macros de la entrada de la interrupcion */
#define Interrupt_Pin_Gpio  0
#define Interrupt_Pin_Tris  TRISB
#define Interrupt_Pin_Ansel ANSELB

/* Macros de los led de sentido de giro */
#define ClockWise_Led_Gpio  0
#define ClockWise_Led_Tris  TRISF
#define ClockWise_Led_Lat   LATF

/* Macros de los led de sentido de giro */
#define Anti_ClockWise_Led_Gpio  1
#define Anti_ClockWise_Led_Tris  TRISF
#define Anti_ClockWise_Led_Lat   LATF


/* Macros de la bandera de mov */
#define ROTATION_FLAG    0
#endif	/* PROJECT_DEFINES_H */

