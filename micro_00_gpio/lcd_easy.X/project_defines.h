/* 
 * File:   project_defines.h
 * Author: Administrador
 *
 * Created on 21 de abril de 2023, 07:14 PM
 */

#ifndef PROJECT_DEFINES_H
#define	PROJECT_DEFINES_H

/* Includes */

#include <xc.h>
#include <stdint.h>
#include "mcc_generated_files/device_config.h"
/* Led del sistema */

#define Led_Sys_Gpio        3
#define Led_Sys_Tris        TRISF
#define Led_Sys_Lat         LATF
#define Led_Sys_On()        Led_Sys_Lat |= (1 << Led_Sys_Gpio);
#define Led_Sys_Off()       Led_Sys_Lat &= ~(1 << Led_Sys_Gpio);
#define Led_Sys_Toggle()    Led_Sys_Lat ^= (1 << Led_Sys_Gpio);

#endif	/* PROJECT_DEFINES_H */

