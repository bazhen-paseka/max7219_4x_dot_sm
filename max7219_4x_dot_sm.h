#ifndef MAX7219_4X_DOT_SM_H_INCLUDED
#define MAX7219_4X_DOT_SM_H_INCLUDED

typedef struct
{
	SPI_HandleTypeDef	*spi;
	GPIO_TypeDef 		*cs_port;
    uint16_t 			 cs_pin;
    uint8_t				data[8];
} max7219_struct;
/***********************************/

typedef struct
{
	uint8_t				kub_0[9];
	uint8_t				kub_1[9];
	uint8_t				kub_2[9];
	uint8_t				kub_3[9];
}	max7219_map;

typedef enum
{
	DisplayDigit_0_only	=	0x00,
	DisplayDigit_0_1	=	0x01,
	DisplayDigit_0_2	=	0x02,
	DisplayDigit_0_3	=	0x03,
	DisplayDigit_0_4	=	0x04,
	DisplayDigit_0_5	=	0x05,
	DisplayDigit_0_6	=	0x06,
	DisplayDigit_0_7	=	0x07
}	 max7219_Scan_Limit;
/***********************************/

typedef enum
{
	Intensity_1		= 0x00,
	Intensity_3		= 0x01,
	Intensity_5		= 0x02,
	Intensity_7		= 0x03,
	Intensity_9		= 0x04,
	Intensity_11	= 0x05,
	Intensity_13	= 0x06,
	Intensity_15	= 0x07,
	Intensity_17	= 0x08,
	Intensity_19	= 0x09,
	Intensity_21	= 0x0A,
	Intensity_23	= 0x0B,
	Intensity_25	= 0x0C,
	Intensity_27	= 0x0D,
	Intensity_29	= 0x0E,
	Intensity_31	= 0x0F
}	 max7219_LED_Intensity;
/***********************************/

typedef enum
{
	ShutdownMode	= 0,
	NormalOperation	= 1
}	max7219_Shutdown;
/***********************************/

typedef enum
{
	WorkMode	= 0,
	TestMode	= 1
}	max7219_Display_Test ;
/***********************************/

typedef enum
{
	NoDecode			= 0x00,
	DecodeForDigit_0	= 0x01,
	DecodeForDigit_1_0	= 0x03,
	DecodeForDigit_2_0	= 0x07,
	DecodeForDigit_3_0	= 0x0F,
	DecodeForDigit_4_0	= 0x1F,
	DecodeForDigit_5_0	= 0x3F,
	DecodeForDigit_6_0	= 0x7F,
	DecodeForDigit_7_0	= 0xFF,
}	 max7219_Decode_Mode;
/***********************************/

void max7219_init(max7219_struct *max7219_handler, max7219_Decode_Mode _decodemode,  max7219_LED_Intensity _intensity, max7219_Scan_Limit _scanlimit, max7219_Shutdown _shutdown);
void max7219_show_all(max7219_struct *max7219_handler, 	max7219_map max7219_map_handler);
/***********************************/

#endif 	//	MAX7219_4X_DOT_SM_H_INCLUDED
