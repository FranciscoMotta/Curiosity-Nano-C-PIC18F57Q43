/* 
 * File:   FM_LCD_Easy.h
 * Author: Administrador
 *
 * Created on April 14, 2023, 7:41 PM
 */

#ifndef FM_LCD_EASY_H
#define	FM_LCD_EASY_H

#include <xc.h>

#define UN_SOLO_PUERTO

#ifdef UN_SOLO_PUERTO
#define UNI_PORT_TRIS   TRISF
#define UNI_PORT_LAT    LATF
#else
#warning "DEFINE LAS MACROS DE LCD_DATA_PORT_XXXX y LCD_CONTROL_PORT_XXXX"
#define LCD_DATA_PORT_TRIS   
#define LCD_DATA_PORT_LAT

#define LCD_CONTROL_PORT_TRIS
#define LCD_CONTROL_PORT_LAT
#endif

#include "project_defines.h"

/* NOTA: OJO!!!
 * Se supone que lo de abajo lo debes hacer igual para 
 * la definición de que trabaja los pines del LCD
 * en puertos separados, es decir, los pines de control
 * separados de los pines de datos.
 */

/* Rutinas simples de control */

/* Control */
#define LCD_CHARACTER_MODE()       UNI_PORT_LAT |= (1 << LCD_RS_PIN);
#define LCD_COMMAND_MODE()         UNI_PORT_LAT &= ~(1 << LCD_RS_PIN);
#define LCD_RS_PIN                 2
#define LCD_RW_PIN                 1
#define LCD_EN_PIN                 0

#define LCD_CONTROL_MASK           (1 << LCD_RS_PIN) | (1 << LCD_RW_PIN) | (1 << LCD_EN_PIN) 

/* Datos */
#define LCD_DATA_D4                4
#define LCD_DATA_D5                5
#define LCD_DATA_D6                6
#define LCD_DATA_D7                7

#define LCD_DATA_MASK              (1 << LCD_DATA_D4) | (1 << LCD_DATA_D5) | (1 << LCD_DATA_D6) | (1 << LCD_DATA_D7)

/* Enable */
#define EN_ON()                    UNI_PORT_LAT |= (1 << LCD_EN_PIN);
#define EN_OFF()                   UNI_PORT_LAT &= ~(1 << LCD_EN_PIN);
#define LCD_ENABLE_TOG()           EN_ON(); __delay_ms(1); EN_OFF();

/* Funcion:
 * void FM_LCD_Gpio_Init (void);
 * Parámetros de entrada: void
 * Parámetros de salida: void
 */

void FM_LCD_Gpio_Init (void);

#endif	/* FM_LCD_EASY_H */

