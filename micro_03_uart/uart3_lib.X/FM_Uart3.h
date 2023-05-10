/* 
 * File:   FM_Uart3.h
 * Author: Administrador
 *
 * Created on 8 de mayo de 2023, 06:57 PM
 */

#ifndef FM_UART3_H
#define	FM_UART3_H

#include "system_config.h"
#include "project_defines.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000UL
#endif

#define _FOSC  4000000UL

/* Macros de los pines del UART3 */
#define UART3_TX_GPIO        0
#define UART3_TX_TRIS        TRISF
#define UART3_TX_LAT         LATF

#define UART3_RX_GPIO        1
#define UART3_RX_TRIS        TRISF
#define UART3_RX_ANSEL       ANSELF

/* Tipo de datos enumarados */

typedef enum
{
    Normal_Operation_Mode = 0,
    High_Speed_Operation_Mode
}_uart3_operation_mode_select_t;

typedef enum
{
    Auto_Baud_Disabled = 0,
    Auto_Baud_Enabled
}_uart3_autobaud_detect_enable_t;

typedef enum
{
    Transmiter_Disabled = 0,
    Transmiter_Enabled
}_uart3_transmiter_enabled_t;

typedef enum
{
    Receiver_Disabled = 0,
    Receiver_Enabled
}_uart3_receiver_enabled_t;

/*
 * Nota: Solo se implementarán las funcionalidades
 * básicas del protocolo UARTX, ya que las funciones:
 * - DALI Control Device mode
 * - DALI Control Gear mode
 * - DMX mode
 * - LIN Client Only mode
 * - LIN Host/Client mode
 * 
 * Posiblemente no sean soportadas por todos los puertos UART
 * Por tal motivo, solo pondremos los modos básicos, en una 
 * segunda librería extendida se usarán esas modalidades.
 */

typedef enum
{
    Asynchronous_8bits_Uart_Mode = 0,
    Asynchronous_7bits_Uart_Mode,
    Asynchronous_9bits_Uart_Mode_Odd_Parity,
    Asynchronous_9bits_Uart_Mode_Even_Parity,
    Asynchronous_9bits_Uart_Address_Mode
}_uart3_mode_select_t;

typedef enum
{
    Uart3_Port_Disabled = 0,
    Uart3_Port_Enabled 
}_uart3_enabled_port_t;

typedef enum
{
    Receiver_Wake_Up_Disabled = 0,
    Receiver_Wake_Up_Enabled
}_uart3_wake_up_receiver_t;

typedef enum
{
    Rx_Polarity_High_Def = 0,
    Rx_Polarity_Low
}_uart3_rx_polarity_t;

typedef enum
{
    Tx_Polarity_High_Def = 0,
    Tx_Polarity_Low
}_uart3_tx_polarity_t;

typedef enum
{
    Transmiter_1_SB_Receiver_Verify_SB = 0,
    Transmiter_1_5_SB_Receiver_Verify_SB,
    Transmiter_2_SB_Receiver_Verify_1_2SB,
    Transmiter_2_SB_Receiver_Verify_SB
}_uart3_stop_bits_ctrl_t;

typedef enum
{
    Flow_Control_Off = 0,
    XON_XOFF_Software_Flow_Control,
    RTS_CTS_And_TXDE_Hardware_Flow_Control,
    Reserved
}_uart3_hand_shake_config_t;

typedef enum
{
    Baud_Rate_300BPS    = 300UL,
    Baud_Rate_600BPS    = 600UL,
    Baud_Rate_1200BPS   = 1200UL,
    Baud_Rate_2400BPS   = 2400UL,
    Baud_Rate_4800BPS   = 4800UL,
    Baud_Rate_9600BPS   = 9600UL,
    Baud_Rate_14400BPS  = 14400UL,
    Baud_Rate_19200BPS  = 19200UL,
    Baud_Rate_38400BPS  = 38400UL,
    Baud_Rate_57600BPS  = 57600UL,
    Baud_Rate_115200BPS = 115200UL
}_uart3_baud_rate_select_t;

/* Creamos la estructura de configuracion */

typedef struct
{
    _uart3_baud_rate_select_t baudios;
    _uart3_operation_mode_select_t op_mode_speed;
    _uart3_autobaud_detect_enable_t auto_baud_mode;
    _uart3_transmiter_enabled_t tx_en;
    _uart3_receiver_enabled_t rx_en;
    _uart3_mode_select_t mode_select_data;
    _uart3_enabled_port_t port_enable;
    _uart3_wake_up_receiver_t wake_up;
    _uart3_rx_polarity_t rx_pol;
    _uart3_tx_polarity_t tx_pol;
    _uart3_stop_bits_ctrl_t stop_bits;
    _uart3_hand_shake_config_t hand_shake;
}_my_uart3_config_params_t;

/* Declaramos las funciones de configuración */

void FM_Uart3_Config (_my_uart3_config_params_t *uart3_params);
void FM_Send_Uart3_Byte (char byte);
void FM_Send_Uart3_String (char *string);
#endif	/* FM_UART3_H */

