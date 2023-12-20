/*********************************************************************************/
/* Author    : Zeiad Salah                                                       */
/* Version   : V01                                                               */
/* Date      : 20  DEC 2023                                                       */
/*********************************************************************************/

#include "STD_TYPES.h"
#include "USART_INTERFACE.h"
#include "USART_Config.h"
#include "GPIO_private.h"
#include "USART_PRIVATE.h"

#include "GPIO_private.h"
#include "RCC_CONFIG.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void USART_voidInit(u8 Copy_u8uart, u32 Copy_u32BaudRate)
{
	
switch(Copy_u8uart)
{
	case UART1:
	
	//GPIOA_CRH&=~0x00000FF0;
  //GPIOA_CRH|= 0x000008B0; 	// RX1=input with pull-up, TX1=alt. func output
	//GPIOA_ODR |= (1<<10);  //pull-up PA10
	
	USART1->CR1 = 0x200C; //enable uart,Tx,&Rx
	#if  RCC_CLOCK_TYPE  == RCC_HSI
	USART1->BRR = 8000000/Copy_u32BaudRate;	  // Fclk/bps 
	#elif RCC_CLOCK_TYPE  == RCC_PLL
	USART1->BRR = 72000000/Copy_u32BaudRate;	  // Fclk/bps 
	#endif
	
	break;
  default: break;
}


}
void USART_voidTransmit(u8 Copy_u8uart,void* Copy_u8data, u8 Copy_u8type){
  
	char array[16];



	switch(Copy_u8uart)
{
	case UART1:
	
	 if(Copy_u8type == STRING)
	{
	 for(int i=0;*(char*)(Copy_u8data+i)!='\0';i++)
		{

	USART1->DR = *(char*)(Copy_u8data+i);
	while((USART1->SR&(1<<6)) == 0 );
		}
  }
	break;
		default:break;
} 

}
char USART_charReceive(u8 Copy_u8uart)
{
	volatile u32 LOC_u32TimeOut=0;
switch(Copy_u8uart)
{
	case UART1:
	
		while((USART1->SR&(1<<5)) == 0 );
	return USART1->DR;
	break;
	default:
	return 0;	
	break;
}


}
