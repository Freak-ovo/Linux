#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "MCIMX6Y2.h" 


void UART_Init();
void UART_IO_Init();
void UART_Open(UART_Type *base);
void UART_Close(UART_Type *base);
void UART_SoftReset(UART_Type *base);

void UART_SetBaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);

uint8_t UART_Read(UART_Type *base);
void UART_Write(UART_Type *base, uint8_t data);
void UART_WriteString(UART_Type *base, const unsigned char *str);

#endif