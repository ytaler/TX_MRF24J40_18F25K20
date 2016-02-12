/**
  EUSART Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart.c

  @Summary
    This is the generated driver implementation file for the EUSART driver using MPLAB® Code Configurator

  @Description
    This header file provides implementations for driver APIs for EUSART.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
        Device            :  PIC18F45K20
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB             :  MPLAB X v2.35 or v3.00
 */

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */

/**
  Section: Included Files
 */
#include "eusart.h"

/**
  Section: EUSART APIs
 */

void EUSART_Initialize(void) {
    // TRMT TSR_empty; TX9 8-bit; TX9D 0x0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
    TXSTA = 0b00100110; // Pagina 246 manual --> SYNC=0, BGRH=1

    // SPEN enabled; OERR no_error; RX9 8-bit; RX9D 0x0; CREN enabled; ADDEN disabled; SREN disabled; FERR no_error; 
    RCSTA = 0b10010000; // Pagina 247 manual
    //RCSTA = 0b10000000; // Solo TX
    
    // ABDOVF no_overflow; RCIDL idle; BRG16 16bit_generator; WUE disabled; CKTXP async_noninverted_sync_fallingedge; ABDEN disabled; DTRXP not_inverted; 
    BAUDCON = 0b00001000; // Pagina 248 manual --> BGR16=1

    // Estos valores dependen del clock del micro!! REVISAR!! Pagina 252
    // Para SYNC=0, BGRH=1 y BGR16=1
    // a 4 MHz el valor para 9600 baudios con un error de 0,16 es 103=0067
    // a 8 MHz el valor para 9600 baudios con un error de 0,16 es 207=0x00CF
    // a 16 MHz el valor para 9600 baudios con un error de -0.08 es 416=0x01A0
    SPBRGH=0x01;
    SPBRG=0xA0;    
}

uint8_t EUSART_Read(void) {

    while (!PIR1bits.RCIF) {
    }

    if (1 == RCSTAbits.OERR) {
        // EUSART error - restart

        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }

    return RCREG;
}

void EUSART_Write(uint8_t txData) {
    while (0 == PIR1bits.TXIF) {
    }

    TXREG = txData; // Write the data byte to the USART.
}

char getch(void) {
    return EUSART_Read();
}

void putch(char txData) {
    EUSART_Write(txData);
}
/**
  End of File
 */
