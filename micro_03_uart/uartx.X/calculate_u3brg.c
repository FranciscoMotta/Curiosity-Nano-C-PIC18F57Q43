/* Librerías */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define _FOSC 	4000000UL

typedef enum
{
	NORMAL_MODE = 0,
	HIGH_SPEED_MODE
}_operation_mode_t;

/* Declaracion de funciones */

uint32_t calculate_u3brg (_operation_mode_t mode, uint32_t baud_rate);

/* Main */

int main (void)
{
	printf("el valor de U3BRG = %u", calculate_u3brg(NORMAL_MODE, 9600));
	return 0;
}

/* Definicion de funciones */

uint32_t calculate_u3brg (_operation_mode_t mode, uint32_t baud_rate)
{
	uint32_t u3brg_register = 0;
	if(mode == NORMAL_MODE)
	{
		u3brg_register = ((((_FOSC)/(16 * baud_rate))) - 1);
	}
	else
	{
		u3brg_register = ((((_FOSC)/(4 * baud_rate))) - 1);
	}
	return u3brg_register;
}
