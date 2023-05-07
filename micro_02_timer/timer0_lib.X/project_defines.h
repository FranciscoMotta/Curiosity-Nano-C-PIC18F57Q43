/* 
 * File:   project_defines.h
 * Author: Administrador
 *
 * Created on 6 de mayo de 2023, 02:31 PM
 */

#ifndef PROJECT_DEFINES_H
#define	PROJECT_DEFINES_H

/* Macros del led */
#define Led_Sys_Gpio    3
#define Led_Sys_Tris    TRISF
#define Led_Sys_Lat     LATF
#define Led_Sys_On()    Led_Sys_Lat &= ~(1 << Led_Sys_Gpio);
#define Led_Sys_Off()   Led_Sys_Lat |= (1 << Led_Sys_Gpio);
#define Led_Sys_Tog()   Led_Sys_Lat ^= (1 << Led_Sys_Gpio);


#endif	/* PROJECT_DEFINES_H */

