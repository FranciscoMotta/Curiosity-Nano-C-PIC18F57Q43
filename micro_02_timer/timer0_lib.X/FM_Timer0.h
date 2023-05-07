/* 
 * File:   FM_Timer0.h
 * Author: Administrador
 *
 * Created on 6 de mayo de 2023, 05:14 PM
 */

#ifndef FM_TIMER0_H
#define	FM_TIMER0_H

/* Includes */

#include <xc.h>
#define _XTAL_FREQ  4000000UL
#define _TCY    (4 / _XTAL_FREQ)
/* typedefs y enumeraciones */

// Habilitacion y deshabilitación del timer 0

typedef enum 
{
    TMR0_Enable = 0,
    TMR0_Disable
}_timer0_enable_t;

// Modo de operación del timer 0

typedef enum
{
    TMR0_8bits = 0,
    TMR0_16bits
}_timer0_mode_op_t;

// Post-escaler del timer 0

typedef enum 
{
    TMR0_Post_1_1 = 0b0000,
    TMR0_Post_1_2,
    TMR0_Post_1_3,
    TMR0_Post_1_4,
    TMR0_Post_1_5,
    TMR0_Post_1_6,
    TMR0_Post_1_7,
    TMR0_Post_1_8,
    TMR0_Post_1_9,
    TMR0_Post_1_10,
    TMR0_Post_1_11,
    TMR0_Post_1_12,
    TMR0_Post_1_13,
    TMR0_Post_1_14,
    TMR0_Post_1_15,
    TMR0_Post_1_16
}_timer0_postescaler_t;

// Fuente de entrada de reloj del timer0

typedef enum
{
    TMR0_Pin_Select_T0CKIPPS_NI = 0,
    TMR0_Pin_Select_T0CKIPPS_I,
    TMR0_Fosc_div_4,
    TMR0_HFINTOSC,
    TMR0_LFINTOSC,
    TMR0_MFINTOSC,
    TMR0_SOSC,
    TMR0_CLC1_OUT,
}_timer0_clock_source_t;

// Habilitador de sincronismo del reloj de entrada

typedef enum
{
    TMR0_Counter_Sync_FOSC4 = 0,
    TMR0_Counter_Not_Sync
}_timer0_async_t;

// Configuramos el preescaler del timer 0

typedef enum
{
    TMR0_Prescaler_1_1 = 0,
    TMR0_Prescaler_1_2,
    TMR0_Prescaler_1_4,
    TMR0_Prescaler_1_8,
    TMR0_Prescaler_1_16,
    TMR0_Prescaler_1_32,
    TMR0_Prescaler_1_64,
    TMR0_Prescaler_1_128,
    TMR0_Prescaler_1_256,
    TMR0_Prescaler_1_512,
    TMR0_Prescaler_1_1024,
    TMR0_Prescaler_1_2048,
    TMR0_Prescaler_1_4096,
    TMR0_Prescaler_1_8192,
    TMR0_Prescaler_1_16384,
    TMR0_Prescaler_1_32768
}_timer0_prescaler_t;

/* Generamos la estructura de las configuraciones */

typedef struct
{
    _timer0_enable_t timer0_enable;
    _timer0_mode_op_t timer0_bits;
    _timer0_postescaler_t timer0_postecaler;
    _timer0_clock_source_t timer0_clock_source;
    _timer0_async_t timer0_counter_sync;
    _timer0_prescaler_t timer0_prescaler;
}_timer0_params_config_t;

/* Declaracion de funciones */

void FM_Timer0_Init (_timer0_params_config_t *timer_configs);

int16_t FM_Timer0_Load_Calculator(uint16_t time);
#endif	/* FM_TIMER0_H */

