/*
 * max31855.h
 *
 *  Created on: Sep 27, 2019
 *      Author: Walter Jacob, IV
 */

#ifndef MAX31855_H_
#define MAX31855_H_

#include "main.h"

/*
 * TypeDefs
 */

typedef struct
{
	uint16_t pin;
	GPIO_TypeDef * port;
	int16_t temp_thermo;
	int16_t temp_int;
	uint8_t fault;
	uint8_t scv;
	uint8_t scg;
	uint8_t oc;
} MAX31855;

typedef enum
{
	SPI_READ_SUCCESS,
	SPI_READ_ERROR
} SPIReadErrorCode;

/*
 * Function Prototypes
 */

SPIReadErrorCode MAX31855_Read(MAX31855 *T, SPI_HandleTypeDef *hspi);

#endif /* MAX31855_H_ */
