#ifndef MAX7219_DIGIT_H_INCLUDED
#define MAX7219_DIGIT_H_INCLUDED

typedef struct
{
	SPI_HandleTypeDef	*spi;
	GPIO_TypeDef 		*cs_port;
    uint16_t 			 cs_pin;
} max7219_struct;

typedef struct
{
//	// test - On
//	spi_buffer[0] = 0x0F;  spi_buffer[1] = 0x01;
//
//	// test - Off
//	spi_buffer[0] = 0x0F;  spi_buffer[1] = 0x00;
//
//	// Decode Mode - No 1 in 1
//	// spi_buffer[1] hex   -> FF
//	// spi_buffer[1] pixel -> 00
//	uint8_t DecodeMode = 0xFF;
//	spi_buffer[0] = 0x09;  spi_buffer[1] = DecodeMode;	// pixel
//
//
//	// Intensity 3/32 0x01,
//	spi_buffer[0] = 0x0A;  spi_buffer[1] = 0x03;
//
//
//	//Scan Limit - All
//	spi_buffer[0] = 0x0B;  spi_buffer[1] = 0x07;
//
//
//	// Shutdown - none
//	// spi_buffer[1] -> 00 sleep
//	// spi_buffer[1] -> 01 work
//	spi_buffer[0] = 0x0C;  spi_buffer[1] = 0x01;

}
max7219_init_struct;

typedef enum
{
	most_position  = 0,
	least_position = 1
} position_enum;


void max7219_init(max7219_struct *max7219_handler);
void max7219_print_value(max7219_struct *max7219_handler, uint32_t value, position_enum position);

#endif 	//	MAX7219_DIGIT_H_INCLUDED

