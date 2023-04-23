/* 
 * File:   project_defines.h
 * Author: Administrador
 *
 * Created on April 14, 2023, 7:34 PM
 */

#ifndef PROJECT_DEFINES_H
#define	PROJECT_DEFINES_H

/* User Led Defines */

#define USER_LED_TRIS       TRISF
#define USER_LED_LAT        LATF
#define USER_LED            3   
#define USER_LED_TIME       250
#define USER_LED_ON()       USER_LED_LAT  |= (1 << USER_LED);
#define USER_LED_OFF()      USER_LED_LAT  &= ~(1 << USER_LED);
#define USER_LED_TOGGLE()   USER_LED_ON(); __delay_ms(USER_LED_TIME); USER_LED_OFF(); __delay_ms(USER_LED_TIME);

/* Definicion de funciones */

void User_GPIO_LED (void)
{
    USER_LED_TRIS &= ~(1 << USER_LED); // Led como salida
}

#endif	/* PROJECT_DEFINES_H */

