/* Declarar una función */

void Clock_Local_Init (void)
{
    _clock_hfintosc_params_t my_clock_param;
    my_clock_param.divisor_clock = clock_div_1;
    my_clock_param.frecuencia_clock = freq_clk_16MHZ;
    FM_Hfintosc_Init(&my_clock_param); // Pasamos los datos de conf.
}
