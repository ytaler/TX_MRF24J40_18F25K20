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

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "user.h"
#include "eusart.h"
#include "ads119x.h"
#include "MRF24J40.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void InitApp(void)
{
    // Configuracion de puertos E/S. Pines utilizados:
    // Pin 12 RB0/INT0/FLT0/AN12 --> Entrada --> INT
    
    // Pin 32 RC0/T1OSO/T13CKI --> Salida --> WAKE
    // Pin 35 RC1/T1OSO/T13CKI --> Salida --> RESET
    // Pin 36 RC2/CCP1/P1A --> Salida --> CS
    
    // Pin 37 RC3/SCK/SCL --> Salida --> Serial Clock ? SCK/SCL
    // Pin 42 RC4/SDI/SDA --> Entrada --> Serial Data In ? SDI/SDA
    // Pin 43 RC5/SDO --> Salida -->Serial Data Out ? SDO
    
    // Pin 44 RC6/TX/CK --> Entrada --> UART TX
    // Pin 1 RC7/RX/DT --> Entrada --> UART RX
    
    ANSEL = 0x00; // Apagar entradas analogicas
    ANSELH = 0x00; // Apagar entradas analogicas    
    TRISA=0b00000000; // SS debe estar como salida, el resto como entrada
    PORTA=0; // Limpiamos el puerto
    
    TRISB=0b00000011;
    PORTB=0x00;
    
    TRISC=0b00010000; // SDO y SCK como salida, SDI y el resto como entrada
    PORTC=0x00; // Limpiamos el puerto
   
    ADS_CS=1;
    MRF24J40_CS=1;    
    
    //EUSART_Initialize();
    Init_SPI();
    Init_MRF24J40(); // Initialize MRF24J40 module
    Init_ads();
    InitIRQ();
}

// Inicializacion de SPI
void Init_SPI(void){
    // Configuracion de SPI para TX
    // BF RCinprocess_TXcomplete; UA dontupdate; SMP Sample At Middle; P stopbit_notdetected; S startbit_notdetected; R_nW write_noTX; CKE Idle to Active; D_nA lastbyte_address; 
    SSPSTAT=0x00;
    CKE=1;//Clock edge 1 para NRF, 0 para ADS
    CKP=0;//El estado de idle es low
    SMP=0;
    SSPCON1=0x00; // Borrar valores de config. Toma todo por default.
    // bit 7 WCOL: Write Collision Detect bit --> No collision
    // bit 6 SSPOV: Receive Overflow Indicator bit --> No overflow
    SSPCON1bits.SSPEN=1; // bit 5 Habilita y configura los pines seriales SCK, SDO, SDI, y SS.
    // bit 4 CKP: Clock Polarity Select bit --> Idle state for clock is a low level
    // bit 3-0 SSPM<3:0>: Synchronous Serial Port Mode Select bits --> 0000 = SPI Master mode, clock = F OSC /4
    // Esto se corresponde al bit rate, si la frecuencia del micro es 4 MHz el bit rate es 1Mbit/s
}

// Funcion que envia y recibe los datos por SPI
unsigned char SpiTxRx (unsigned char data)
{
    SSPCON1bits.WCOL = 0;
    SSPBUF = data; // comienza la escritura con el dato recibido
    while(!PIR1bits.SSPIF); // espera la bandera que indica fin de tx/rx
    PIR1bits.SSPIF=0; // se borra la bandera
    return SSPBUF; // en caso que el receptor responda, retorna la respuesta
}

// Inicializa interrupciones
void InitIRQ(void){
    // Interrupciones pagina 110
    
    INTCON2bits.nRBPU = 0; // habilita resistencias pull-up en PORTB
    INTCON2bits.INTEDG0 = 0;    // interrupt on falling edge of INT0 (switch pressed)
    WPUB = 0x03; // seleccionamos conectar la resistencia pull-up en RB0 y RB1
    INTCONbits.INT0IF = 0;      // ensure flag is cleared
    INTCONbits.INT0IE = 1;      // enable INT0 interrupt
    // NOTE: INT0 is ALWAYS a high priority interrupt
    //INTCON2bits.INTEDG1 = 0; // falling edge
    //INTCON3bits.INT1F = 0;
    //INTCON3bits.INT1IP = 1;
    //INTCON3bits.INT1E = 1;
    // Set up global interrupts
    RCONbits.IPEN = 1;          // Enable priority levels on interrupts
    INTCONbits.GIEL = 1;        // Low priority interrupts allowed
    INTCONbits.GIEH = 1;        // Interrupting enabled.   
}