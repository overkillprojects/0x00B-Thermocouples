/*
 * max31855.c
 *
 *  Created on: Sep 27, 2019
 *      Author: Walter Jacob, IV
 */

#include "max31855.h"

SPIReadErrorCode MAX31855_Read(MAX31855 *T, SPI_HandleTypeDef *hspi)
{
	uint16_t buffer[2];

	HAL_GPIO_WritePin(T->port, T->pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(hspi, (uint8_t*)(buffer), 2, 100);
	HAL_GPIO_WritePin(T->port, T->pin, GPIO_PIN_SET);

	T->temp_thermo = ((buffer[0] >> 2) & 0xFFFF);
	T->temp_int = ((buffer[1] >> 4) & 0xFFFF);
	T->fault = ((buffer[0] >> 0) & 0x01);
	T->scv = ((buffer[1] >> 2) & 0x01);
	T->scg = ((buffer[1] >> 1) & 0x01);
	T->oc = ((buffer[1] >> 0) & 0x01);

	return SPI_READ_SUCCESS;
}
