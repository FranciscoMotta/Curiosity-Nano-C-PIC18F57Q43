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
#define MINIMUN_PERIOD      100
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
    /* Timer */
    if (PIR3 & (1 << _PIR3_TMR0IF_POSITION)) 
    {
        /* C�digo */
        led_counter++;
        rpm_counter++;
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
            /* C�digo */
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
    /* Configuraci�n general */
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
        /* Leemos el valor del ADC0 */
        uint16_t adc_val = Adc_Read_Analog_Pin(ANA0_CHANEL);
        uint16_t com_val = Adc_Read_Analog_Pin(ANA1_CHANEL);
        float curr_motor = (com_val / 4095.0) * 5.0;
        uint8_t val_percent = ((float) adc_val / 4095.0) * 100.0;
        /* Detener interrupciones */
        INTCON0 &= ~(1 << _INTCON0_GIE_POSITION); // Disnable Ints
        average_ms += contador_ms;
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
            if(indicador_ms <= MINIMUN_PERIOD)
            {
               FM_Lcd_Send_Command(0x01);
               FM_Lcd_Set_Cursor(ROW_1, COL_2);
               FM_Lcd_Send_String("MAX SPEED!      "); 
               __delay_ms(500);
            }
            else if (indicador_ms >= MAXIMUN_PERIOD)
            {
                
               FM_Lcd_Send_Command(0x01);
               FM_Lcd_Set_Cursor(ROW_1, COL_2);
               FM_Lcd_Send_String("MOTOR STOPPED!      "); 
               __delay_ms(500);
            }
            else
            {
                float rpm = (FROM_MS_TO_HERTZ(indicador_ms)) * 60;
                sprintf(msj1, "V=%-3u%% R=%.2f", val_percent, rpm);
                sprintf(msj2, "C=%-3.3fA", curr_motor/10);
                FM_Lcd_Set_Cursor(ROW_1, COL_1);
                FM_Lcd_Send_String(msj1);
                FM_Lcd_Set_Cursor(ROW_2, COL_1);
                FM_Lcd_Send_String(msj2);
            }
        }
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

uint16_t Adc_Read_Analog_Pin (_adc_pin_to_read_t pin)
{
    /* Seleccionar el canal a leer */
    ADPCH = pin;
    /* Iniciar la conversi�n */
    ADCON0 |= (1 << _ADCON0_GO_NOT_DONE_POSITION);
    /* Esperar a que termine la conversi�n */
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
    /* Pasamos los par�metros configurados */
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
    /* Configurar el Uart3 */
    // Init_Uart3();
    /* Configurar el PPS */
    // Init_PPS();
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

    //    /* Interrupci�n externa */
    //    PIR1 &= ~(1 << _PIR1_INT0IF_POSITION); // Int0 flag clear
    //    PIE1 |= (1 << _PIE1_INT0IE_POSITION); // Int0 int enable
    //    INTCON0 &= ~(1 << _INTCON0_INT0EDG_POSITION); // Falling Edge

    /* Interrupci�n Timer0 */
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
    /* La justificaci�n hace referencia a la como deseamos
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
     * manejo de las caracter�sticas del m�dulo computacional
     * por ahora no lo usaremos */

    /* Elegimos los voltajes de referencia */
    ADREF &= ~(1 << _ADREF_NREF_POSITION); // NREF a VSS
    ADREF &= ~(0b11 << _ADREF_PREF0_POSITION); // PREF a VDD

    /* Elegimos el canal de conversi�n del ADCC */
    ADPCH = 0x00; // Elegimos la entrada anal�gica 0 del puerto A

    /* Como vamos a usar el RA0 como entrada anal�gica configuramos */
    TRISA |= (1 << _TRISA_TRISA0_POSITION); // RA0 como entrada
    ANSELA |= (1 << _ANSELA_ANSELA0_POSITION); // RA0 como an�logo

    /* Los registros 
     * ADPRE = ADC Precharge Time Control Register
     * ADACQ = ADC Acquisition Time Control Register
     * No ser�n usados por ahora */
    ADPRE = 0x00;
    ADACQ = 0x00;

    /* Encendemos el ADC */
    ADCON0 |= (1 << _ADCON0_ON_POSITION); // M�dulo ADC prendido
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
}

void Init_Internal_Oscillator(void) {
    /* INTOSC a 8mhz */
    _clock_hfintosc_params_t my_clock;
    my_clock.frecuencia_clock = freq_clk_16MHZ;
    my_clock.divisor_clock = clock_div_1;
    FM_Hfintosc_Init(&my_clock);
}