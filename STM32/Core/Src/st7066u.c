/*
 * st7066u.c
 *
 *  Created on: Oct 7, 2019
 *      Author: Walter Jacob, IV
 */

#include "st7066u.h"

void ST7066U_ClearDisplay(ST7066U* D)
{
	ST7066U_WriteCmd(D, 0x01, ST7066U_CMD);
}

void ST7066U_GoTo(ST7066U* D, uint32_t position)
{
	ST7066U_WriteCmd(D, position, ST7066U_CMD);
}

void ST7066U_WriteInteger(ST7066U* D, uint32_t value, uint8_t position, ST7066U_CLR_OPT clr)
{
	uint8_t buff[20];
	itoa(value, buff);
	ST7066U_WriteText(D, buff, position, clr);
}

void ST7066U_WriteText(ST7066U* D, uint8_t* buff, uint8_t position, ST7066U_CLR_OPT clr)
{
	// TODO:
	//	text position
	uint32_t buff_idx = 0;
	if (clr == ST7066U_CLR)
	{
		ST7066U_ClearDisplay(D);
	}
	ST7066U_GoTo(D, position);
	while (buff[buff_idx] != 0)
	{
		ST7066U_WriteCmd(D, buff[buff_idx++], ST7066U_WRITE);
	}
}

void ST7066U_WriteCmd(ST7066U *D, uint8_t c, ST7066U_RS_PIN rs)
{

	// MSNibble
	if (c & 0x80) HAL_GPIO_WritePin(D->d7_port, D->d7_pin, SET);
	else HAL_GPIO_WritePin(D->d7_port, D->d7_pin, RESET);
	if (c & 0x40) HAL_GPIO_WritePin(D->d6_port, D->d6_pin, SET);
	else HAL_GPIO_WritePin(D->d6_port, D->d6_pin, RESET);
	if (c & 0x20) HAL_GPIO_WritePin(D->d5_port, D->d5_pin, SET);
	else HAL_GPIO_WritePin(D->d5_port, D->d5_pin, RESET);
	if (c & 0x10) HAL_GPIO_WritePin(D->d4_port, D->d4_pin, SET);
	else HAL_GPIO_WritePin(D->d4_port, D->d4_pin, RESET);

	if (rs == ST7066U_CMD)
	{
		HAL_GPIO_WritePin(D->rs_port, D->rs_pin, RESET);	// c is a command
	}
	else HAL_GPIO_WritePin(D->rs_port, D->rs_pin, SET);		// c is write data
	ST7066U_ShiftIn(D);


	// LSNibble
	if (c & 0x08) HAL_GPIO_WritePin(D->d7_port, D->d7_pin, SET);
	else HAL_GPIO_WritePin(D->d7_port, D->d7_pin, RESET);
	if (c & 0x04) HAL_GPIO_WritePin(D->d6_port, D->d6_pin, SET);
	else HAL_GPIO_WritePin(D->d6_port, D->d6_pin, RESET);
	if (c & 0x02) HAL_GPIO_WritePin(D->d5_port, D->d5_pin, SET);
	else HAL_GPIO_WritePin(D->d5_port, D->d5_pin, RESET);
	if (c & 0x01) HAL_GPIO_WritePin(D->d4_port, D->d4_pin, SET);
	else HAL_GPIO_WritePin(D->d4_port, D->d4_pin, RESET);

	ST7066U_ShiftIn(D);
	if (c <= 0x03) HAL_Delay(2);
	else ST7066U_Delay_us(D, 40);
}

void ST7066U_ShiftIn(ST7066U *D)
{
	// Takes ~180ns @ 128MHz
	HAL_GPIO_WritePin(D->e_port, D->e_pin, SET);
	HAL_GPIO_WritePin(D->e_port, D->e_pin, RESET);
}

void ST7066U_Delay_us(ST7066U *D, uint32_t wait_cycles)
{
	// ~10us per 100 wait_cycles
	for (int i = 0; i < (wait_cycles * 10); i++)
	{
		asm("NOP");
	}
}

void ST7066U_Init(ST7066U *D)
{
	// Wake up (x3)
	HAL_Delay(50);										// Wait for power up
	HAL_GPIO_WritePin(D->rs_port, D->rs_pin, RESET);	// Write 0x30
	HAL_GPIO_WritePin(D->d7_port, D->d7_pin, RESET);	// can't use cmdST7066U
	HAL_GPIO_WritePin(D->d6_port, D->d6_pin, RESET);	// function since only
	HAL_GPIO_WritePin(D->d5_port, D->d5_pin, SET);		// a nibble at a time
	HAL_GPIO_WritePin(D->d4_port, D->d4_pin, SET);
	HAL_Delay(5);
	ST7066U_ShiftIn(D);
	HAL_Delay(1);
	ST7066U_ShiftIn(D);
	HAL_Delay(1);
	ST7066U_ShiftIn(D);
	ST7066U_Delay_us(D, 40);								// Now can check BusyFlag
	HAL_GPIO_WritePin(D->d4_port, D->d4_pin, RESET);	// Write 0x20
	ST7066U_ShiftIn(D);

	// Send initialization commands
	ST7066U_WriteCmd(D, 0x38, ST7066U_CMD);					// Set 4-bit * 2-line
	ST7066U_WriteCmd(D, 0x10, ST7066U_CMD);					// Set cursor
	ST7066U_WriteCmd(D, 0x0D, ST7066U_CMD);					// Display ON, blinking cursor
	ST7066U_WriteCmd(D, 0x06, ST7066U_CMD);					// Entry mode set
}

