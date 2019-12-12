/*
 * st7066u.h
 *
 *  Created on: Oct 7, 2019
 *      Author: Walter Jacob, IV
 */

#ifndef ST7066U_H_
#define ST7066U_H_

/* external includes */
#include "main.h"

/* struct defs */
typedef struct
{
	uint32_t rs_pin;
	GPIO_TypeDef* rs_port;
	uint32_t rw_pin;
	GPIO_TypeDef* rw_port;
	uint32_t e_pin;
	GPIO_TypeDef* e_port;
	uint32_t d4_pin;
	GPIO_TypeDef* d4_port;
	uint32_t d5_pin;
	GPIO_TypeDef* d5_port;
	uint32_t d6_pin;
	GPIO_TypeDef* d6_port;
	uint32_t d7_pin;
	GPIO_TypeDef* d7_port;
} ST7066U;

/* enum defs */
typedef enum
{
	ST7066U_CMD,
	ST7066U_WRITE
} ST7066U_RS_PIN;

typedef enum
{
	ST7066U_NO_CLR,
	ST7066U_CLR
} ST7066U_CLR_OPT;

/* function prototypes */
void ST7066U_WriteCmd(ST7066U *D, uint8_t c, ST7066U_RS_PIN rs);
void ST7066U_ShiftIn(ST7066U *D);
void ST7066U_Delay_us(ST7066U *D, uint32_t wait_cycles);
void ST7066U_Init(ST7066U *D);

void ST7066U_WriteText(ST7066U* D, uint8_t* buff, uint8_t position, ST7066U_CLR_OPT clr);
void ST7066U_WriteInteger(ST7066U* D, uint32_t value, uint8_t position, ST7066U_CLR_OPT clr);
void ST7066U_GoTo(ST7066U* D, uint32_t position);
void ST7066U_ClearDisplay(ST7066U* D);

#endif /* ST7066U_H_ */
