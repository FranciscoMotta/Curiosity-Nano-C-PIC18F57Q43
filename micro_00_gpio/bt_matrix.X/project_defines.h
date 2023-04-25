/* 
 * File:   project_defines.h
 * Author: Administrador
 *
 * Created on 25 de abril de 2023, 03:47 PM
 */

#ifndef PROJECT_DEFINES_H
#define	PROJECT_DEFINES_H

#include <xc.h>
#include "system_config.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000UL
#endif

/*
 * Definiciones del LED del sistema
 */

#define User_Led_Sys          3
#define User_Led_Tris         TRISF
#define User_Led_Lat          LATF
#define User_Led_Mask         (1 << User_Led_Sys)

#define User_Led_On()         User_Led_Lat &= ~User_Led_Mask; // Led lógica negativa
#define User_Led_Off()        User_Led_Lat |= User_Led_Mask;
#define User_Led_Toggle()     User_Led_Lat ^= User_Led_Mask;


#endif	/* PROJECT_DEFINES_H */

