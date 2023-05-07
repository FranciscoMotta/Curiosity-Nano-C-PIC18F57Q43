/* Inclusiones */

#include "FM_Timer0.h"

/* Definicion de funciones */

void FM_Timer0_Init (_timer0_params_config_t *timer_configs)
{
    /* Limpiamos los registros */
    
    T0CON0 = 0x00;
    T0CON1 = 0x00;
    
    /* Registro T0CON0 */
    
    T0CON0 |= (timer_configs->timer0_enable << _T0CON0_EN_POSITION); // Enable del timer
    T0CON0 |= (timer_configs->timer0_bits << _T0CON0_MD16_POSITION); // 8/16bits
    T0CON0 |= (timer_configs->timer0_postecaler << _T0CON0_OUTPS3_POSITION); // Postescaler
    
    /* Registro T0CON1 */
    T0CON1 |= (timer_configs->timer0_clock_source << _T0CON1_CS2_POSITION); // Clock source
    T0CON1 |= (timer_configs->timer0_counter_sync << _T0CON1_ASYNC_POSITION); // Sync
    T0CON1 |= (timer_configs->timer0_prescaler << _T0CON1_T0CKPS3_POSITION); // Prescaler
}

int16_t FM_Timer0_Load_Calculator(uint16_t time)
{
    int16_t load_timer;
    if(T0CON0bits.MD16)
    {
        load_timer = ((time /(_TCY * T0CON1bits.CKPS)) - 65536);
    }
    else
    {
        load_timer = ((time / (_TCY * T0CON1bits.CKPS)) - 256);
    }
    return load_timer;
}