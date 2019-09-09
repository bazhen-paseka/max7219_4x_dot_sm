
#include "main.h"
#include "max7219_4x_dot_sm.h"

#define BYTE_IN_SPI_PACKAGE		8
#define SPI_PACKAGE_TIMEOUT		1

typedef enum
{
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
}	Register_Address_Map;
/***************************************************************************************/

void _max7219_print_one_digit(max7219_struct max7219_handler, uint8_t position, uint32_t value);
void _max7219_show_all(max7219_struct max7219_handler);
void _max7219_write_strob(max7219_struct max7219_handler);
void _max7219_push_data(max7219_struct max7219_handler);
uint8_t inverse_order_in_byte (uint8_t input);

/***************************************************************************************/

void max7219_init(max7219_struct *max7219_handler, max7219_Decode_Mode _decodemode,  max7219_LED_Intensity _intensity, max7219_Scan_Limit _scanlimit, max7219_Shutdown _shutdown)
{
	// test - On
	max7219_handler->data[0] = ADDR_DISPLAY_TEST;  max7219_handler->data[1] = TestMode;
	max7219_handler->data[2] = ADDR_DISPLAY_TEST;  max7219_handler->data[3] = TestMode;
	max7219_handler->data[4] = ADDR_DISPLAY_TEST;  max7219_handler->data[5] = TestMode;
	max7219_handler->data[6] = ADDR_DISPLAY_TEST;  max7219_handler->data[7] = TestMode;
	_max7219_push_data(*max7219_handler);
	HAL_Delay(200);

	// test - Off
	max7219_handler->data[0] = ADDR_DISPLAY_TEST;  max7219_handler->data[1] = WorkMode;
	max7219_handler->data[2] = ADDR_DISPLAY_TEST;  max7219_handler->data[3] = WorkMode;
	max7219_handler->data[4] = ADDR_DISPLAY_TEST;  max7219_handler->data[5] = WorkMode;
	max7219_handler->data[6] = ADDR_DISPLAY_TEST;  max7219_handler->data[7] = WorkMode;
	_max7219_push_data(*max7219_handler);

	// Decode Mode - No. 1 in 1
	max7219_handler->data[0] = ADDR_DECODE_MODE;  max7219_handler->data[1] = _decodemode;
	max7219_handler->data[2] = ADDR_DECODE_MODE;  max7219_handler->data[3] = _decodemode;
	max7219_handler->data[4] = ADDR_DECODE_MODE;  max7219_handler->data[5] = _decodemode;
	max7219_handler->data[6] = ADDR_DECODE_MODE;  max7219_handler->data[7] = _decodemode;
	_max7219_push_data(*max7219_handler);

	// Intensity 3/32
	max7219_handler->data[0] = ADDR_INTENSITY;  max7219_handler->data[1] = _intensity;
	max7219_handler->data[2] = ADDR_INTENSITY;  max7219_handler->data[3] = _intensity;
	max7219_handler->data[4] = ADDR_INTENSITY;  max7219_handler->data[5] = _intensity;
	max7219_handler->data[6] = ADDR_INTENSITY;  max7219_handler->data[7] = _intensity;
	_max7219_push_data(*max7219_handler);

	//Scan Limit - All
	max7219_handler->data[0] = ADDR_SCAN_LIMIT;  max7219_handler->data[1] = _scanlimit;
	max7219_handler->data[2] = ADDR_SCAN_LIMIT;  max7219_handler->data[3] = _scanlimit;
	max7219_handler->data[4] = ADDR_SCAN_LIMIT;  max7219_handler->data[5] = _scanlimit;
	max7219_handler->data[6] = ADDR_SCAN_LIMIT;  max7219_handler->data[7] = _scanlimit;
	_max7219_push_data(*max7219_handler);

	// Shutdown - none
	// myTrans[1] -> 00 sleep
	// myTrans[1] -> 01 work
	max7219_handler->data[0] = ADDR_SHUTDOWN;  max7219_handler->data[1] = _shutdown;
	max7219_handler->data[2] = ADDR_SHUTDOWN;  max7219_handler->data[3] = _shutdown;
	max7219_handler->data[4] = ADDR_SHUTDOWN;  max7219_handler->data[5] = _shutdown;
	max7219_handler->data[6] = ADDR_SHUTDOWN;  max7219_handler->data[7] = _shutdown;
	_max7219_push_data(*max7219_handler);

}
/***************************************************************************************/

void _max7219_print_one_digit(max7219_struct max7219_handler, uint8_t position, uint32_t value)
{
	uint8_t spi_buffer[2];
	spi_buffer[0] = position	; // razryad
	spi_buffer[1] = value	; // znachenie
	HAL_SPI_Transmit(max7219_handler.spi, spi_buffer, BYTE_IN_SPI_PACKAGE, SPI_PACKAGE_TIMEOUT);
	_max7219_write_strob(max7219_handler);
}
/***************************************************************************************/

void _max7219_push_data(max7219_struct max7219_handler)
{
    HAL_SPI_Transmit(max7219_handler.spi, max7219_handler.data, BYTE_IN_SPI_PACKAGE, SPI_PACKAGE_TIMEOUT);
    _max7219_write_strob(max7219_handler);
}
/***************************************************************************************/

void _max7219_write_strob(max7219_struct max7219_handler)
{
	HAL_GPIO_WritePin(max7219_handler.cs_port, max7219_handler.cs_pin, SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(max7219_handler.cs_port, max7219_handler.cs_pin, RESET);
}
/***************************************************************************************/

void _max7219_show_all(max7219_struct max7219_handler)
{
//	uint8_t myTrans[8]; // 1 - for  razryad or Adres; 2- znachenie

	for (uint8_t i=0; i<8; i++)
	{
		uint8_t addr_u8 = i + ADDR_DIGIT_0;
		max7219_handler.data[6] = addr_u8;  max7219_handler.data[7] = inverse_order_in_byte(max7219_handler.kub_0[i]) ;
		max7219_handler.data[4] = addr_u8;  max7219_handler.data[5] = inverse_order_in_byte(max7219_handler.kub_1[i]) ;
		max7219_handler.data[2] = addr_u8;  max7219_handler.data[3] = inverse_order_in_byte(max7219_handler.kub_2[i]) ;
		max7219_handler.data[0] = addr_u8;  max7219_handler.data[1] = inverse_order_in_byte(max7219_handler.kub_3[i]) ;
		_max7219_push_data(max7219_handler);
	}
}

uint8_t inverse_order_in_byte (uint8_t input)
{
    uint8_t v2 =	((input & 0x01) << 7) |
					((input & 0x02) << 5) |
					((input & 0x04) << 3) |
					((input & 0x08) << 1) |
					((input & 0x10) >> 1) |
					((input & 0x20) >> 3) |
					((input & 0x40) >> 5) |
					((input & 0x80) >> 7);
    return v2;
}

void max7219_show_time(max7219_struct *max7219_handler, uint8_t _hour, uint8_t _minut)
{
	uint8_t digit[DIGIT_QNT_MAX][LINE_IN_DIGIT];

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

	for (uint8_t i=0; i<8; i++)
	{
		max7219_handler->kub_0[i] = digit[_hour/10][i]  <<1 ;
		max7219_handler->kub_1[i] = digit[_hour%10][i]  <<2 ;
		max7219_handler->kub_2[i] = digit[_minut/10][i] <<1 ;
		max7219_handler->kub_3[i] = digit[_minut%10][i] <<2 ;
	}

	_max7219_show_all(*max7219_handler);
}
