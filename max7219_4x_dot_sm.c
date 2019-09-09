
#include "main.h"
#include "max7219_4x_dot_sm.h"

#define BYTE_IN_SPI_PACKAGE		2
#define SPI_PACKAGE_TIMEOUT		2

	uint8_t myTrans[8]; // 1 - for  razryad or Adres; 2- znachenie

void _max7219_print_one_digit(max7219_struct max7219_handler, uint8_t position, uint32_t value);
void _max7219_write_strob(max7219_struct max7219_handler);
/***************************************************************************************/

void max7219_4x_dot_init(max7219_struct *max7219_handler)
{
	 // test - On
	    myTrans[0] = 0x0F;  myTrans[1] = 0x01;
	    myTrans[2] = 0x0F;  myTrans[3] = 0x01;
	    myTrans[4] = 0x0F;  myTrans[5] = 0x01;
	    myTrans[6] = 0x0F;  myTrans[7] = 0x01;

	    HAL_SPI_Transmit(max7219_handler->spi, myTrans, 8, 1);
	    _max7219_write_strob(*max7219_handler);

	    HAL_Delay(500);

	    // test - Off
	    myTrans[0] = 0x0F;  myTrans[1] = 0x00;
	    myTrans[2] = 0x0F;  myTrans[3] = 0x00;
	    myTrans[4] = 0x0F;  myTrans[5] = 0x00;
	    myTrans[6] = 0x0F;  myTrans[7] = 0x00;

	    HAL_SPI_Transmit(max7219_handler->spi, myTrans,8,1);
	    _max7219_write_strob(*max7219_handler);

	    // Decode Mode - No 1 in 1
	    // myTrans[1] hex   -> FF
	    // myTrans[1] pixel -> 00
	    myTrans[0] = 0x09;  myTrans[1] = 0x00;	// pixel
	    myTrans[2] = 0x09;  myTrans[3] = 0x00;	// pixel
	    myTrans[4] = 0x09;  myTrans[5] = 0x00;	// pixel
	    myTrans[6] = 0x09;  myTrans[7] = 0x00;	// pixel

	    HAL_SPI_Transmit(max7219_handler->spi, myTrans, 8, 1);;
	    _max7219_write_strob(*max7219_handler);

	    // Intensity 3/32
	    myTrans[0] = 0x0A;  myTrans[1] = 0x01;
	    myTrans[2] = 0x0A;  myTrans[3] = 0x01;
	    myTrans[4] = 0x0A;  myTrans[5] = 0x01;
	    myTrans[6] = 0x0A;  myTrans[7] = 0x01;

	    HAL_SPI_Transmit(max7219_handler->spi, myTrans, 8, 1);
	    _max7219_write_strob(*max7219_handler);

	    //Scan Limit - All
	    myTrans[0] = 0x0B;  myTrans[1] = 0x07;
	    myTrans[2] = 0x0B;  myTrans[3] = 0x07;
	    myTrans[4] = 0x0B;  myTrans[5] = 0x07;
	    myTrans[6] = 0x0B;  myTrans[7] = 0x07;

	    HAL_SPI_Transmit(max7219_handler->spi, myTrans, 8, 1);
	    _max7219_write_strob(*max7219_handler);

	    // Shutdown - none
	    // myTrans[1] -> 00 sleep
	    // myTrans[1] -> 01 work
	    myTrans[0] = 0x0C;  myTrans[1] = 0x01;
	    myTrans[2] = 0x0C;  myTrans[3] = 0x01;
	    myTrans[4] = 0x0C;  myTrans[5] = 0x01;
	    myTrans[6] = 0x0C;  myTrans[7] = 0x01;

	    HAL_SPI_Transmit(max7219_handler->spi, myTrans, 8, 1);
	    _max7219_write_strob(*max7219_handler);
}
/***************************************************************************************/

void max7219_print_value(max7219_struct *max7219_handler,uint32_t value, position_enum position)
{
	if (position == 0)
	{
		position = 4;
	}
	else
	{
		position = 0;
	}

	_max7219_print_one_digit(*max7219_handler, position + 4, (value/1000) % 10 );
	_max7219_print_one_digit(*max7219_handler, position + 3, (value/100 ) % 10 );
	_max7219_print_one_digit(*max7219_handler, position + 2, (value/10  ) % 10 );
	_max7219_print_one_digit(*max7219_handler, position + 1, (value     ) % 10 );
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
	HAL_Delay(1);
	HAL_GPIO_WritePin(max7219_handler.cs_port, max7219_handler.cs_pin, SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(max7219_handler.cs_port, max7219_handler.cs_pin, RESET);
	//HAL_Delay(1);
}
/***************************************************************************************/
