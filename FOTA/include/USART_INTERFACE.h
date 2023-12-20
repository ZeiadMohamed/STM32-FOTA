/*********************************************************************************/
/* Author    : Zeiad Salah                                                       */
/* Version   : V01                                                               */
/* Date      : 20  DEC 2023                                                       */
/*********************************************************************************/
#include "STD_TYPES.h"

#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

#define UART1 1
#define UART2 2
#define UART3 3

#define CHAR 1
#define INT 2
#define FLOAT 3
#define STRING 4

void USART_voidInit(u8 Copy_u8uart, u32 Copy_u32BaudRate);
void USART_voidTransmit(u8 Copy_u8uart,void* Copy_u8data, u8 Copy_u8type); 
char USART_charReceive(u8 Copy_u8uart);




#endif // UART_INTERFACE_H
