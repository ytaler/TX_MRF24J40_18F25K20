#include <pic18.h>
#include "ads119x.h"
#include "user.h"
#include "system.h"
#include "MRF24J40.h"
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <stdio.h>
#include <string.h>

void Init_ads()
{
    unsigned char i,j;
    CKE=0;//Clock edge 1 para NRF, 0 para ADS

    ADS_START=0; //Detiene conversiones. Ya viene en cero, pero lo reafirmamos
    
    ADS_CS=0;
    SpiTxRx(RESET_ADS); // Resetea registros no hardware.. lleva 9 us.
    ADS_CS=1;

    //Delay de 1s para estabilizacion.. seguramente es menos, pero por las dudas...
    for(i=0; i<10; i++){
        __delay_ms(20);
        __delay_ms(20);
        __delay_ms(20);
        __delay_ms(20);
        __delay_ms(20);        
    }

    ADS_CS=0;
    SpiTxRx(SDATAC);
    //__delay_us(4);
    ADS_CS=1;

    __delay_ms(10); // Delay hasta procese el comando, exagerado por supuesto.
    
    //Verificación de dispositivo conectado. 10 intentos
    for(i=0; i<9 ; i++){
        ADS_CS=0;
        SpiTxRx(ADS_READ|DEVICE_ID);
        SpiTxRx(SINGLE_BYTE_READ_WRITE);
        j=SpiTxRx(0x0);
        __delay_us(4);
        ADS_CS=1;
        if(j==ADS1x9x_REV){
            // todo ok.
            MRF24J40_Send(mensaje_1, strlen(mensaje_1));
            INDICADOR = 0;
            break;
        }
        ///*
        else{
            if(i==8){
                //Condicion de error, dispositivo no detectado, no se puede continuar
                MRF24J40_Send(mensaje_2, strlen(mensaje_2));
                //char temp[32];
                //sprintf(temp,"ID:%u",j);
                //MRF24J40_Send(temp, strlen(temp));
                INDICADOR = 1;
                while(1) ;
            }
        }
        //*/
    }

// ********************* Configuracion **************************
    CKE=0;//Clock edge 1 para NRF, 0 para ADS
    ADS_CS=0;
    SpiTxRx(ADS_WRITE|CONFIG1); // Direccion inicial de escritura
    SpiTxRx(ADS1x9x_TOP_REGISTER_SIZE); // Indica la cantidad de registros a escribir
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // Config 1 - Direccion 0x01
    ADS1x9x_Config_1_Register.Config.Output_Data_Rate = MODULATION_FREQUENCY_DIVIDED_BY_512;
    ADS1x9x_Config_1_Register.Config.Config_1_Reserved_1 = CONFIG_1_RESERVED_VALUE_1;
    ADS1x9x_Config_1_Register.Config.Oscillator_Clock_Output = DEFAULT_OSCILLATOR_CLOCK_OUTPUT_DISABLED;
    ADS1x9x_Config_1_Register.Config.Readback_Mode = DEFAULT_DAISY_CHAIN_MODE;
    ADS1x9x_Config_1_Register.Config.Config_1_Reserved_2 = CONFIG_1_RESERVED_VALUE_2;
    SpiTxRx(ADS1x9x_Config_1_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Config 2 - Direccion 0x02 *******************
    ADS1x9x_Config_2_Register.Config.Test_Signal_Frequency = DEFAULT_PULSED_AT_CLOCK_FREQUENCY_DIVIDED_BY_2_TO_THE_21ST;
    ADS1x9x_Config_2_Register.Config.Test_Signal_Amplitude = DEFAULT_PLUS_MINUS_1_MV_TIMES_VREF_DIVIDED_BY_2_4;
    ADS1x9x_Config_2_Register.Config.Config_2_Reserved_1 = CONFIG_2_RESERVED_VALUE_1;
    ADS1x9x_Config_2_Register.Config.Test_Source = TEST_SIGNALS_ARE_DRIVEN_INTERNALLY;
    ADS1x9x_Config_2_Register.Config.Config_2_Reserved_2 = CONFIG_2_RESERVED_VALUE_2;
    SpiTxRx(ADS1x9x_Config_2_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Config 3 - Direccion 0x03 *******************
    ADS1x9x_Config_3_Register.Config.Right_Leg_Detect_Lead_Off_Status = DEFAULT_RLD_IS_CONNECTED;
    ADS1x9x_Config_3_Register.Config.Right_Leg_Detect_Sense_Selection = DEFAULT_RLD_SENSE_IS_DISABLED;
    ADS1x9x_Config_3_Register.Config.Right_Leg_Detect_Power_Buffer = RLD_BUFFER_IS_ENABLED;
    ADS1x9x_Config_3_Register.Config.Right_Leg_Detect_Reference_Source = IS_FED_INTERNALLY;
    ADS1x9x_Config_3_Register.Config.Right_Leg_Detect_Measurement = DEFAULT_IS_OPEN;
    ADS1x9x_Config_3_Register.Config.Reference_Voltage = IS_SET_TO_4_VOLTS;    
    ADS1x9x_Config_3_Register.Config.Config_3_Reserved = CONFIG_3_RESERVED_1_VALUE;
    ADS1x9x_Config_3_Register.Config.Internal_Reference_Buffer = ENABLE_INTERNAL_REFERENCE_BUFFER;
    SpiTxRx(ADS1x9x_Config_3_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* lead Off - Direccion 0x04 *******************
    ADS1x9x_Lead_Off_Control_Register.Config.Lead_Off_Comparator_Threshold = DEFAULT_NINETY_FIVE;
    ADS1x9x_Lead_Off_Control_Register.Config.Lead_Off_Current_Magnitude = DEFAULT_FOUR_nA;
    ADS1x9x_Lead_Off_Control_Register.Config.Lead_Off_Detection_Mode = DEFAULT_CURRENT_SOURCE_MODE;
    ADS1x9x_Lead_Off_Control_Register.Config.Lead_Off_Frequency = DEFAULT_NO_DETECTION;
    SpiTxRx(ADS1x9x_Lead_Off_Control_Register.Result); // 
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Channel 1 - Direccion 0x05 *******************
    ADS1x9x_Channel_Settings_Register.Config.Channel_Input_Is = ADS1x9x_INPUT_SHORTED;
    ADS1x9x_Channel_Settings_Register.Config.Channel_Setting_Reserved = CHANNEL_RESERVED_VALUE;
    ADS1x9x_Channel_Settings_Register.Config.Programmable_Gain_Setting = DEFAULT_GAIN_OF_6;
    ADS1x9x_Channel_Settings_Register.Config.Power_Down_Channel = ENABLE_POWER_DOWN;
    SpiTxRx(ADS1x9x_Channel_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Channel 2 - Direccion 0x06 *******************
    ADS1x9x_Channel_Settings_Register.Config.Channel_Input_Is = DEFAULT_ADS1x9x_ELECTRODE_INPUT;
    ADS1x9x_Channel_Settings_Register.Config.Channel_Setting_Reserved = CHANNEL_RESERVED_VALUE;
    ADS1x9x_Channel_Settings_Register.Config.Programmable_Gain_Setting = GAIN_OF_12;
    ADS1x9x_Channel_Settings_Register.Config.Power_Down_Channel = DEFAULT_DISABLE_POWER_DOWN;
    SpiTxRx(ADS1x9x_Channel_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Channel 3 - Direccion 0x07 *******************
    ADS1x9x_Channel_Settings_Register.Config.Channel_Input_Is = DEFAULT_ADS1x9x_ELECTRODE_INPUT;
    ADS1x9x_Channel_Settings_Register.Config.Channel_Setting_Reserved = CHANNEL_RESERVED_VALUE;
    ADS1x9x_Channel_Settings_Register.Config.Programmable_Gain_Setting = GAIN_OF_12;
    ADS1x9x_Channel_Settings_Register.Config.Power_Down_Channel = DEFAULT_DISABLE_POWER_DOWN;
    SpiTxRx(ADS1x9x_Channel_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Channel 4 - Direccion 0x08 *******************
    ADS1x9x_Channel_Settings_Register.Config.Channel_Input_Is = ADS1x9x_INPUT_SHORTED;
    ADS1x9x_Channel_Settings_Register.Config.Channel_Setting_Reserved = CHANNEL_RESERVED_VALUE;
    ADS1x9x_Channel_Settings_Register.Config.Programmable_Gain_Setting = DEFAULT_GAIN_OF_6;
    ADS1x9x_Channel_Settings_Register.Config.Power_Down_Channel = ENABLE_POWER_DOWN;
    SpiTxRx(ADS1x9x_Channel_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Channel 5 - Direccion 0x09 *******************
    ADS1x9x_Channel_Settings_Register.Config.Channel_Input_Is = ADS1x9x_INPUT_SHORTED;
    ADS1x9x_Channel_Settings_Register.Config.Channel_Setting_Reserved = CHANNEL_RESERVED_VALUE;
    ADS1x9x_Channel_Settings_Register.Config.Programmable_Gain_Setting = DEFAULT_GAIN_OF_6;
    ADS1x9x_Channel_Settings_Register.Config.Power_Down_Channel = ENABLE_POWER_DOWN;
    SpiTxRx(ADS1x9x_Channel_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Channel 6 - Direccion 0x0A *******************
    ADS1x9x_Channel_Settings_Register.Config.Channel_Input_Is = ADS1x9x_INPUT_SHORTED;
    ADS1x9x_Channel_Settings_Register.Config.Channel_Setting_Reserved = CHANNEL_RESERVED_VALUE;
    ADS1x9x_Channel_Settings_Register.Config.Programmable_Gain_Setting = DEFAULT_GAIN_OF_6;
    ADS1x9x_Channel_Settings_Register.Config.Power_Down_Channel = ENABLE_POWER_DOWN;
    SpiTxRx(ADS1x9x_Channel_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Channel 7 - Direccion 0x0B *******************
    ADS1x9x_Channel_Settings_Register.Config.Channel_Input_Is = ADS1x9x_INPUT_SHORTED;
    ADS1x9x_Channel_Settings_Register.Config.Channel_Setting_Reserved = CHANNEL_RESERVED_VALUE;
    ADS1x9x_Channel_Settings_Register.Config.Programmable_Gain_Setting = DEFAULT_GAIN_OF_6;
    ADS1x9x_Channel_Settings_Register.Config.Power_Down_Channel = ENABLE_POWER_DOWN;
    SpiTxRx(ADS1x9x_Channel_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Channel 8 - Direccion 0x0C *******************
    ADS1x9x_Channel_Settings_Register.Config.Channel_Input_Is = ADS1x9x_INPUT_SHORTED;
    ADS1x9x_Channel_Settings_Register.Config.Channel_Setting_Reserved = CHANNEL_RESERVED_VALUE;
    ADS1x9x_Channel_Settings_Register.Config.Programmable_Gain_Setting = DEFAULT_GAIN_OF_6;
    ADS1x9x_Channel_Settings_Register.Config.Power_Down_Channel = ENABLE_POWER_DOWN;
    SpiTxRx(ADS1x9x_Channel_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* RLD Sens P - Direccion 0x0D *******************
    ADS1x9x_RLD_Sens_P_Register.Config.Input_1 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_P_Register.Config.Input_2 = ENABLE;
    ADS1x9x_RLD_Sens_P_Register.Config.Input_3 = ENABLE;
    ADS1x9x_RLD_Sens_P_Register.Config.Input_4 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_P_Register.Config.Input_5 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_P_Register.Config.Input_6 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_P_Register.Config.Input_7 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_P_Register.Config.Input_8 = DEFAULT_DISABLE;
    SpiTxRx(ADS1x9x_RLD_Sens_P_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* RLD Sens N - Direccion 0x0E *******************
    ADS1x9x_RLD_Sens_N_Register.Config.Input_1 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_N_Register.Config.Input_2 = ENABLE;
    ADS1x9x_RLD_Sens_N_Register.Config.Input_3 = ENABLE;
    ADS1x9x_RLD_Sens_N_Register.Config.Input_4 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_N_Register.Config.Input_5 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_N_Register.Config.Input_6 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_N_Register.Config.Input_7 = DEFAULT_DISABLE;
    ADS1x9x_RLD_Sens_N_Register.Config.Input_8 = DEFAULT_DISABLE;
    SpiTxRx(ADS1x9x_RLD_Sens_N_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* LOFF - SENS P - Direccion 0x0F *******************
    ADS1x9x_Lead_Off_Sens_P_Register.Config.Input_1 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_P_Register.Config.Input_2 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_P_Register.Config.Input_3 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_P_Register.Config.Input_4 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_P_Register.Config.Input_5 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_P_Register.Config.Input_6 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_P_Register.Config.Input_7 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_P_Register.Config.Input_8 = DEFAULT_DISABLE;
    SpiTxRx(ADS1x9x_Lead_Off_Sens_P_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* LOFF - SENS N - Direccion 0x10 *******************
    ADS1x9x_Lead_Off_Sens_N_Register.Config.Input_1 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_N_Register.Config.Input_2 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_N_Register.Config.Input_3 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_N_Register.Config.Input_4 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_N_Register.Config.Input_5 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_N_Register.Config.Input_6 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_N_Register.Config.Input_7 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Sens_N_Register.Config.Input_8 = DEFAULT_DISABLE;
    SpiTxRx(ADS1x9x_Lead_Off_Sens_N_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* LOFF - FLIP - Direccion 0x11 *******************
    ADS1x9x_Lead_Off_Flip_Register.Config.Input_1 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Flip_Register.Config.Input_2 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Flip_Register.Config.Input_3 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Flip_Register.Config.Input_4 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Flip_Register.Config.Input_5 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Flip_Register.Config.Input_6 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Flip_Register.Config.Input_7 = DEFAULT_DISABLE;
    ADS1x9x_Lead_Off_Flip_Register.Config.Input_8 = DEFAULT_DISABLE;
    SpiTxRx(ADS1x9x_Lead_Off_Flip_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    ADS_CS=1;

    
    // Ahora se escriben los otros registros, la "parte baja"
    ADS_CS=0;
    SpiTxRx(ADS_WRITE|GPIO_ADS); // Direccion inicial de escritura
    SpiTxRx(ADS1x9x_BOTTOM_REGISTER_SIZE); // Indica la cantidad de registros a escribir
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* GPIO - Direccion 0x14 *******************
    ADS1x9x_GPIO_Settings_Register.Config.GPIO_1_Control = DEFAULT_INPUT;
    ADS1x9x_GPIO_Settings_Register.Config.GPIO_2_Control = DEFAULT_INPUT;
    ADS1x9x_GPIO_Settings_Register.Config.GPIO_3_Control = DEFAULT_INPUT;
    ADS1x9x_GPIO_Settings_Register.Config.GPIO_4_Control = DEFAULT_INPUT;
    ADS1x9x_GPIO_Settings_Register.Config.GPIO_Data = 0;
    SpiTxRx(ADS1x9x_GPIO_Settings_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* PACE - Direccion 0x15 *******************
    ADS1x9x_Pace_Register.Config.Pace_Detect_Buffer = DEFAULT_PACE_OFF;
    ADS1x9x_Pace_Register.Config.Odd_Pace_Channel_Select = PACE_CHANNEL_IS_5;
    ADS1x9x_Pace_Register.Config.Even_Pace_Channel_Select = PACE_CHANNEL_IS_6;
    ADS1x9x_Pace_Register.Config.Pace_Reserved = PACE_RESERVED_VALUE;
    SpiTxRx(ADS1x9x_Pace_Register.Result); 
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Reservado - Direccion 0x16 *******************
    SpiTxRx(0x0); // Se escribe todo cero.
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* Config4 - Direccion 0x17 *******************
    ADS1x9x_Config_4_Register.Config.Config_4_Reserved_2 = CONFIG_4_RESERVED_2_VALUE;
    ADS1x9x_Config_4_Register.Config.Pulse_Mode = DEFAULT_CONTINUOUS_CONVERSION_MODE;
    ADS1x9x_Config_4_Register.Config.Wilson_Center_Terminal_to_Right_Leg_Detect = DEFAULT_WCT_TO_RLD_DISCONECTED;
    ADS1x9x_Config_4_Register.Config.Disable_Lead_Off_Comparators = DEFAULT_DISABLE_COMPARATORS;
    ADS1x9x_Config_4_Register.Config.Config_4_Reserved_1 = CONFIG_4_RESERVED_1_VALUE;
    SpiTxRx(ADS1x9x_Config_4_Register.Result);
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* WCT1 - Direccion 0x18  *******************
    SpiTxRx(0x0); // Falta hacer estructura
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    // ******************* WCT2 - Direccion 0x19 *******************
    SpiTxRx(0x0); // Falta hacer estructura
    for(i=0; i<ADS1x9x_SPI_WRITE_DELAY; i++); // Delay entre escrituras
    ADS_CS=1;
// ********************* FIN Configuracion **************************
    
    //---------Conversion continua ---------    
    ADS_START=1; // Comienza a convertir
    ADS_CS=0;
    SpiTxRx(RDATAC); // Conversion continua, se realiza lectura directa sin enviar comando de lectura (RDATA)
    ADS_CS=1;
    __delay_us(4);
}