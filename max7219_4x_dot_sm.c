
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
void _max7219_write_strob(max7219_struct max7219_handler);
void _max7219_push_data(max7219_struct max7219_handler);

/***************************************************************************************/

void max7219_init(max7219_struct *max7219_handler, max7219_Decode_Mode _decodemode,  max7219_LED_Intensity _intensity, max7219_Scan_Limit _scanlimit, max7219_Shutdown _shutdown)
{
	// test - On
	max7219_handler->data[0] = ADDR_DISPLAY_TEST;  max7219_handler->data[1] = TestMode;
	max7219_handler->data[2] = ADDR_DISPLAY_TEST;  max7219_handler->data[3] = TestMode;
	max7219_handler->data[4] = ADDR_DISPLAY_TEST;  max7219_handler->data[5] = TestMode;
	max7219_handler->data[6] = ADDR_DISPLAY_TEST;  max7219_handler->data[7] = TestMode;
	_max7219_push_data(*max7219_handler);
	HAL_Delay(1500);

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

void max7219_show_all(max7219_struct *max7219_handler, 	max7219_map max7219_map_handler)
{
//	uint8_t myTrans[8]; // 1 - for  razryad or Adres; 2- znachenie

	//for (int i=1; i<=8; i++)
	{
//	  myTrans[0] = i; // razryad
//	  myTrans[1] = _map[0][i]; // znachenie
//	  myTrans[2] = i; // razryad
//	  myTrans[3] = _map[1][i]; // znachenie
//	  myTrans[4] = i; // razryad
//	  myTrans[5] = _map[2][i]; // znachenie
//	  myTrans[6] = i; // razryad
//	  myTrans[7] = _map[3][i]; // znachenie

//		max7219_handler->data[0] = i;  max7219_handler->data[1] = max7219_map_handler.kub_0[i];
//		max7219_handler->data[2] = i;  max7219_handler->data[3] = max7219_map_handler.kub_1[i];
//		max7219_handler->data[4] = i;  max7219_handler->data[5] = max7219_map_handler.kub_2[i];
//		max7219_handler->data[6] = i;  max7219_handler->data[7] = max7219_map_handler.kub_3[i];

		max7219_handler->data[0] = 1;  max7219_handler->data[1] = 0xFF;
		max7219_handler->data[2] = 1;  max7219_handler->data[3] = 3;
		max7219_handler->data[4] = 1;  max7219_handler->data[5] = 7;
		max7219_handler->data[6] = 1;  max7219_handler->data[7] = 15;

		HAL_Delay(1);
	  _max7219_push_data(*max7219_handler);
	  HAL_Delay(1);
	}
}
