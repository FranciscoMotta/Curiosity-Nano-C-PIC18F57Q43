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
    FM_Lcd_Set_Cursor(ROW_1, COL_1);
    FM_Lcd_Send_String("HOLA LCD 2X16:)");
    FM_Lcd_Set_Cursor(ROW_2, COL_3);
    FM_Lcd_Send_String("PIC18F57Q43");
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