/* 
 * File:   system_config.h
 * Author: Administrador
 *
 * Created on 24 de abril de 2023, 03:31 PM
 */

#ifndef SYSTEM_CONFIG_H
#define	SYSTEM_CONFIG_H

// PIC18F57Q43 Configuration Bit Settings

// 'C' source line config statements

#define WITH_CONFIGS

#ifdef WITH_CONFIGS

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG2
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = OFF     // PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be set and cleared repeatedly)
#pragma config CSWEN = OFF      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)

// CONFIG3
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = OFF     // Multi-vector enable bit (Interrupt contoller does not use vector table to prioritze interrupts)
#pragma config IVT1WAY = OFF    // IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared and set repeatedly)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG4
#pragma config BORV = VBOR_1P9  // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (HV on MCLR/VPP must be used for programming)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG5
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG6
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG7
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config DEBUG = OFF      // Background Debugger (Background Debugger disabled)

// CONFIG8
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not Write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG10
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#endif

#include <xc.h>

/*
 * Configuracion del oscilador interno de alta frecuencia
 */

typedef enum 
{
    clock_div_1 = 0b0000, // Dividir el reloj entre 1 ... 
    clock_div_2 = 0b0001,
    clock_div_4 = 0b0010,
    clock_div_8 = 0b0011,
    clock_div_16 = 0b0100,
    clock_div_32 = 0b0101,
    clock_div_64 = 0b0110,
    clock_div_128 = 0b0111,
    clock_div_256 = 0b1000,
    clock_div_512 = 0b1001
}_div_clock_hfintosc_t;

typedef enum
{
    freq_clk_1MHZ = 0b0000, // Frecuencia del osciliador interno de HF
    freq_clk_2MHZ = 0b0001,
    freq_clk_4MHZ = 0b0010,
    freq_clk_8MHZ = 0b0011,
    freq_clk_12MHZ = 0b0100,
    freq_clk_16MHZ = 0b0101,
    freq_clk_32MHZ = 0b0110,
    freq_clk_48MHZ = 0b0111,
    freq_clk_64MHZ = 0b1000
}_freq_clock_hfintosc_t;

typedef struct
{
    _div_clock_hfintosc_t   divisor_clock;
    _freq_clock_hfintosc_t  frecuencia_clock;
}_clock_hfintosc_params_t;

/*
 * Funciones para inicar el oscilador interno de alta frecuencia
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000UL
#endif

#define HIGH_FREQ_CONF      0b110

void FM_Hfintosc_Init (_clock_hfintosc_params_t *clock_params);

#endif	/* SYSTEM_CONFIG_H */

