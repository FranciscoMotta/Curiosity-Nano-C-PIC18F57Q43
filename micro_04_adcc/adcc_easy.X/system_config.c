#include <xc.h>
#include "system_config.h"

/*
 * Funciones para iniciar el oscilador interno de alta freq
 */

void FM_Hfintosc_Init (_clock_hfintosc_params_t *clock_params)
{
    OSCCON1bits.NOSC = HIGH_FREQ_CONF; // High Speed Internal Oscillator
    OSCCON1bits.NDIV = clock_params->divisor_clock; // Elegimos el divisor deseado
    OSCFRQbits.HFFRQ = clock_params->frecuencia_clock; // Elegimos la freq deseada
}
