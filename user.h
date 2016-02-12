/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define MRF24J40_RESET LATAbits.LATA0 //salida
#define MRF24J40_CS LATAbits.LATA1 //salida
#define INDICADOR LATAbits.LATA2//salida
//#define MRF24J40_WAKE LATCbits.LATC0
#define ADS_DRDY	PORTBbits.RB0 // entrada
#define MRF24J40_INT PORTBbits.RB1 // entrada
#define ADS_START   LATBbits.LATB4 // salida
#define ADS_CS		LATCbits.LATC0 // salida

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);         /* I/O and Peripheral Initialization */
void Init_SPI(void);
unsigned char SpiTxRx (unsigned char data); // Envia y recibe los datos por SPI
void InitIRQ(void); // Inicializa interrupciones