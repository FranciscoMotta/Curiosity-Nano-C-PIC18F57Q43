/* Proyecto de control de un LCD 16X2 */

/* 
 * Includes 
 */

#include "mcc_generated_files/mcc.h"
#include "project_defines.h"
#include "FM_Lcd_Easy.h"
/*
 * Macros
 */

/*
 * Declaracion de funciones
 */

void FM_Init_Gpio(void);

/*
  Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    FM_Init_Gpio();
    FM_Lcd_Easy_Init();
    FM_Lcd_Send_Command(0x80);
    FM_Lcd_Send_Character('A');
    while (1)
    {
        Led_Sys_Toggle();
        __delay_ms(250);
    }
}

/*
 * Definicion de funciones
 */

void FM_Init_Gpio(void)
{
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
}