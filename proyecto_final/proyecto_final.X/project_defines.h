
/* 
 * File:   Project_defines
 * Author: Julio Motta
 * Comments: Nothing yet
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define Led_Sys_Gpio    3
#define Led_Sys_Tris    TRISF
#define Led_Sys_Lat     LATF
#define Led_Sys_Ansel   ANSELF

#define Sens_RPM_Gpio   2
#define Sens_RPM_Tris   TRISA
#define Sens_RPM_Port   PORTA
#define Sens_RPM_Ansel  ANSELA

/* Macros de la entrada de la interrupcion */
#define Interrupt_Pin_Gpio  0
#define Interrupt_Pin_Tris  TRISB
#define Interrupt_Pin_Port  PORTB
#define Interrupt_Pin_Ansel ANSELB

/* Macros del control del motor */

#define Motor_Ena_Gpio      7
#define Motor_Ena_Tris      TRISD
#define Motor_Ena_Lat       LATD
#define Motor_Ena_Ansel     ANSELD

/* Led Warning */
#define Led_Warning_Indicator   0
#define Led_Warning_Tris        TRISC
#define Led_Warning_Lat         LATC
#define Led_Warning_Ansel       ANSELC

/* Tipos de datos y enumeraciones */

typedef enum
{
    ANA0_CHANEL = 0b000000,
    ANA1_CHANEL,
    ANA2_CHANEL,
    ANA3_CHANEL,
    ANA4_CHANEL,
    ANA5_CHANEL
}_adc_pin_to_read_t;

#endif	/* XC_HEADER_TEMPLATE_H */

