#ifndef __ads119x_H
#define __ads119x_H

unsigned char mensaje_1[]="ADS Detectado.";
unsigned char mensaje_2[]="error: No se pudo detectar el dispositivo.";
unsigned char mensaje_3[]="error: No coincide la configuracion de los registros.";

//FUNCIONES------------------------
void Init_ads(void);

// DEFINE ADS macros-------------------------
#define WAKEUP		0x02
#define STANDBY		0x04
#define RESET_ADS	0x06
#define START		0x08
#define STOP		0x0A
#define RDATAC		0x10
#define SDATAC		0x11
#define RDATA		0x12
#define CALIB       0x1A

// ----Escritura de los REGISTROS ADS1198------------
#define DEVICE_ID   0x0
#define CONFIG1		0x01
#define CONFIG2		0x02
#define CONFIG3		0x03
#define LEAD_OFF    0x04
#define CH1SET		0x05
#define CH2SET		0x06
#define CH3SET		0x07
#define CH4SET		0x08
#define CH5SET		0x09
#define CH6SET		0x0A
#define CH7SET		0x0B
#define CH8SET		0x0C
#define RLD_SENSP   0x0D
#define RLD_SENSN   0x0E
#define LOFF_SENSP  0x0F
#define LOFF_SENSN  0x10
#define LOFF_FLIP   0x11
#define GPIO_ADS    0x14
#define PACE        0x15
#define CONFIG4		0x17
#define WCT1        0x18
#define WCT2        0x19

#define ADS_WRITE   0x40
#define ADS_READ    0x20
#define SINGLE_BYTE_READ_WRITE          0x00
#define ADS1x9x_SPI_WRITE_DELAY         0x04
#define ADS1x9x_TOP_REGISTER_SIZE       0x11  
#define ADS1x9x_BOTTOM_REGISTER_SIZE    0x06

// ******************** Estructuras de los registros ***************************
typedef struct
{
    unsigned char ID_Reserved:  5; // Reserved. El valor es 0xB
    unsigned char Revision_ID:  3; // Revision ID (REV_ID[3:1])
    // 0x4=ADS 1194, 0x5=ADS 1196 , 0x6=ADS 1198, 0x7=Reservado
} ADS1x9x_ID_Register_type;

// REVISION_ID bit field definition
#define ADS1x9x_REV 0xB6 // ADS 1198, ver comentario variable revision_id

// ******************** CONFIG 1 ***************************

typedef struct
{
    //LSB
    unsigned char Output_Data_Rate:         3; // Output Data Rate (DR[2:0])
    unsigned char Config_1_Reserved_1:      2; // Reserved
    unsigned char Oscillator_Clock_Output:  1; // CLKOUT Connection (CLK_EN)  
    unsigned char Readback_Mode:            1; // Daisy-chain/multiple readback mode (DAISY_EN')
    unsigned char Config_1_Reserved_2:      1; // Reserved
    //MSB
} ADS1x9x_Config_1_Register_struct;
#define CONFIG_1_RESERVED_VALUE_1 0 // CONFIG_1_RESERVED bit field definition
#define CONFIG_1_RESERVED_VALUE_2 0 // CONFIG_1_RESERVED bit field definition

union {
 unsigned char Result;
 ADS1x9x_Config_1_Register_struct Config;
} ADS1x9x_Config_1_Register;

enum Output_Data_Rate_Type
{
    // fmod = fclk/16 = 2 MHz/16 = 125 KHz -------> Datos cada:
    DEFAULT_MODULATION_FREQUENCY_DIVIDED_BY_16  = 0, // 125 us
    MODULATION_FREQUENCY_DIVIDED_BY_32          = 1, // 250 us
    MODULATION_FREQUENCY_DIVIDED_BY_64          = 2, // 500 us
    MODULATION_FREQUENCY_DIVIDED_BY_128         = 3, // 1 ms
    MODULATION_FREQUENCY_DIVIDED_BY_256         = 4, // 2 ms
    MODULATION_FREQUENCY_DIVIDED_BY_512         = 5, // 4 ms
    MODULATION_FREQUENCY_DIVIDED_BY_1024        = 6  // 8 ms
};

enum Readback_Mode_Type
{
    DEFAULT_DAISY_CHAIN_MODE    = 0, // Modo de lectura unica
    MULTIPLE_READBACK_MODE      = 1  // Modo de lecturas multiples
};

enum Clock_Connection_Type
{
    DEFAULT_OSCILLATOR_CLOCK_OUTPUT_DISABLED    = 0, // Salida del oscilador desactivada 
    OSCILLATOR_CLOCK_OUTPUT_ENABLED             = 1  // Activada
};

// ******************** CONFIG 2 ***************************

typedef struct
{
    //LSB
    unsigned char Test_Signal_Frequency:    2; // Test Signal Frequency (TEST_FREQ[1:0]):
    unsigned char Test_Signal_Amplitude:    1; // Test Signal Amplitude (TEST_AMP[1:0]):
    unsigned char Config_2_Reserved_1:      1; // Reservado
    unsigned char Test_Source:              1; // Test Source: interno o externo
    unsigned char Config_2_Reserved_2:      3; // Reservado
    //MSB
} ADS1x9x_Config_2_Register_struct;
#define CONFIG_2_RESERVED_VALUE_1   0 // CONFIG_2_RESERVED bit field definition
#define CONFIG_2_RESERVED_VALUE_2   1 // CONFIG_2_RESERVED bit field definition

union {
 unsigned char Result;
 ADS1x9x_Config_2_Register_struct Config;
} ADS1x9x_Config_2_Register;

enum Test_Source_Type
{
    DEFAULT_TEST_SIGNALS_ARE_DRIVEN_EXTERNALLY  = 0, // Señales de prueba generadas externamente
    TEST_SIGNALS_ARE_DRIVEN_INTERNALLY          = 1  // Generadas internamente
}; 

enum Test_Signal_Amplitude_Type
{
    DEFAULT_PLUS_MINUS_1_MV_TIMES_VREF_DIVIDED_BY_2_4   = 0, // 
    PLUS_MINUS_2_MV_TIMES_VREF_DIVIDED_BY_2_4           = 1  //
};

enum Test_Signal_Frequency_Type
{
    DEFAULT_PULSED_AT_CLOCK_FREQUENCY_DIVIDED_BY_2_TO_THE_21ST  = 0, // 2 Mhz / 2^21
    PULSED_AT_CLOCK_FREQUENCY_DIVIDED_BY_2_TO_THE_20TH          = 1, // 2 Mhz / 2^20
    NOT_USED                                                    = 2, // Sin usar
    AT_DC                                                       = 3  // Frecuencia 0, conectado a continua
};

// ******************** CONFIG 3 ***************************

typedef struct
{
    //LSB
    unsigned char Right_Leg_Detect_Lead_Off_Status:     1; // RLD Lead off status (RLD_STAT)
    unsigned char Right_Leg_Detect_Sense_Selection:     1; // RLD sense selection (RLD_LOFF_SENS)
    unsigned char Right_Leg_Detect_Power_Buffer:        1; // RLD Buffer Power (PD_RLD)
    unsigned char Right_Leg_Detect_Reference_Source:    1; // RLD Reference Select (RLDREF)
    unsigned char Right_Leg_Detect_Measurement:         1; // RLD Output Routed to ADC Mux (RLD_MEAS)
    unsigned char Reference_Voltage:                    1; // Reference Voltage (VREF_4V)
    unsigned char Config_3_Reserved:                    1; // Reserved - Needs to be written with 1
    unsigned char Internal_Reference_Buffer:            1; // Reference Voltage (PD_REFBUR')
    //MSB
} ADS1x9x_Config_3_Register_struct;
#define CONFIG_3_RESERVED_1_VALUE   1 // CONFIG_3_RESERVED_1 bit field definition

union {
 unsigned char Result;
 ADS1x9x_Config_3_Register_struct Config;
} ADS1x9x_Config_3_Register;

enum Power_Down_Reference_Buffer_Type
{
    DEFAULT_POWER_DOWN_INTERNAL_REFERENCE_BUFFER    = 0, // Buffer externo
    ENABLE_INTERNAL_REFERENCE_BUFFER                = 1  // Habilita el buffer interno de referencia
};

enum Reference_Voltage_Type
{
    DEFAULT_IS_SET_TO_2_4_VOLTS = 0, // Referencia de 2,4v
    IS_SET_TO_4_VOLTS           = 1  // Referencia de 4v
};

enum Right_Leg_Detect_Measurement_Type
{
    DEFAULT_IS_OPEN             = 0, // Medicion deshabilitada
    ROUTED_TO_MUX_SETTING_VREF  = 1  // Habilita la medicion de la señal RLD, ruteando la señal
    //  a traves de RLDIN, con el canal que tenga el valor b010=2 en MUX
}; 

enum Right_Leg_Detect_Reference_Source_Type
{
    DEFAULT_IS_FED_EXTERNALLY   = 0, // Referencia RLD externa.
    IS_FED_INTERNALLY           = 1  // Referencia RLD generada internamente: (AVDD-AVSS)/2
};

enum Right_Leg_Detect_Buffer_Power_Type
{
    DEFAULT_RLD_BUFFER_IS_POWERED_DOWN  = 0, // Buffer RLD apgado
    RLD_BUFFER_IS_ENABLED               = 1  // Buffer RLD encendido
};

enum Right_Leg_Detect_Sense_Selection_Type
{
    DEFAULT_RLD_SENSE_IS_DISABLED   = 0, // RLD Sense deshabilitado
    RLD_SENSE_IS_ENABLED            = 1  // Habilitado
};

enum Right_Leg_Detect_Lead_Off_Status_Type
{
    DEFAULT_RLD_IS_CONNECTED    = 0, // RLD Conetado
    RLD_IS_NOT_CONNECTED        = 1  // Desconectado
};

// ******************** LEAD OFF ***************************

typedef struct
{
    //LSB
    unsigned char Lead_Off_Frequency:               2; // Frecuencia de deteccion
    unsigned char Lead_Off_Current_Magnitude:       2; // Corriente de deteccion
    unsigned char Lead_Off_Detection_Mode:          1; // Modo de deteccion
    unsigned char Lead_Off_Comparator_Threshold:    3; // Umbral de deteccion
    //MSB
} ADS1x9x_Lead_Off_Control_Register_struct;

union {
 unsigned char Result;
 ADS1x9x_Lead_Off_Control_Register_struct Config;
} ADS1x9x_Lead_Off_Control_Register;

enum Lead_Off_Frequency_Type
{
    DEFAULT_NO_DETECTION    = 0, // Deteccion desactivada
    AC_LEAD_DETECTION       = 1, // Deteccion usando AC
    DO_NOT_USE              = 2, // No usar
    DC_LEAD_DETECTION       = 3  // Deteccion usando DC
};

enum Lead_Off_Current_Magnitude_Type
{
    DEFAULT_FOUR_nA = 0, // Corriente de deteccion de 4nA
    EIGHT_nA        = 1, // 8nA
    TWELVE_nA       = 2, // 12nA
    SIXTEEN_nA      = 3  // 16nA
};

enum Lead_Off_Detection_Mode_Type
{
    DEFAULT_CURRENT_SOURCE_MODE = 0,
    PULL_UP_DOWN_RESISTOR_MODE  = 1
};

enum Lead_Off_Comparator_Threshold_Type
{
    DEFAULT_NINETY_FIVE = 0, // 95% lado P, 5% lado N
    NINETY_TWO          = 1, // 92,5% lado P, 7,5% lado N
    NINETY              = 2, // 90% lado P, 10% lado N
    EIGHTY_SEVEN        = 3, // 87,5% lado P, 12,5% lado N
    EIGHTY_FIVE         = 4, // 85% lado P, 15% lado N
    EIGHTY              = 5, // 80% lado P, 20% lado N
    SEVENTY_FIVE        = 6, // 75% lado P, 25% lado N
    SEVENTY             = 7  // 70% lado P, 30% lado N
};

// ******************** CHnSET ***************************

typedef struct
{
    //LSB
    unsigned char Channel_Input_Is:             3; // Channel Input (MUX[2:0])
    unsigned char Channel_Setting_Reserved:     1; // Reserved (Must Write 0)
    unsigned char Programmable_Gain_Setting:    3; // PGA Gain (GAIN[2:0])
    unsigned char Power_Down_Channel:           1; // Power-Down (PD)
    //MSB
} ADS1x9x_Channel_Settings_Register_struct;
#define CHANNEL_RESERVED_VALUE 0 // Valor del bit reservado

union {
 unsigned char Result;
 ADS1x9x_Channel_Settings_Register_struct Config;
} ADS1x9x_Channel_Settings_Register;

enum Programmable_Gain_Setting_Type
{
    DEFAULT_GAIN_OF_6   = 0,  
    GAIN_OF_1           = 1, 
    GAIN_OF_2           = 2, 
    GAIN_OF_3           = 3,
    GAIN_OF_4           = 4, 
    GAIN_OF_8           = 5, 
    GAIN_OF_12          = 6 
};

enum Channel_Input_Is_Type
{
    DEFAULT_ADS1x9x_ELECTRODE_INPUT     = 0, // Conexion normal
    ADS1x9x_INPUT_SHORTED               = 1, // Entrada en corto, se usa para medicion de ruido o cuando el canal esta apagado
    ADS1x9x_RIGHT_LEG_DETECT            = 2, // Se usa en conjunto a RLD_MEAS del bit config3
    ADS1x9x_ONE_HALF_DIGITAL_SUPPLY     = 3, // Conectado a la fuente. Canal 1,2,5,6,7,8 a (AVDD-AVSS)/2. Canal 3,4 a DVDD/2.
    ADS1x9x_TEMPERATURE_SENSOR          = 4, // Sensor de temperatura
    ADS1x9x_TEST_SIGNAL                 = 5, // Señal de prueba
    ADS1x9x_RIGHT_LEG_DETECT_POSITIVE   = 6, // RLD conectada a la entrada P
    ADS1x9x_RIGHT_LEG_DETECT_NEGATIVE   = 7  // RLD conectada a la entrada N
};

enum Power_Down_Channel_Type
{
    DEFAULT_DISABLE_POWER_DOWN  = 0, // Canal encendido
    ENABLE_POWER_DOWN           = 1  // Apagado
};

// ******************** RLD SENS P ***************************

typedef struct
{
    //LSB
    unsigned char Input_1:    1; // Channel 1
    unsigned char Input_2:    1; // Channel 2
    unsigned char Input_3:    1; // Channel 3
    unsigned char Input_4:    1; // Channel 4
    unsigned char Input_5:    1; // Channel 5
    unsigned char Input_6:    1; // Channel 6
    unsigned char Input_7:    1; // Channel 7
    unsigned char Input_8:    1; // Channel 8
    //MSB
} ADS1x9x_Channel_Input_List_struct;

union {
 unsigned char Result;
 ADS1x9x_Channel_Input_List_struct Config;
} ADS1x9x_RLD_Sens_P_Register;

enum Channel_Input_Function_Type
{
    DEFAULT_DISABLE = 0,
    ENABLE          = 1
};

// ******************** RLD SENS N ***************************

union {
 unsigned char Result;
 ADS1x9x_Channel_Input_List_struct Config;
} ADS1x9x_RLD_Sens_N_Register;

// ******************** LOFF SENS P ***************************

union {
 unsigned char Result;
 ADS1x9x_Channel_Input_List_struct Config;
} ADS1x9x_Lead_Off_Sens_P_Register;

// ******************** LOFF SENS N ***************************

union {
 unsigned char Result;
 ADS1x9x_Channel_Input_List_struct Config;
} ADS1x9x_Lead_Off_Sens_N_Register;

// ******************** LOFF FLIP ***************************

union {
 unsigned char Result;
 ADS1x9x_Channel_Input_List_struct Config;
} ADS1x9x_Lead_Off_Flip_Register;

// ******************** GPIO ***************************

typedef struct
{
    //LSB
    unsigned char GPIO_1_Control:   1; // Control del pin 1 GPIO
    unsigned char GPIO_2_Control:   1; // Control del pin 2 GPIO
    unsigned char GPIO_3_Control:   1; // Control del pin 3 GPIO
    unsigned char GPIO_4_Control:   1; // Control del pin 4 GPIO
    unsigned char GPIO_Data:        4; // Contiene los datos de los pines, en caso que se configure como lectura
    // MSB
} ADS1x9x_GPIO_Settings_Register_struct;

union {
 unsigned char Result;
 ADS1x9x_GPIO_Settings_Register_struct Config;
} ADS1x9x_GPIO_Settings_Register;

enum GPIO_Control_Type
{
    OUTPUT          = 0, // Salida
    DEFAULT_INPUT   = 1  // Entrada
};

// ******************** PACE ***************************

typedef struct
{
    //LSB
    unsigned char Pace_Detect_Buffer:       1; // PACE detect buffer (PD_PACE')
    unsigned char Odd_Pace_Channel_Select:  2; // PACE_OUT1 odd (PACEO[1:0]
    unsigned char Even_Pace_Channel_Select: 2; // PACE_OUT2 even (PACEE[1:0])
    unsigned char Pace_Reserved:            4; // Reservado
    //MSB
} ADS1x9x_Pace_Register_struct;
#define PACE_RESERVED_VALUE 0 // Valor reservado

union {
 unsigned char Result;
 ADS1x9x_Pace_Register_struct Config;
} ADS1x9x_Pace_Register;

enum Pace_DetectBuffer
{
    DEFAULT_PACE_OFF    = 0, // Apagado
    PACE_ON             = 1  // Encendido
};

enum Pace_Channel_Select_Type
{
    DEFAULT_PACE_CHANNEL_IS_1                           = 0,  
    DEFAULT_PACE_CHANNEL_IS_2                           = 0, 
    PACE_CHANNEL_IS_3                                   = 1,  
    PACE_CHANNEL_IS_4                                   = 1, 
    PACE_CHANNEL_IS_5                                   = 2,  
    PACE_CHANNEL_IS_6                                   = 2, 
    PACE_CHANNEL_IS_7                                   = 3,  
    PACE_CHANNEL_IS_8                                   = 3 
};

// ******************** CONFIG 4 ***************************

typedef struct
{
    //LSB
    unsigned char Config_4_Reserved_1:                          1; // Reservado
    unsigned char Disable_Lead_Off_Comparators:                 1; // Controla los comparadores para deteccion de desconexion
    unsigned char Wilson_Center_Terminal_to_Right_Leg_Detect:   1; // Conectar el terminal de wilson a RLD
    unsigned char Pulse_Mode:                                   1; // Modo de conversion: continua o pulso
    unsigned char Config_4_Reserved_2:                          4; // Reservado
    //MSB
} ADS1x9x_Config_4_Register_struct;
#define CONFIG_4_RESERVED_1_VALUE   0 // CONFIG_4_RESERVED bit field definition
#define CONFIG_4_RESERVED_2_VALUE   0 // CONFIG_4_RESERVED bit field definition

union {
 unsigned char Result;
 ADS1x9x_Config_4_Register_struct Config;
} ADS1x9x_Config_4_Register;

enum Disable_Lead_Off_Comparators_Type
{
    DEFAULT_DISABLE_COMPARATORS = 0, // Comparadores para deteccion de desconexion de electroso desactivados
    COMPARATORS_ENABLED         = 1  // Activados
};

enum Wilson_Center_Terminal_to_Right_Leg_Detect_Type
{   
    DEFAULT_WCT_TO_RLD_DISCONECTED  = 0, // WCT no conectado a RLD buffer
    WTC_TO_RLD                      = 1  // Conectado.
};

enum Pulse_Mode_Type
{
    DEFAULT_CONTINUOUS_CONVERSION_MODE  = 0, // Habilita conversion continua
    SINGLE_SHOT_MODE                    = 1  // Conversion a demanda
};
#endif