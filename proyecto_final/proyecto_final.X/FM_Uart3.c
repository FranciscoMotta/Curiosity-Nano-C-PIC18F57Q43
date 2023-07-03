/*
 * Includes
 */

#include "FM_Uart3.h"

/*
 * Definiciones de funciones
 */

void FM_Uart3_Config (_my_uart3_config_params_t *uart3_params)
{
    /*
     * Primero, limpiamos los registros
     */
    
    U3TXB = 0x00;
    U3RXB = 0x00;
    
    U3CON0 = 0x00;
    U3CON1 = 0x00;
    U3CON2 = 0x00;
    
    /* Configuramos el registro U3CON0 */
    U3CON0 |= (uart3_params->op_mode_speed << _U3CON0_BRGS_POSITION);
    U3CON0 |= (uart3_params->auto_baud_mode << _U3CON0_ABDEN_POSITION);
    U3CON0 |= (uart3_params->tx_en << _U3CON0_TXEN_POSITION);
    U3CON0 |= (uart3_params->rx_en << _U3CON0_RXEN_POSITION);
    U3CON0 |= (uart3_params->mode_select_data << _U3CON0_MODE0_POSITION);
    
    /* Configuramos el registro U3CON1 */
    U3CON1 |= (uart3_params->port_enable << _U3CON1_ON_POSITION);
    U3CON1 |= (uart3_params->wake_up << _U3CON1_WUE_POSITION);
    
    /* Configuramos el registro U3CON2 */
    U3CON2 |= (uart3_params->rx_pol << _U3CON2_RXPOL_POSITION);
    U3CON2 |= (uart3_params->tx_pol << _U3CON2_TXPOL_POSITION);
    U3CON2 |= (uart3_params->stop_bits << _U3CON2_STP0_POSITION);
    U3CON2 |= (uart3_params->hand_shake << _U3CON2_FLO0_POSITION);
    
    /* Calculamos el valor del U3BRG */
    /* Table 34-2. Baud Rate Formulas - pg 559 datasheet */
    if(U3CON0 & (1 << _U3CON0_BRGS_POSITION)) // <- acá podia preguntar por uart3_params->op_mode_speed
    {
        /* High Speed Mode */
        U3BRG = (((_FOSC)/(4UL * uart3_params->baudios)) - 1UL);
    }
    else
    {
        /* Normal Speed Mode */
        U3BRG = (((_FOSC)/(16UL * uart3_params->baudios)) - 1UL);
    }
}

void FM_Send_Uart3_Byte (char byte)
{
    /* Cargamos el dato en el registro de envío */
    U3TXB = byte; 
    while(!(PIR9 & (1 << _PIR9_U3TXIF_POSITION)));
}

void FM_Send_Uart3_String (char *string)
{
    uint16_t index = 0;
    while(string[index] != '\0')
    {
        FM_Send_Uart3_Byte(string[index]);
        index++;
    }
}