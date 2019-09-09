
#include "main.h"
#include "max7219_4x_dot_sm.h"

#define BYTE_IN_SPI_PACKAGE		8
#define SPI_PACKAGE_TIMEOUT		1

	uint8_t myTrans[8]; // 1 - for  razryad or Adres; 2- znachenie

void _max7219_print_one_digit(max7219_struct max7219_handler, uint8_t position, uint32_t value);
void _max7219_write_strob(max7219_struct max7219_handler);
void _max7219_push_data(max7219_struct max7219_handler);

/***************************************************************************************/

void _max7219_push_data(max7219_struct max7219_handler)
{
    HAL_SPI_Transmit(max7219_handler.spi, max7219_handler.data, BYTE_IN_SPI_PACKAGE, SPI_PACKAGE_TIMEOUT);
    _max7219_write_strob(max7219_handler);
}

void max7219_init(max7219_struct *max7219_handler)
{
	// test - On
	max7219_handler->data[0] = 0x0F;  max7219_handler->data[1] = 0x01;
	max7219_handler->data[2] = 0x0F;  max7219_handler->data[3] = 0x01;
	max7219_handler->data[4] = 0x0F;  max7219_handler->data[5] = 0x01;
	max7219_handler->data[6] = 0x0F;  max7219_handler->data[7] = 0x01;
	_max7219_push_data(*max7219_handler);
	HAL_Delay(1500);

	// test - Off
	max7219_handler->data[0] = 0x0F;  max7219_handler->data[1] = 0x00;
	max7219_handler->data[2] = 0x0F;  max7219_handler->data[3] = 0x00;
	max7219_handler->data[4] = 0x0F;  max7219_handler->data[5] = 0x00;
	max7219_handler->data[6] = 0x0F;  max7219_handler->data[7] = 0x00;
	_max7219_push_data(*max7219_handler);

	// Decode Mode - No 1 in 1
	// myTrans[1] hex   -> FF
	// myTrans[1] pixel -> 00
	max7219_handler->data[0] = 0x09;  max7219_handler->data[1] = 0x00;
	max7219_handler->data[2] = 0x09;  max7219_handler->data[3] = 0x00;
	max7219_handler->data[4] = 0x09;  max7219_handler->data[5] = 0x00;
	max7219_handler->data[6] = 0x09;  max7219_handler->data[7] = 0x00;
	_max7219_push_data(*max7219_handler);

	// Intensity 3/32
	max7219_handler->data[0] = 0x0A;  max7219_handler->data[1] = 0x01;
	max7219_handler->data[2] = 0x0A;  max7219_handler->data[3] = 0x01;
	max7219_handler->data[4] = 0x0A;  max7219_handler->data[5] = 0x01;
	max7219_handler->data[6] = 0x0A;  max7219_handler->data[7] = 0x01;
	_max7219_push_data(*max7219_handler);

	//Scan Limit - All
	max7219_handler->data[0] = 0x0B;  max7219_handler->data[1] = 0x07;
	max7219_handler->data[2] = 0x0B;  max7219_handler->data[3] = 0x07;
	max7219_handler->data[4] = 0x0B;  max7219_handler->data[5] = 0x07;
	max7219_handler->data[6] = 0x0B;  max7219_handler->data[7] = 0x07;
	_max7219_push_data(*max7219_handler);

	// Shutdown - none
	// myTrans[1] -> 00 sleep
	// myTrans[1] -> 01 work
	max7219_handler->data[0] = 0x0C;  max7219_handler->data[1] = 0x01;
	max7219_handler->data[2] = 0x0C;  max7219_handler->data[3] = 0x01;
	max7219_handler->data[4] = 0x0C;  max7219_handler->data[5] = 0x01;
	max7219_handler->data[6] = 0x0C;  max7219_handler->data[7] = 0x01;
	_max7219_push_data(*max7219_handler);

}
/***************************************************************************************/


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

void _max7219_write_strob(max7219_struct max7219_handler)
{
	HAL_GPIO_WritePin(max7219_handler.cs_port, max7219_handler.cs_pin, SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(max7219_handler.cs_port, max7219_handler.cs_pin, RESET);
}
/***************************************************************************************/
