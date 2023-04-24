#include "system_config.h"

/*
 * Funciones para iniciar el oscilador interno de alta freq
 */

void FM_Hfintosc_Init (_clock_hfintosc_params_t *clock_params)
{
    OSCCON1 = 0x00;
}
