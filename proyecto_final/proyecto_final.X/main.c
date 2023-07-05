/*
 * Includes
 */

#include <xc.h>
#include <pic18.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_config.h"
#include "FM_Lcd_Easy.h"
#include "project_defines.h"
#include "FM_Uart3.h"

/*
 * Macros
 */

#define ERROR_CORRECT_FACTOR 6
#define MINIMUN_PERIOD      0
#define MAXIMUN_PERIOD      5000
#define FROM_MS_TO_HERTZ(x)    (1/(x * (0.001)))

/*
 * Declaracion de funciones
 */

void System_Init(void);
void Init_Internal_Oscillator(void);
void Init_Gpio_System(void);
void Init_ADCC_Module(void);
void Init_Timer0_As_Timer(void);
void Init_Interrupts(void);
void Init_Uart3 (void);
void Init_PPS (void);
void Init_PWM_Motor (void);
void Init_Timer2_As_Timer (void);
bool Detect_Falling_Edge (void);
uint16_t Adc_Read_Analog_Pin (_adc_pin_to_read_t pin);

/*
 * Variables globales
 */

char msj1[16];
char msj2[16];
uint64_t led_counter = 0;
uint64_t rpm_counter = 0;
uint16_t print_counter = 0;

bool print_flag = 0;
bool refresh_flag = 0;

volatile uint16_t contador_ms = 0;
volatile uint16_t indicador_ms = 0;
volatile uint16_t sample_counter = 0;
volatile uint16_t average_ms = 0;

/* Rutinas de servicio a interrupciones */

void __interrupt(irq(IRQ_TMR0)) ISR(void) 
{
    /* Timer 0 - 1ms*/
    if (PIR3 & (1 << _PIR3_TMR0IF_POSITION)) 
    {
        /* Código */
        led_counter++;
        rpm_counter++; // < ESTE IMPORTA
        print_counter++;

        if(print_counter >= 50)
        {
            print_flag = !print_flag;
            print_counter = 0;
        }
        if (led_counter >= 250) {
            Led_Sys_Lat ^= (1 << Led_Sys_Gpio);
            led_counter = 0;
        }
        
        if(Detect_Falling_Edge())
        {
            /* Código */
            contador_ms = rpm_counter;
            rpm_counter = 0;
        } 
        else
        {   
            if(rpm_counter >= MAXIMUN_PERIOD)
            {
                rpm_counter = MAXIMUN_PERIOD;
                indicador_ms = rpm_counter;
            }
        }

        /* Cargar el valor al registro */
        TMR0 = 6;
        /* Limpiar la bandera de interrupcion */
        PIR3 &= ~(1 << _PIR3_TMR0IF_POSITION);
    }
}

/*
 * Main
 */
int main(void) 
{
    /* Configuración general */
    System_Init();
    /* Habilitar el enable del motor */
    Motor_Ena_Lat |= (1 << Motor_Ena_Gpio);
    /* Mensaje por el LCD */
    FM_Lcd_Send_Command(0x01);
    FM_Lcd_Set_Cursor(ROW_1, COL_3);
    FM_Lcd_Send_String("PF MOTOR RPM");
    FM_Lcd_Set_Cursor(ROW_2, COL_3);
    FM_Lcd_Send_String("Julio Motta");  
    __delay_ms(4000);
    FM_Lcd_Send_Command(0x01);
    /* Bucle principal */
    while (true) 
    {
        /* Leemos el valor del ADC0 - Velocidad (RPM)*/
        uint16_t adc_val = Adc_Read_Analog_Pin(ANA0_CHANEL);
        /* Leemos el valor del ADC1 - Consumo (Amp)*/
        uint16_t com_val = Adc_Read_Analog_Pin(ANA1_CHANEL);
        /* Conversiones */
        float curr_motor = (com_val / 4095.0) * 5.0;
        uint8_t val_percent = ((float) adc_val / 4095.0) * 100.0;
        /* Sacamos el valor PWM */
        CCPR1 = (adc_val << 2);
        /* Detener interrupciones */
        INTCON0 &= ~(1 << _INTCON0_GIE_POSITION); // Disnable Ints
        average_ms += contador_ms; // average_ms = average_ms + contador_ms;
        sample_counter++;
        /* Reanudar interrupciones */
        INTCON0 |= (1 << _INTCON0_GIE_POSITION); // Enable Ints
        if(sample_counter >= 4)
        {
            indicador_ms = (average_ms / 4) + ERROR_CORRECT_FACTOR;
            sample_counter = 0;
            average_ms = 0;
        }
        /* Imprimir LCD cada 100ms*/
        if(print_flag)
        {
            if (indicador_ms >= MAXIMUN_PERIOD)
            {
                
               FM_Lcd_Send_Command(0x01);
               FM_Lcd_Set_Cursor(ROW_1, COL_2);
               FM_Lcd_Send_String("MOTOR STOPPED!"); 
               Led_Warning_Lat |= (1 << Led_Warning_Indicator);
               __delay_ms(1000);
            }
            else
            {
                float rpm = (FROM_MS_TO_HERTZ(indicador_ms)) * 60;
                sprintf(msj1, "V=%-3u%% R=%-3.0frpm", val_percent, rpm);
                sprintf(msj2, "C=%-3.3fA", curr_motor/10);
                FM_Lcd_Set_Cursor(ROW_1, COL_1);
                FM_Lcd_Send_String(msj1);
                FM_Lcd_Set_Cursor(ROW_2, COL_1);
                FM_Lcd_Send_String(msj2);
                Led_Warning_Lat &= ~(1 << Led_Warning_Indicator);
            }
        }
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_Timer2_As_Timer (void)
{
    /* Limpiar los registros */
    T2CON = 0x00;
    T2HLT = 0x00;
    T2RST = 0x00;
    /* Configurar el timer */
    T2CLKCON = 0b0001; // Se elige Fosc/4
    T2CON &= ~(1 << _T2CON_ON_POSITION); // Timer 2 off
    /* Recordamos:
     * PWM Period = [T2PR + 1] * 4 * Tosc * (TMR2 Prescaler)
     * T2PR = (PWM Period / (4 * Tosc * (TMR2 Prescaler))) - 1
     * Se desea: 10Hkz - Freq = 0.1ms = 10^-4seg
     * Por lo tanto:
     * PWM Period = 10^-4seg
     * Tosc = (125/2)nseg
     * TMR2 Prescaler = 4
     * T2PR = (10^-4 / (4 * (125 / 2) * 10^-9 * 4)) - 1
     * T2PR = 99
     *  */
    T2CON |= (0b010 << _T2CON_CKPS0_POSITION); // Prescaler 1:4
    T2CON |= (0b0000 << _T2CON_OUTPS0_POSITION); // Postcaler 1:1
    
    /* Se carga el valor del T2PR */
    T2PR = 99;
    
    /* Habilitamos el Timer 2 */
    T2CON |= (1 << _T2CON_ON_POSITION); // Enable timer2
}

void Init_PWM_Motor (void)
{
    /* Limpiar registros */
    CCP1CON = 0x00;
    CCPR1H = 0x00;
    CCPR1L = 0x00;
    CCPTMRS0 = 0x00;
    
    /* Configurar el módulo CCP1 como PWM */
    CCP1CON |= (1 << _CCP1CON_EN_POSITION); // CCP1 Encendido
    CCP1CON |= (1 << _CCP1CON_FMT_POSITION); // Left Justify
    CCP1CON |= (0b1100 << _CCP1CON_CCP1MODE0_POSITION); // PWM mode
    
    /* Asignamos el timer 2 */
    CCPTMRS0 |= (0b01 << _CCPTMRS0_C1TSEL0_POSITION);
    
    /* Asignamos la salida del CCP1 - RC1 */
    RC1PPS = 0x15;
    
    /* Pin RC1 como salida */
    ANSELC |= (1 << _ANSELC_ANSELC1_POSITION); // Pin analógico
    TRISC &= ~(1 << _TRISC_TRISC1_POSITION); // Pin como salida
    LATC &= ~(1 << _LATC_LATC1_POSITION); // Inicia apagado
}

uint16_t Adc_Read_Analog_Pin (_adc_pin_to_read_t pin)
{
    /* Seleccionar el canal a leer */
    ADPCH = pin;
    /* Iniciar la conversión */
    ADCON0 |= (1 << _ADCON0_GO_NOT_DONE_POSITION);
    /* Esperar a que termine la conversión */
    while (ADCON0 & (1 << _ADCON0_GO_NOT_DONE_POSITION));
    uint16_t adc_val = (ADRESH << 8) | ADRESL;
    return adc_val;
}

void Init_PPS (void)
{
    // PPS Entrada
    U3RXPPS = 0x29; // Conectado a RF1 UART3 RX
    // PPS Salida
    RF0PPS = 0x26; // Conectado a RF0 UART3 TX
    
    /* Configuramos los pines del UART3 */
    UART3_RX_ANSEL &= ~(1 << UART3_RX_GPIO);
    UART3_RX_TRIS |= (1 << UART3_RX_GPIO);
    
    UART3_TX_TRIS &= ~(1 << UART3_TX_GPIO);
    UART3_TX_LAT &= ~(1 << UART3_TX_GPIO);  
}

void Init_Uart3 (void)
{
    /* Creamos la variable de config. */
    _my_uart3_config_params_t my_uart3;
    
    /* Configuramos el protocolo */
    my_uart3.auto_baud_mode = Auto_Baud_Disabled;
    my_uart3.baudios = Baud_Rate_9600BPS;
    my_uart3.hand_shake = Flow_Control_Off;
    my_uart3.mode_select_data = Asynchronous_8bits_Uart_Mode;
    my_uart3.op_mode_speed = Normal_Operation_Mode;
    my_uart3.port_enable = Uart3_Port_Enabled;
    my_uart3.rx_en = Receiver_Enabled;
    my_uart3.tx_en = Transmiter_Enabled;
    my_uart3.rx_pol = Rx_Polarity_High_Def;
    my_uart3.tx_pol = Tx_Polarity_High_Def;
    my_uart3.stop_bits = Transmiter_1_SB_Receiver_Verify_SB;
    my_uart3.wake_up = Receiver_Wake_Up_Disabled;
    /* Pasamos los parámetros configurados */
    FM_Uart3_Config(&my_uart3);
}

bool Detect_Falling_Edge (void)
{
    static bool past_value = true;
    bool current_value = false;
    bool falling_edge_detected = false;
    /* Se convierte a bool */
    current_value = !!(Interrupt_Pin_Port & (1 << Interrupt_Pin_Gpio));
    falling_edge_detected = past_value && !current_value;
    past_value = current_value;
    return falling_edge_detected;
}

void System_Init(void) {
    /* Configurar el Oscilador Interno */
    Init_Internal_Oscillator();
    /* Configurar interrupciones */
    Init_Interrupts();
    /* Configurar el timer0 a 1ms */
    Init_Timer0_As_Timer();
    /* Configurar el ADC */
    Init_ADCC_Module();
    /* Configurar el GPIO */
    Init_Gpio_System();
    /* Configurar el LCD */
    FM_Lcd_Easy_Init();
    /* Configurar el Timer2 */
    Init_Timer2_As_Timer();
    /* Configurar el CCP1 */
    Init_PWM_Motor();
}

void Init_Timer0_As_Timer(void) {
    /* Limpiar los registros */
    T0CON0 = 0x00;
    T0CON1 = 0x00;

    /* Configurar el timer0 a 1ms */
    T0CON0 &= ~(1 << _T0CON0_EN_POSITION); // Timer0 off
    T0CON0 &= ~(1 << _T0CON0_MD16_POSITION); // 8 Bits
    T0CON0 |= (0b0000 << _T0CON0_OUTPS0_POSITION); // Postcaler 1:1

    T0CON1 |= (0b010 << _T0CON1_CS0_POSITION); // Clock Source Fosc/4
    T0CON1 |= (1 << _T0CON1_ASYNC_POSITION); // Not sync
    T0CON1 |= (0b0100 << _T0CON1_CKPS0_POSITION); // Prescaler 1:16

    /* Se desea calcular 1ms:
     * 
     * TMR0 = 2^n - (T * FOSC) / /4 * prescaler)
     * 
     * Datos:
     * T = 10^-3
     * Fosc = 16*10^6
     * Prescaler = 16
     * n = 8
     * 
     * Por lo tanto:
     * 
     * TMR0 = 2^8- (10^-3 * 16*10^6) / (4 * 16)
     * TMR0 = 6DEC
     */

    TMR0 = 6;

    /* Encender el timer0 */
    T0CON0 |= (1 << _T0CON0_EN_POSITION);
}

void Init_Interrupts(void) {
    /* Limpiar los registros */
    INTCON0 = 0x00;

    /* Configurar las interrupciones timer0 y int0 */
    INTCON0 |= (1 << _INTCON0_GIE_POSITION); // Enable Ints
    INTCON0 &= ~(1 << _INTCON0_IPEN_POSITION); // Sin Prior

    //    /* Interrupción externa */
    //    PIR1 &= ~(1 << _PIR1_INT0IF_POSITION); // Int0 flag clear
    //    PIE1 |= (1 << _PIE1_INT0IE_POSITION); // Int0 int enable
    //    INTCON0 &= ~(1 << _INTCON0_INT0EDG_POSITION); // Falling Edge

    /* Interrupción Timer0 */
    PIR3 &= ~(1 << _PIR3_TMR0IF_POSITION); // TMR0 flag clear
    PIE3 |= (1 << _PIE3_TMR0IE_POSITION); // TMR0 Int enable
}

void Init_ADCC_Module(void) {
    /* Limpiar los registros */
    ADCON0 = 0x00;
    ADCON1 = 0x00;
    ADCON2 = 0x00;

    /* Configurar el ADC */
    ADCON0 &= ~(1 << _ADCON0_ON_POSITION); // ADC Apagado
    ADCON0 &= ~(1 << _ADCON0_CONT_POSITION); // ADC Continuo
    ADCON0 &= ~(1 << _ADCON0_CS_POSITION); // Fadc = Fosc
    ADCON0 &= ~(1 << _ADCON0_GO_POSITION); // No se inicia la conv.

    ADCLK = 0x01; // ADCLK = 1 / FOSC / 4
    /* La justificación hace referencia a la como deseamos
     * colocar los 12 bits del ADC en los registros del 
     * ADRESH y ADRESL, registros de 8 bits que componen
     * el registro ADRES de 16bits
     * Considerando:
     *         ADRESH                       ADRESL  
     * ------------------------   -------------------------
     * |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
     * -------------------------  -------------------------
     * 
     * FM = 1 - Justificado a la derecha
     *         ADRESH                       ADRESL  
     * -------------------------  -------------------------
     * |  |  |  |  | X| X| X| X|  | X| X| X| X| X| X| X| X|
     * -------------------------  -------------------------
     * 
     * FM = 0 - Justificado a la izquierda
     *        ADRESH                       ADRESL  
     * ------------------------   -------------------------
     * | X| X| X| X| X| X| X| X|  | X| X| X| X|  |  |  |  |
     * -------------------------  -------------------------
     */
    ADCON0 |= (1 << _ADCON0_FM_POSITION); // Right Justify

    /* Los registros ADCON1 ADCON2 ADCON3 son para el 
     * manejo de las características del módulo computacional
     * por ahora no lo usaremos */

    /* Elegimos los voltajes de referencia */
    ADREF &= ~(1 << _ADREF_NREF_POSITION); // NREF a VSS
    ADREF &= ~(0b11 << _ADREF_PREF0_POSITION); // PREF a VDD

    /* Elegimos el canal de conversión del ADCC */
    ADPCH = 0x00; // Elegimos la entrada analógica 0 del puerto A

    /* Como vamos a usar el RA0 como entrada analógica configuramos */
    TRISA |= (1 << _TRISA_TRISA0_POSITION); // RA0 como entrada
    ANSELA |= (1 << _ANSELA_ANSELA0_POSITION); // RA0 como análogo

    /* Los registros 
     * ADPRE = ADC Precharge Time Control Register
     * ADACQ = ADC Acquisition Time Control Register
     * No serán usados por ahora */
    ADPRE = 0x00;
    ADACQ = 0x00;

    /* Encendemos el ADC */
    ADCON0 |= (1 << _ADCON0_ON_POSITION); // Módulo ADC prendido
}

void Init_Gpio_System(void) {
    /* Configurar el GPIO */
    // Led del sistema
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Ansel &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Lat &= ~(1 << Led_Sys_Gpio);

    // Entrada del sensor
    Sens_RPM_Tris |= (1 << Sens_RPM_Gpio);
    Sens_RPM_Ansel &= ~(1 << Sens_RPM_Gpio);
    
    // Entrada de interrupciones
    Interrupt_Pin_Ansel &= ~(1 << Interrupt_Pin_Gpio); 
    Interrupt_Pin_Tris |= (1 << Interrupt_Pin_Gpio);
    
    // Habilitador del motor
    Motor_Ena_Ansel &= ~(1 << Motor_Ena_Gpio);
    Motor_Ena_Tris &= ~(1 << Motor_Ena_Gpio);
    
    // Led Indicator
    Led_Warning_Tris &= ~(1 << Led_Warning_Indicator);
    Led_Warning_Ansel &= ~(1 << Led_Warning_Indicator);
    Led_Warning_Lat &= ~(1 << Led_Warning_Indicator);
}

void Init_Internal_Oscillator(void) {
    /* INTOSC a 8mhz */
    _clock_hfintosc_params_t my_clock;
    my_clock.frecuencia_clock = freq_clk_16MHZ;
    my_clock.divisor_clock = clock_div_1;
    FM_Hfintosc_Init(&my_clock);
}