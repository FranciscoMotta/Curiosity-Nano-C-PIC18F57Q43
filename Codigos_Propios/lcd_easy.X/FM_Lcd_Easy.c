/*
 * Includes
 */

#include "FM_Lcd_Easy.h"

/*
 * Declaracion de funciones
 */

void FM_Lcd_Easy_Gpio_Init (void)
{
    uint8_t bits_data;
#ifdef FOUR_BITS_HIGHER
    bits_data = 0xF0;
#else
    bits_data = 0x0F;
#endif
    Lcd_Data_Ctrl_Tris &= ~(bits_data | Bits_Control_Mask);
}

void FM_Lcd_Send_Nibble (char byte_to_send)
{
    uint8_t upper_bits, lower_bits;
#ifdef FOUR_BITS_HIGHER
    upper_bits = (byte_to_send & 0xF0);
    lower_bits = (byte_to_send & 0x0F) << 4;
#else 
    upper_bits = (byte_to_send & 0xF0) >> 4;
    lower_bits = (byte_to_send & 0x0F);
#endif
    
    Lcd_Data_Ctrl_Lat |= upper_bits;
    Lcd_Enable_Pulse();
    Lcd_Data_Ctrl_Lat &= ~upper_bits;
    Lcd_Data_Ctrl_Lat = lower_bits;
    Lcd_Enable_Pulse();
    Lcd_Data_Ctrl_Lat &= ~lower_bits;
}


void FM_Lcd_Send_Command (char command_to_send)
{
    Lcd_Control_Mode();
    FM_Lcd_Send_Nibble(command_to_send);
}

void FM_Lcd_Send_Character (char character_to_send)
{
    Lcd_Character_Mode();    
    FM_Lcd_Send_Nibble(character_to_send);
}

void FM_Lcd_Easy_Init (void)
{
    FM_Lcd_Easy_Gpio_Init(); // Iniciamos los GPIO para la LCD
    /* Ac� procedemos a mandar los comandos de control */
    FM_Lcd_Send_Command(0x01); // Clear Display
    FM_Lcd_Send_Command(0x02); // Return Home
    FM_Lcd_Send_Command(0x04); // Cursor to right
    FM_Lcd_Send_Command(0x0C); // Display On - Cursor Off - Blink Off
    FM_Lcd_Send_Command(0x14); // Shift cursor to right AC + 1
    FM_Lcd_Send_Command(0x2C); // 4bits LCD - 2 Lines - 5x11 dots
}