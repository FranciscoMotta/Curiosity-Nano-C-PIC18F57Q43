/* Inclusiones */

#include "FM_Timer0.h"

/* Definicion de funciones */

void FM_Timer0_Init (_timer0_params_config_t timer_configs)
{
    T0CON0bits.OUTPS = timer_configs.timer0_postecaler;
    T0CON1bits.ASYNC = timer_configs.timer0_counter_sync;
    T0CON1bits.CS = timer_configs.timer0_clock_source;
    T0CON1bits.CKPS = timer_configs.timer0_prescaler;
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