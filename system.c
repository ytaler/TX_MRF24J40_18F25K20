/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    ADCON0 = 0;         // ADC OFF

    // ¡¡¡¡¡ SI SE CAMBIA LA FRECUENCIA CAMBIAR DEFINICION EN SYSTEM.H !!!!
    OSCCON=0b01111100; // 01011100 = 0x5C
    // bit 7 IDLEN: Idle Enable bit --> 0 dispositivo entra en modo bajo consumo con instruccion sleep
    // bit 6-4 IRCF<2:0>: Internal Oscillator Frequency Select bits --> 101 frecuencia de 8 MHz
    // bit 3 OSTS: Oscillator Start-up Time-out Status bit --> 1 Segun configuracion de registro CONFIG1 (bits de configuracion)
    // bit 2 IOFS: HFINTOSC Frequency Stable bit --> 1 estable
    // bit 1-0 SCS<1:0>: System Clock Select bits --> 00 segun configuracion de registro CONFIG1 (bits de configuracion)

    // INTSRC disabled; PLLEN disabled; TUN 0x00; 
    OSCTUNE = 0x00;
    // Set the secondary oscillator
    //PLLEN = 0x01;
}
