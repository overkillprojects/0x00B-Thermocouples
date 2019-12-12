/*
 * reflo_utils.c
 *
 *  Created on: Oct 7, 2019
 *      Author: Walter Jacob, IV
 */

#include "reflo_utils.h"

uint8_t* itoa(uint32_t value, uint8_t* result)
{
	uint8_t* ptr = result, *ptr1 = result, tmp_char;
	uint32_t tmp_value;

	do {
		tmp_value = value;
		value /= 10;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * 10)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
