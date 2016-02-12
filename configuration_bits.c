/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

// CONFIG1H
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable bit->Fail-Safe Clock Monitor enabled
#pragma config FOSC = INTIO67    // Oscillator Selection bits->Internal oscillator block, port function on RA6 and RA7
#pragma config IESO = OFF    // Internal/External Oscillator Switchover bit->Oscillator Switchover mode enabled

// CONFIG2L
#pragma config BOREN = OFF    // Brown-out Reset Enable bits->Brown-out Reset enabled in hardware only (SBOREN is disabled)
#pragma config PWRT = OFF    // Power-up Timer Enable bit->PWRT disabled
#pragma config BORV = 30    // Brown Out Reset Voltage bits->VBOR set to 1.8 V nominal

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits->1:32768
#pragma config WDTEN = OFF    // Watchdog Timer Enable bit->WDT is controlled by SWDTEN bit of the WDTCON register

// CONFIG3H
#pragma config CCP2MX = PORTC    // CCP2 MUX bit->CCP2 input/output is multiplexed with RC1
#pragma config MCLRE = ON    // MCLR Pin Enable bit->MCLR pin enabled; RE3 input pin disabled
#pragma config PBADEN = OFF    // PORTB A/D Enable bit->PORTB<4:0> pins are configured as analog input channels on Reset
#pragma config LPT1OSC = ON    // Low-Power Timer1 Oscillator Enable bit->Timer1 configured for low-power operation
#pragma config HFOFST = ON    // HFINTOSC Fast Start-up->HFINTOSC starts clocking the CPU without waiting for the oscillator to stablize.

// CONFIG4L
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
#pragma config LVP = OFF    // Single-Supply ICSP Enable bit->Single-Supply ICSP disabled
#pragma config STVREN = ON    // Stack Full/Underflow Reset Enable bit->Stack full/underflow will cause Reset
#pragma config DEBUG = OFF    // Background Debugger Enable bit->Background debugger disabled, RB6 and RB7 configured as general purpose I/O pins

// CONFIG5L
#pragma config CP0 = OFF    // Code Protection Block 0->Block 0 (000800-001FFFh) not code-protected
#pragma config CP2 = OFF    // Code Protection Block 2->Block 2 (004000-005FFFh) not code-protected
#pragma config CP1 = OFF    // Code Protection Block 1->Block 1 (002000-003FFFh) not code-protected
#pragma config CP3 = OFF    // Code Protection Block 3->Block 3 (006000-007FFFh) not code-protected

// CONFIG5H
#pragma config CPB = OFF    // Boot Block Code Protection bit->Boot block (000000-0007FFh) not code-protected
#pragma config CPD = OFF    // Data EEPROM Code Protection bit->Data EEPROM not code-protected

// CONFIG6L
#pragma config WRT1 = OFF    // Write Protection Block 1->Block 1 (002000-003FFFh) not write-protected
#pragma config WRT2 = OFF    // Write Protection Block 2->Block 2 (004000-005FFFh) not write-protected
#pragma config WRT3 = OFF    // Write Protection Block 3->Block 3 (006000-007FFFh) not write-protected
#pragma config WRT0 = OFF    // Write Protection Block 0->Block 0 (000800-001FFFh) not write-protected

// CONFIG6H
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block (000000-0007FFh) not write-protected
#pragma config WRTC = OFF    // Configuration Register Write Protection bit->Configuration registers (300000-3000FFh) not write-protected
#pragma config WRTD = OFF    // Data EEPROM Write Protection bit->Data EEPROM not write-protected

// CONFIG7L
#pragma config EBTR3 = OFF    // Table Read Protection Block 3->Block 3 (006000-007FFFh) not protected from table reads executed in other blocks
#pragma config EBTR2 = OFF    // Table Read Protection Block 2->Block 2 (004000-005FFFh) not protected from table reads executed in other blocks
#pragma config EBTR1 = OFF    // Table Read Protection Block 1->Block 1 (002000-003FFFh) not protected from table reads executed in other blocks
#pragma config EBTR0 = OFF    // Table Read Protection Block 0->Block 0 (000800-001FFFh) not protected from table reads executed in other blocks

// CONFIG7H
#pragma config EBTRB = OFF    // Boot Block Table Read Protection bit->Boot Block (000000-0007FFh) not protected from table reads executed in other blocks
