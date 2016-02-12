#include "MRF24J40.h"
#include "user.h"
#include "eusart.h"

#include <stdio.h>

extern bool low_bat;

void Init_MRF24J40(void) {
    int _i;
    unsigned int _channel;
    _channel=CHANNEL_26;
    //printf("Inicializando modulo MRF24J40: modo %cx, canal (hexa) %u....\r\n",modo,_channel);
    
    CKE=1;//Clock edge 1 para NRF, 0 para ADS

    // Perform a hardware reset
    MRF24J40_RESET = 0; // activate reset
    __delay_ms(3);
        MRF24J40_RESET = 1; // deactivate reset
    __delay_ms(10);

//1)    
    SetShortRAMAddress(SOFTRST,0x07);   // SW RESET
    INDICADOR = 1;
    do
    {
        _i = GetShortRAMAddress(SOFTRST);
    } while ((_i & 0x07) !=  0x00);
    INDICADOR = 0;    
    __delay_ms(2);

    SetShortRAMAddress(RXFLUSH, 0x01 ); // flush RX hw FIFO manually (workaround for silicon errata #1)

//2)
    SetShortRAMAddress(PACON2, 0x98); // Initialize FIFOEN = 1 and TXONTS = 0x6

//3)
    SetShortRAMAddress(TXSTBL, 0x95); // TXSTBL; RFSTBL=9, MSIFS-5

//4)        
    SetLongRAMAddress(RFCON0, 0x03);			// RFOPT=0x03

//5)
    SetLongRAMAddress(RFCON1, 0x01);			// VCOOPT=0x02, per datasheet
    
//6)
    SetLongRAMAddress(RFCON2, 0x80); // mask for PLL enable  // Enable PLL (PLLEN = 1)    
    
//7)
    SetLongRAMAddress(RFCON6, 0x90); // Initialize TXFIL = 1 and 20MRECVR = 1    

//8)
    SetLongRAMAddress(RFCON7, 0x80);    // Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator)

//9)
    SetLongRAMAddress(RFCON8, 0x10);    // Initialize RFVCO = 1

//10)
    SetLongRAMAddress(SLPCON1, 0x21);   // CLKOUT disabled, sleep clock divisor is 2

//11)
    SetShortRAMAddress(BBREG2, 0x80); // Set CCA MODE to 1 --> estaba en 80.
    
//12)
    SetShortRAMAddress(CCAEDTH, 0x60); //Set CCA ED threshold to -69 dBm
    
//13)
    SetShortRAMAddress(BBREG6, 0x40); // RSSI on every packet

//14) IRQ
    SetShortRAMAddress(INTCON_M, 0xFE); //0x00  Enable tx interrupts
    
//15) Channel
    SetLongRAMAddress(RFCON0, _channel|0x03); // set channel

//16) Tx power
    SetLongRAMAddress(RFCON3, 0x00);		// set transmit power 0 dBm
    
//17)
    SetShortRAMAddress(RFCTL, 0x04); // Perform a RF reset
    
//18)
    SetShortRAMAddress(RFCTL, 0x00); // back to normal operation
    
//19)
    __delay_ms(1); // Delay after RF reset
        
    // Set address
    for (_i = 0; _i < 8; _i++)
        SetShortRAMAddress(EADR0 + _i, MY_MAC_ADDRESS[_i]); // 0x05 address of EADR0 - drv_mrf_miwi_24j40

    SetShortRAMAddress(SADRL, MY_MAC_ADDRESS[0]);
    SetShortRAMAddress(SADRH, MY_MAC_ADDRESS[1]);

    SetShortRAMAddress(PANIDL, PAN_ID[0]); // set Pan ID
    SetShortRAMAddress(PANIDH, PAN_ID[1]); // set Pan ID

    
    while((GetLongRAMAddress(RFSTATE)&0xA0) != 0xA0);	// wait till RF state machine in RX mode

    /*
    bit 7-4 BATTH<3:0>: Battery Low-Voltage Threshold bits (1)
        0xE=1110 = 3.5V
        0xD=1101 = 3.3V
        0xC=1100 = 3.2V
        0xB=1011 = 3.1V
        0xA=1010 = 2.8V
        0x9=1001 = 2.7V
        0x8=1000 = 2.6V
        0x7=0111 = 2.5V
        0x6=0110 = Undefined
        ...
        0x0=0000 = Undefined
    */
    
    SetLongRAMAddress(RFCON5,0xB0); // 3,1v
    SetLongRAMAddress(RFCON6,0x98); // Battery monitor enabled
    
    // Turbo mode....
    SetShortRAMAddress(0x38,0x01);
    SetShortRAMAddress(0x3B,0x38);
    SetShortRAMAddress(0x3C,0x5C);
    SetShortRAMAddress(0x2A,0x02);
    INDICADOR = 1;
    do
    {
        _i = GetShortRAMAddress(SOFTRST);
    } while ((_i & 0x07) !=  0x00);
    INDICADOR = 0;
}

void SetShortRAMAddress(unsigned char address, unsigned char value) {
    MRF24J40_CS = 0;
    SpiTxRx(((address << 1) & 0b01111111) | 0x01);
    SpiTxRx(value);
    MRF24J40_CS = 1;
}

void SetLongRAMAddress(unsigned int address, unsigned char value) {
    MRF24J40_CS = 0;
    SpiTxRx(((address >> 3)&0b01111111) | 0x80);
    SpiTxRx(((address << 5)&0b11100000) | 0x10);
    SpiTxRx(value);
    MRF24J40_CS = 1;
}

unsigned char GetShortRAMAddress(unsigned char address) {
    unsigned char toReturn;
    CKE=1;
    MRF24J40_CS = 0;
    SpiTxRx(((address << 1) & 0b01111110));
    toReturn = SpiTxRx(0xFF);
    MRF24J40_CS = 1;
    return (toReturn);
}

unsigned char GetLongRAMAddress(unsigned int address) {
    unsigned char toReturn;
    MRF24J40_CS = 0;
    SpiTxRx(((address >> 3) & 0b01111111) | 0x80);
    SpiTxRx(((address << 5) & 0b11100000));
    toReturn = SpiTxRx(0xFF);
    MRF24J40_CS = 1;
    return (toReturn);
}

void MRF24J40_Send(unsigned char * _Value, unsigned char _Value_Length) {
    int i;
    unsigned char reg_temp;
    CKE=1;//Clock edge 1 para NRF, 0 para ADS

    INTCONbits.INT0IE = 0; //deshabilitar irq int0 hasta que envie los datos 

    reg_temp = GetShortRAMAddress(RXSR);
    if((reg_temp & 0x20) == 0x20)
        low_bat=true; // Si es igual a 0x20 significa que esta por debajo del umbral
    else
        low_bat=false; // Sino, esta con carga
    
    MRF24J40_CS = 0;
    SpiTxRx(0x80);
    SpiTxRx(0x10);
    SpiTxRx(HEADER_LENGTH);  // Dato 1, pos 0
    SpiTxRx(HEADER_LENGTH + _Value_Length); // data length fija, dato 2, pos 1
    SpiTxRx(0x01); // control frame
    SpiTxRx(0x88); // dato 4, pos 3
    //SetLongRAMAddress(0x004, SEQ_NUMBER); // sequence number
    SpiTxRx(reg_temp & 0x20); //Escribimos el valor de la bat , dato 5, pos 4
    SpiTxRx(PAN_ID[0]); // destinatoin pan
    SpiTxRx(PAN_ID[1]);
    SpiTxRx(DESTINATION_MAC_ADDRESS[0]); // destination address
    SpiTxRx(DESTINATION_MAC_ADDRESS[1]);
    SpiTxRx(PAN_ID[0]); // source pan
    SpiTxRx(PAN_ID[1]);
    SpiTxRx(MY_MAC_ADDRESS[0]);
    SpiTxRx(MY_MAC_ADDRESS[1]); // dato 13, pos 12
// los datos propiamente dichos comienzan en posicion 13, dato 14
    for (i = 0; i < (HEADER_LENGTH + _Value_Length + 2); i++)
        SpiTxRx(_Value[i]); // write frame into normal FIFO

    MRF24J40_CS = 1;
    
    reg_temp = GetShortRAMAddress(TXNCON);
    SetShortRAMAddress(TXNCON, ( ( (reg_temp | 0x04) & (!0x02) ) | 0x01) ); // ACK, Sin encriptacion y envio

    INTCONbits.INT0IE = 1; // habilitar nuevamente interrupcion int0

    //Codigo para esperar el ok de recepcion
    while((GetShortRAMAddress(TXNCON) & (0x04))!=0){
        ;
    }
}