/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <stdio.h>
#include <string.h>

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "eusart.h"
#include "MRF24J40.h"
#include "ads119x.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

unsigned char tx_data[101],lecturas=0,cont_bat=0,i;
bool low_bat=false;

//void leer_registros(void);
//void ads_send_data(void);
//void mrf_demo(void);

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    for(i=0;i<101;i++)
    {
        tx_data[i]=0;
    }
    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    while(1)
    {
        //leer_registros();
        //ads_send_data();
        //mrf_demo();
        ///*
        if(lecturas > 24){
            MRF24J40_Send(tx_data, strlen(tx_data));
            lecturas=0;
            for(i=0;i<101;i++)
            {
                tx_data[i]=0;
            }
            if(low_bat){
                if(cont_bat >= 10){
                    cont_bat=0;
                    INDICADOR=~INDICADOR;
                }
                else
                    cont_bat++;
            }
        }
        else
            Sleep();
        //*/
    }
}

/*
void leer_registros(void){
    unsigned char registros[24];

    if(lecturas == 0){
        // Deshabilita conversion para poder leer los registros
        ADS_CS=0;
        SpiTxRx(SDATAC);
        __delay_us(4);
        ADS_CS=1;
        ADS_START=0;        
        lecturas++;
    }
    
    CKE=0;

    ADS_CS=0;
    SpiTxRx(ADS_READ|DEVICE_ID);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[0]=SpiTxRx(0x0);
    ADS_CS=1;
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|CONFIG1);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[1]=SpiTxRx(0x0);
    ADS_CS=1;
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|CONFIG2);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[2]=SpiTxRx(0x0);
    ADS_CS=1;
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|CONFIG3);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[3]=SpiTxRx(0x0);
    ADS_CS=1;
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|LEAD_OFF);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[4]=SpiTxRx(0x0);
    ADS_CS=1;
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras    

    ADS_CS=0;
    SpiTxRx(ADS_READ|CH1SET);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[5]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    
    ADS_CS=0;
    SpiTxRx(ADS_READ|CH2SET);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[6]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|CH3SET);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[7]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|CH4SET);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[8]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    
    ADS_CS=0;
    SpiTxRx(ADS_READ|CH5SET);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[9]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|CH6SET);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[10]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|CH7SET);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[11]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    
    ADS_CS=0;
    SpiTxRx(ADS_READ|CH8SET);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[12]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|RLD_SENSP);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[13]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|RLD_SENSN);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[14]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    
    ADS_CS=0;
    SpiTxRx(ADS_READ|LOFF_SENSP);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[15]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|LOFF_SENSN);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[16]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|LOFF_FLIP);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[17]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    
    ADS_CS=0;
    SpiTxRx(ADS_READ|GPIO_ADS);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[18]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|PACE);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[19]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|CONFIG4);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[20]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    
    ADS_CS=0;
    SpiTxRx(ADS_READ|WCT1);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[21]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras

    ADS_CS=0;
    SpiTxRx(ADS_READ|WCT2);
    SpiTxRx(SINGLE_BYTE_READ_WRITE);
    registros[22]=SpiTxRx(0x0);
    ADS_CS=1;        
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    
    sprintf(tx_data,"%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",registros[0],registros[1],registros[2],registros[3],registros[4],registros[5],registros[6],registros[7],registros[8],registros[9],registros[10],registros[11],registros[12],registros[13],registros[14],registros[15],registros[16],registros[17],registros[18],registros[19],registros[20],registros[21],registros[22]);
    MRF24J40_Send(tx_data, strlen(tx_data));
}

void ads_send_data(void){
    unsigned char dato[10];
    
    if(!ADS_DRDY)
    {
        CKE=0;
        __delay_us(4);
        ADS_CS=0;
        for(i=0;i<9;i++)
        {
            dato[i] = SpiTxRx(0x0);
        }
        ADS_CS=1;
        tx_data[0+lecturas]=dato[5]; // msb canal 2
        tx_data[25+lecturas]=dato[6]; // lsb canal 2 
        tx_data[50+lecturas]=dato[7]; // msb canal 3
        tx_data[75+lecturas]=dato[8]; // lsb canal 3 
        lecturas++;
        if(lecturas > 24){
            MRF24J40_Send(tx_data, strlen(tx_data)); // POR ALGUN MOTIVO HASTA 110 BYTES NOMAS
            lecturas=0;
            for(i=0;i<101;i++)
            {
                tx_data[i]=0;
            }
        }
    }
}

void mrf_demo(void){
    unsigned char muestra[3][60]=
    {
        {87,86,79,77,81,87,87,84,84,87,94,94,99,110,119,136,135,122,90,86,80,86,82,67,28,72,87,196,252,150,99,93,82,87,89,80,80,85,85,93,86,89,100,91,86,91,89,89,76,77,76,80,81,82,80},
        {90,89,82,80,84,90,90,87,87,90,97,97,102,113,122,139,138,125,93,89,83,89,85,70,31,75,90,199,255,153,102,96,85,90,92,83,83,88,88,96,89,92,103,94,89,94,92,92,79,80,79,83,84,85,83},
        {86,85,78,76,80,86,86,83,83,86,93,93,98,109,118,135,134,121,89,85,79,85,81,66,27,71,86,195,251,149,98,92,81,86,88,79,79,84,84,92,85,88,99,90,85,90,88,88,75,76,75,79,80,81,79}
    };    
    
    //for(i=0;i<3;i++){
        //MRF24J40_Send(muestra[i], strlen(muestra[i]));
        MRF24J40_Send(muestra[0], strlen(muestra[0]));
    //}
}
*/

// -------------------- Iterrupt Service Routines --------------------------
#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{
    unsigned char dato[12];
    if (INTCONbits.INT0IF) // Corresponde a IRQ por pin RB0. Se conecta a DRRY del ADS indicando un dato nuevo
    {
        INTCONbits.INT0IF = 0; // borramos la bandera
        // Sino se completo el buffer de lecturas, almacenamos datos...
        CKE=0; // Clock edge 1 para NRF, 0 para ADS. --> Preparado para ADS
        __delay_us(4);
        ADS_CS=0;
        for(i=0;i<9;i++)
        {
            dato[i] = SpiTxRx(0x0);
        }
        ADS_CS=1;
        //if(lecturas < 54){
            tx_data[0+lecturas]=dato[5]; // msb canal 2
            tx_data[25+lecturas]=dato[6]; // lsb canal 2
            tx_data[50+lecturas]=dato[7]; // msb canal 3
            tx_data[75+lecturas]=dato[8]; // lsb canal 3
            //tx_data[50+lecturas]=dato[9]; // msb canal 4
            //tx_data[75+lecturas]=dato[10]; // lsb canal 4
            lecturas++; // Incrementamos contador de lecturas...
        //}
        CKE=1; // Clock edge 1 para NRF, 0 para ADS. --> Preparado para MRF        
    }
}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{
    ;
}
