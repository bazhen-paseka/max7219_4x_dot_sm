/**
* \file	 max7219-4x-dot-sm.h
* \version 1.0
* \author bazhen.levkovets
** \date 2021-03-06
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

/*
**************************************************************************
*								INCLUDE FILES
**************************************************************************
*/
	#include "max7219_4x_dot_sm.h"
/*
**************************************************************************
*								    DEFINES
**************************************************************************
*/

/*
**************************************************************************
*						        MACROS
**************************************************************************
*/
	#define BIT_SET(   byte , pos )		((byte) |=  (1UL << (pos)))
	#define BIT_CLR(   byte , pos )		((byte) &= ~(1UL << (pos)))
	#define BIT_TOGGLE(byte , pos )		((byte) ^=  (1UL << (pos)))
	#define BIT_CHECK( byte , pos )		((byte) &   (1UL << (pos)))
/*
**************************************************************************
*						    LOCAL DATA TYPES
**************************************************************************
*/

/*
**************************************************************************
*								    ENUM
**************************************************************************
*/
typedef enum {
	ADDR_NO_OP 			= 0x00 ,
	ADDR_DIGIT_0		= 0x01 ,
	ADDR_DIGIT_1 		= 0x02 ,
	ADDR_DIGIT_2 		= 0x03 ,
	ADDR_DIGIT_3 		= 0x04 ,
	ADDR_DIGIT_4 		= 0x05 ,
	ADDR_DIGIT_5 		= 0x06 ,
	ADDR_DIGIT_6 		= 0x07 ,
	ADDR_DIGIT_7		= 0x08 ,
	ADDR_DECODE_MODE 	= 0x09 ,
	ADDR_INTENSITY 		= 0x0A ,
	ADDR_SCAN_LIMIT 	= 0x0B ,
	ADDR_SHUTDOWN		= 0x0C ,
	ADDR_EMPTY			= 0x0D ,
	ADDR_DISPLAY_TEST	= 0x0F
}		Register_Address_Map	;

/*
**************************************************************************
*                              FUNCTION PROTOTYPES
**************************************************************************
*/
	void _max7219_print_one_digit	( max7219_struct 	_max7219_handler 	,
									uint8_t 			_position 			,
									uint32_t 			_value 				) ;

	void _max7219_show_all			( max7219_struct 	_max7219_handler	) ;
	void _max7219_write_strob		( max7219_struct 	_max7219_handler	) ;
	void _max7219_push_data			( max7219_struct 	_max7219_handler	) ;
	uint8_t inverse_order_in_byte	( uint8_t 			_input 				) ;
	void Local_delay				( uint32_t 			_delay_u32			) ;
/*
**************************************************************************
*                        LOCAL FUNCTION PROTOTYPES
**************************************************************************
*/

/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/

void Max7219_struct_init (	max7219_struct 		*handler	,
							SPI_HandleTypeDef	*_spi		,
							GPIO_TypeDef 		*_cs_port	,
							uint16_t 			 _cs_pin	) {

	handler->spi		= _spi		;
	handler->cs_port	= _cs_port	;
	handler->cs_pin		= _cs_pin	;
}
/***************************************************************************************/

void max7219_init(	max7219_struct 			*_max7219_handler	,
					max7219_Decode_Mode 	_decodemode			,
					max7219_LED_Intensity	_intensity			,
					max7219_Scan_Limit 		_scanlimit			,
					max7219_Shutdown 		_shutdown 			) {

	// test - Off
	_max7219_handler->data[0] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[1] = WorkMode ;
	_max7219_handler->data[2] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[3] = WorkMode ;
	_max7219_handler->data[4] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[5] = WorkMode ;
	_max7219_handler->data[6] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[7] = WorkMode ;
	_max7219_push_data( *_max7219_handler ) ;

	// Decode Mode - No. 1 in 1
	_max7219_handler->data[0] = ADDR_DECODE_MODE ;  _max7219_handler->data[1] = _decodemode ;
	_max7219_handler->data[2] = ADDR_DECODE_MODE ;  _max7219_handler->data[3] = _decodemode ;
	_max7219_handler->data[4] = ADDR_DECODE_MODE ;  _max7219_handler->data[5] = _decodemode ;
	_max7219_handler->data[6] = ADDR_DECODE_MODE ;  _max7219_handler->data[7] = _decodemode ;
	_max7219_push_data( *_max7219_handler ) ;

	// Intensity x/32
	_max7219_handler->data[0] = ADDR_INTENSITY ;  _max7219_handler->data[1] = _intensity ;
	_max7219_handler->data[2] = ADDR_INTENSITY ;  _max7219_handler->data[3] = _intensity ;
	_max7219_handler->data[4] = ADDR_INTENSITY ;  _max7219_handler->data[5] = _intensity ;
	_max7219_handler->data[6] = ADDR_INTENSITY ;  _max7219_handler->data[7] = _intensity ;
	_max7219_push_data( *_max7219_handler ) ;

	//Scan Limit - All
	_max7219_handler->data[0] = ADDR_SCAN_LIMIT ;  _max7219_handler->data[1] = _scanlimit ;
	_max7219_handler->data[2] = ADDR_SCAN_LIMIT ;  _max7219_handler->data[3] = _scanlimit ;
	_max7219_handler->data[4] = ADDR_SCAN_LIMIT ;  _max7219_handler->data[5] = _scanlimit ;
	_max7219_handler->data[6] = ADDR_SCAN_LIMIT ;  _max7219_handler->data[7] = _scanlimit ;
	_max7219_push_data( *_max7219_handler ) ;

	// Shutdown - none
	// myTrans[1] -> 00 sleep
	// myTrans[1] -> 01 work
	_max7219_handler->data[0] = ADDR_SHUTDOWN;  _max7219_handler->data[1] = _shutdown;
	_max7219_handler->data[2] = ADDR_SHUTDOWN;  _max7219_handler->data[3] = _shutdown;
	_max7219_handler->data[4] = ADDR_SHUTDOWN;  _max7219_handler->data[5] = _shutdown;
	_max7219_handler->data[6] = ADDR_SHUTDOWN;  _max7219_handler->data[7] = _shutdown;
	_max7219_push_data( *_max7219_handler ) ;
}
/***************************************************************************************/

void max7219_show_time(	max7219_struct	*_max7219_handler	,
						uint8_t 		_hour				,
						uint8_t 		_minut				) {

	uint8_t 	digit [DIGIT_QNT_MAX] [LINE_IN_PANEL] ;

	digit[0][7] = 0b00011111 ;
	digit[0][6] = 0b00010001 ;
	digit[0][5] = 0b00010001 ;
	digit[0][4] = 0b00010001 ;
	digit[0][3] = 0b00010001 ;
	digit[0][2] = 0b00010001 ;
	digit[0][1] = 0b00010001 ;
	digit[0][0] = 0b00011111 ;

	digit[1][7] = 0b00000100 ;
	digit[1][6] = 0b00001100 ;
	digit[1][5] = 0b00010100 ;
	digit[1][4] = 0b00000100 ;
	digit[1][3] = 0b00000100 ;
	digit[1][2] = 0b00000100 ;
	digit[1][1] = 0b00000100 ;
	digit[1][0] = 0b00011111 ;

	digit[2][7] = 0b00011111 ;
	digit[2][6] = 0b00000001 ;
	digit[2][5] = 0b00000001 ;
	digit[2][4] = 0b00011111 ;
	digit[2][3] = 0b00010000 ;
	digit[2][2] = 0b00010000 ;
	digit[2][1] = 0b00010000 ;
	digit[2][0] = 0b00011111 ;

	digit[3][7] = 0b00011111 ;
	digit[3][6] = 0b00000001 ;
	digit[3][5] = 0b00000001 ;
	digit[3][4] = 0b00011111 ;
	digit[3][3] = 0b00000001 ;
	digit[3][2] = 0b00000001 ;
	digit[3][1] = 0b00000001 ;
	digit[3][0] = 0b00011111 ;

	digit[4][7] = 0b00010001 ;
	digit[4][6] = 0b00010001 ;
	digit[4][5] = 0b00010001 ;
	digit[4][4] = 0b00011111 ;
	digit[4][3] = 0b00000001 ;
	digit[4][2] = 0b00000001 ;
	digit[4][1] = 0b00000001 ;
	digit[4][0] = 0b00000001 ;

	digit[5][7] = 0b00011111 ;
	digit[5][6] = 0b00010000 ;
	digit[5][5] = 0b00010000 ;
	digit[5][4] = 0b00011111 ;
	digit[5][3] = 0b00000001 ;
	digit[5][2] = 0b00000001 ;
	digit[5][1] = 0b00000001 ;
	digit[5][0] = 0b00011111 ;

	digit[6][7] = 0b00011111 ;
	digit[6][6] = 0b00010000 ;
	digit[6][5] = 0b00010000 ;
	digit[6][4] = 0b00011111 ;
	digit[6][3] = 0b00010001 ;
	digit[6][2] = 0b00010001 ;
	digit[6][1] = 0b00010001 ;
	digit[6][0] = 0b00011111 ;

	digit[7][7] = 0b00011111 ;
	digit[7][6] = 0b00000001 ;
	digit[7][5] = 0b00000001 ;
	digit[7][4] = 0b00000001 ;
	digit[7][3] = 0b00000001 ;
	digit[7][2] = 0b00000001 ;
	digit[7][1] = 0b00000001 ;
	digit[7][0] = 0b00000001 ;

	digit[8][7] = 0b00011111 ;
	digit[8][6] = 0b00010001 ;
	digit[8][5] = 0b00010001 ;
	digit[8][4] = 0b00011111 ;
	digit[8][3] = 0b00010001 ;
	digit[8][2] = 0b00010001 ;
	digit[8][1] = 0b00010001 ;
	digit[8][0] = 0b00011111 ;

	digit[9][7] = 0b00011111 ;
	digit[9][6] = 0b00010001 ;
	digit[9][5] = 0b00010001 ;
	digit[9][4] = 0b00011111 ;
	digit[9][3] = 0b00000001 ;
	digit[9][2] = 0b00000001 ;
	digit[9][1] = 0b00000001 ;
	digit[9][0] = 0b00011111 ;

	digit[10][7] = 0b01000001 ;
	digit[10][6] = 0b01000001 ;
	digit[10][5] = 0b01000001 ;
	digit[10][4] = 0b01000001 ;
	digit[10][3] = 0b01000001 ;
	digit[10][2] = 0b00100010 ;
	digit[10][1] = 0b00010100 ;
	digit[10][0] = 0b00001001 ;

	for (uint8_t line=0; line<8; line++)	{
		_max7219_handler->panel[0][line] = digit[_hour/10] [line] << 1 ;	//	We shift the digits from the right edge
		_max7219_handler->panel[1][line] = digit[_hour%10] [line] << 2 ;	//		and bringing digits together.
		_max7219_handler->panel[2][line] = digit[_minut/10][line] << 1 ;
		_max7219_handler->panel[3][line] = digit[_minut%10][line] << 2 ;
	}

	#ifdef RANDOM_DOT
	uint8_t random_qnt_u8 = 1 + (uint8_t) rand() % RANDOM_ERROR_QNT ;
	for (uint8_t r=0; r<random_qnt_u8; r++) {
		uint8_t random_panel_u8 = (uint8_t) rand() % PANEL_QNT ;
		uint8_t random_lines_u8 = (uint8_t) rand() % LINE_IN_PANEL ;
		uint8_t random_point_u8 = (uint8_t) rand() % DOT_IN_LINE ;
		BIT_TOGGLE( _max7219_handler->panel[random_panel_u8][random_lines_u8] , random_point_u8) ;
	}
	#endif
	_max7219_show_all( *_max7219_handler ) ;
}
/***************************************************************************************/

void _max7219_show_all( max7219_struct		_max7219_handler ) {

//	uint8_t myTrans[8]; // 1 - for  razryad or Adres; 2- znachenie
	for (uint8_t i=0; i<8; i++) {
		uint8_t addr_u8 = i + ADDR_DIGIT_0;
		_max7219_handler.data[6] = addr_u8 ;  _max7219_handler.data[7] = inverse_order_in_byte(_max7219_handler.panel[0][i] ) ;
		_max7219_handler.data[4] = addr_u8 ;  _max7219_handler.data[5] = inverse_order_in_byte(_max7219_handler.panel[1][i] ) ;
		_max7219_handler.data[2] = addr_u8 ;  _max7219_handler.data[3] = inverse_order_in_byte(_max7219_handler.panel[2][i] ) ;
		_max7219_handler.data[0] = addr_u8 ;  _max7219_handler.data[1] = inverse_order_in_byte(_max7219_handler.panel[3][i] ) ;
		_max7219_push_data( _max7219_handler ) ;
	}
}
/***************************************************************************************/

uint8_t inverse_order_in_byte (uint8_t 		input) {

    uint8_t var_u8 =((input & 0x01) << 7) |
					((input & 0x02) << 5) |
					((input & 0x04) << 3) |
					((input & 0x08) << 1) |
					((input & 0x10) >> 1) |
					((input & 0x20) >> 3) |
					((input & 0x40) >> 5) |
					((input & 0x80) >> 7) ;
    return var_u8 ;
}
/***************************************************************************************/

void max7219_test_LED(	max7219_struct 	*_max7219_handler	,
						uint32_t 		_time_ms_u32			) {
	// test - On
	_max7219_handler->data[0] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[1] = TestMode ;
	_max7219_handler->data[2] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[3] = TestMode ;
	_max7219_handler->data[4] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[5] = TestMode ;
	_max7219_handler->data[6] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[7] = TestMode ;
	_max7219_push_data( *_max7219_handler ) ;

	HAL_Delay ( _time_ms_u32 ) ;

	// test - Off
	_max7219_handler->data[0] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[1] = WorkMode ;
	_max7219_handler->data[2] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[3] = WorkMode ;
	_max7219_handler->data[4] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[5] = WorkMode ;
	_max7219_handler->data[6] = ADDR_DISPLAY_TEST ;  _max7219_handler->data[7] = WorkMode ;
	_max7219_push_data( *_max7219_handler ) ;
}
/***************************************************************************************/

void _max7219_push_data( max7219_struct 	_max7219_handler ) {

    HAL_SPI_Transmit( _max7219_handler.spi , _max7219_handler.data , BYTE_IN_SPI_PACKAGE , SPI_PACKAGE_TIMEOUT ) ;
    _max7219_write_strob( _max7219_handler ) ;
}
/***************************************************************************************/

void _max7219_write_strob( max7219_struct 	_max7219_handler ) {

	HAL_GPIO_WritePin( _max7219_handler.cs_port , _max7219_handler.cs_pin , SET ) ;
	//HAL_Delay( 1 ) ;
	Local_delay( WRITE_STROB_DELAY ) ;
	HAL_GPIO_WritePin( _max7219_handler.cs_port , _max7219_handler.cs_pin , RESET ) ;
}
/***************************************************************************************/

void Local_delay ( uint32_t _delay_u32 ) {

	for ( ; _delay_u32 > 0; _delay_u32-- ) {
		__asm( "nop" ) ;
	}
}
/***************************************************************************************/

void _max7219_print_one_digit(	max7219_struct 	_max7219_handler	,
								uint8_t 		_position				,
								uint32_t 		_value					) {

	uint8_t spi_buffer[2]		;
	spi_buffer[0] = _position	; // razryad
	spi_buffer[1] = _value		; // znachenie
	HAL_SPI_Transmit( _max7219_handler.spi , spi_buffer , BYTE_IN_SPI_PACKAGE , SPI_PACKAGE_TIMEOUT ) ;
	_max7219_write_strob( _max7219_handler ) ;
}
/*
**************************************************************************
*                                   END
**************************************************************************
*/
