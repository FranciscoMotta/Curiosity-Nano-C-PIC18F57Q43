#include "system_config.h"

void Clock_Init (void)
{
    OSCCON1bits.NDIV = 0b0000; // Post Divider FOSC/1
    OSCCON1bits.NOSC = 0b110; // High Speed Internal Oscillator
    OSCFRQbits.HFFRQ = 0b0101; // Configurando del oscilador a 16Mhz
}
