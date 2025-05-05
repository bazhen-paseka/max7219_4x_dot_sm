/**
* \file
* \version 1.0
* \author bazhen.levkovets
** \date 2018
*
*************************************************************************************
* \copyright	Bazhen Levkovets
* \copyright	Brovary, Kyiv region
* \copyright	Ukraine
*
*************************************************************************************
*
* \brief
*
*/
	#ifndef MAX7219_4X_DOT_SM_H_INCLUDED
	#define MAX7219_4X_DOT_SM_H_INCLUDED
/*
**************************************************************************
*								INCLUDE FILES
**************************************************************************
*/
	#include "main.h"
	#include <stdlib.h>
	#include "Local_config.h"
/*
**************************************************************************
*								    DEFINES
**************************************************************************
*/
	#define	WRITE_STROB_DELAY		10
	#define SPI_PACKAGE_TIMEOUT		3
	//	#define SPI_PACKAGE_TIMEOUT		1	only for random picture

	#define DIGIT_QNT_MAX 			11
	#define PANEL_QNT				4
	#define LINE_IN_PANEL			8
	#define DOT_IN_LINE				8
	#define BYTE_IN_SPI_PACKAGE		8
/*
**************************************************************************
*								    STRUCT
**************************************************************************
*/

typedef struct {
	SPI_HandleTypeDef	*spi;
	GPIO_TypeDef 		*cs_port;
    uint16_t 			 cs_pin;
    uint8_t				data [LINE_IN_PANEL];
	uint8_t				panel[PANEL_QNT][LINE_IN_PANEL];
} 		max7219_struct;
//----------------------------------

typedef enum{
	DisplayDigit_0_only	=	0x00 ,
	DisplayDigit_0_1	=	0x01 ,
	DisplayDigit_0_2	=	0x02 ,
	DisplayDigit_0_3	=	0x03 ,
	DisplayDigit_0_4	=	0x04 ,
	DisplayDigit_0_5	=	0x05 ,
	DisplayDigit_0_6	=	0x06 ,
	DisplayDigit_0_7	=	0x07
}		max7219_Scan_Limit ;

/*
**************************************************************************
*								    ENUM
**************************************************************************
*/

typedef enum {
	Intensity_1		= 0x00 ,
	Intensity_3		= 0x01 ,
	Intensity_5		= 0x02 ,
	Intensity_7		= 0x03 ,
	Intensity_9		= 0x04 ,
	Intensity_11	= 0x05 ,
	Intensity_13	= 0x06 ,
	Intensity_15	= 0x07 ,
	Intensity_17	= 0x08 ,
	Intensity_19	= 0x09 ,
	Intensity_21	= 0x0A ,
	Intensity_23	= 0x0B ,
	Intensity_25	= 0x0C ,
	Intensity_27	= 0x0D ,
	Intensity_29	= 0x0E ,
	Intensity_31	= 0x0F
}		max7219_LED_Intensity ;
//----------------------------------

typedef enum
{
	ShutdownMode	= 0 ,
	NormalOperation	= 1
}		max7219_Shutdown ;
//----------------------------------

typedef enum
{
	WorkMode	= 0,
	TestMode	= 1
}		max7219_Display_Test ;
//----------------------------------

typedef enum
{
	NoDecode			= 0x00 ,
	DecodeForDigit_0	= 0x01 ,
	DecodeForDigit_1_0	= 0x03 ,
	DecodeForDigit_2_0	= 0x07 ,
	DecodeForDigit_3_0	= 0x0F ,
	DecodeForDigit_4_0	= 0x1F ,
	DecodeForDigit_5_0	= 0x3F ,
	DecodeForDigit_6_0	= 0x7F ,
	DecodeForDigit_7_0	= 0xFF ,
}		max7219_Decode_Mode ;

/*
**************************************************************************
*                        GLOBAL FUNCTION PROTOTYPES
**************************************************************************
*/

void max7219_init	( max7219_struct 		*max7219_handler	,
					max7219_Decode_Mode 	_decodemode			,
					max7219_LED_Intensity	_intensity			,
					max7219_Scan_Limit 		_scanlimit	 		,
					max7219_Shutdown 		_shutdown 			) ;

void Max7219_struct_init (	max7219_struct 		*handler		,
							SPI_HandleTypeDef	*_spi			,
							GPIO_TypeDef 		*_cs_port		,
							uint16_t 			 _cs_pin		) ;

void max7219_show_time	( max7219_struct	*max7219_handler 	,
						uint8_t 			_hour 				,
						uint8_t 			_minutes 			) ;

void max7219_test_LED	( max7219_struct 	*max7219_handler 	,
						uint32_t 			_time_ms_u32 		) ;

/*
**************************************************************************
*                                   END
**************************************************************************
*/

#endif 	//	MAX7219_4X_DOT_SM_H_INCLUDED
