/*
 * debug.c
 *
 *  Created on: Nov 10, 2022
 *      Author: Igor
 */

#include "debug.h"
#include "usart.h"

int _write(int file, char *ptr, int len)
{
	usart_DebugSend((const uint8_t *)ptr, (uint16_t)len);

	return len;
}
