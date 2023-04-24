/* 
 * File:   project_defines.h
 * Author: jmott
 *
 * Created on 23 de abril de 2023, 06:59 PM
 */

#ifndef PROJECT_DEFINES_H
#define	PROJECT_DEFINES_H

#include <xc.h>
#include <stdint.h>

#define User_Display_7_Seg_Tris     TRISF
#define User_Display_7_Seg_Ansel    ANSELF
#define User_Display_7_Seg_Lat      LATF

#define DISPLAY_ANODO_COMUN

#ifdef DISPLAY_ANODO_COMUN
//                               0            1            2 
uint8_t num_comb_disp[10] = {0b01000000, 0b01111001, 0b00100100, 
//                               3            4           5
                             0b00110000, 0b00011001, 0b00010010, 
//                               6            7           8
                             0b00000010, 0b01111000, 0b00000000, 
//                               9
                             0b00010000};
#else // Aca debes cambiar los 0s por 1s
//                               0            1            2 
uint8_t num_comb_disp[10] = {0b01000000, 0b01111100, 0b00100100,
//                               3            4           5
                             0b00110000, 0b00011001, 0b00010010,
//                               6            7           8
                             0b00000010, 0b01111000, 0b00000000
//                               9
                             0b00100000};
#endif

#endif	/* PROJECT_DEFINES_H */

