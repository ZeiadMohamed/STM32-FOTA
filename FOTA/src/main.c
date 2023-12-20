/*********************************************************************************/
/* Author    : Zeiad Salah                                                       */
/* Version   : V01                                                               */
/* Date      : 20  DEC 2023                                                       */
/*********************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "STK_interface.h"
#include "USART_INTERFACE.h"

#include "ESP8266_interface.h"
#include "FPEC_interface.h"

#include "main.h"

void Parser_voidParseRecord(uint8* Copy_u8BufData);
void clear_data(uint8* data_cleared);
void GetNewRecordLine(void);
void copy_pureData(void);

volatile uint8  u8RecBuffer[60] = {0}  ;
volatile uint8  u8RecCounter    = 0;
volatile uint8  u8TimeOutFlag   = 0;
volatile uint8  u8BLWriteReq    = 1;
uint8 data[60] ={0};
typedef void (*Function_t)(void);
Function_t addr_to_call = 0;

extern uint8 error;

void func(void)
{
#define SCB_VTOR   *((volatile u32*)0xE000ED08) 

	SCB_VTOR = 0x08003000;

	addr_to_call = *(Function_t*)(0x08003004);
	addr_to_call();
}
//fotaaaaaaaaa
int main()
{
	RCC_voidInitSysClock(); /* Enable HSI */
	RCC_voidEnableClock(RCC_APB2,APB2_GPIOA_EN);  /*ENABLE PORTA AS IO PINS*/
	RCC_voidEnableClock(RCC_APB2,APB2_USART1_EN); /*ENABLE USART1 */
	RCC_voidEnableClock(RCC_AHB ,AHB_FLITF_EN); /*ENABLE FPEC */
	
	
	RCC_voidEnableClock(RCC_APB2,APB2_GPIOB_EN);  /*ENABLE PORTB AS IO PINS*/
	STK_voidInit();
	GPIO_voidSetPinDirection(GPIOA,9,0b1010);   /* TX AFPP */
	GPIO_voidSetPinDirection(GPIOA,10,0b0100);  /* RC Input Floating */
	USART_voidInit(UART1,115200);
	ESP8266_voidResetESP();
	//ESP8266_voidConnectToWiFi(SSID,PASSWORD);
	ESP8266_voidConnectToserver(IPserver);
  ESP8266_voidSendData(IPserver,"46","http://192.168.1.23:1142/start_flash.php",data);
	GetNewRecordLine();
	STK_voidSetIntervalSingle((5*1000*1000),func); /* 35 sec */
	uint8 Local_u8RecStatus;
	while(u8TimeOutFlag == 0)
	{
		clear_data(u8RecBuffer);
		
		if (data[2] == ':')
		{
			STK_voidStopInterval();
			
			copy_pureData();
		
			if (u8BLWriteReq == 1)
			{
				FPEC_voidEraseAppArea();
				u8BLWriteReq = 0;
			}
			
			/* Parse */
			Parser_voidParseRecord(u8RecBuffer);
			GetNewRecordLine();
			STK_voidSetIntervalSingle((5*1000*1000),func); /* 5 sec */
		}
		else
		{
			/* No file to burn */
		}
			

			
	}
	
}


void clear_data(uint8* data_cleared)
{
	u8 LOC_u8Iterator1 = 0 ;

	for( LOC_u8Iterator1 = 0 ; LOC_u8Iterator1 < 60 ; LOC_u8Iterator1++ ){

		data_cleared[ LOC_u8Iterator1 ] = 0 ;

	}
}

void GetNewRecordLine(void)
{
	if(error == 0)
	{
		clear_data(data);
		ESP8266_voidConnectToserver(IPserver);
		ESP8266_voidSendData(IPserver,"56","http://192.168.1.23:1142/start_flash.php?config=ok",data);
		
		while(strcmp(data,"") == 0)
		{
			ESP8266_voidResetESP();
			clear_data(data);
			ESP8266_voidConnectToserver(IPserver);
			ESP8266_voidSendData(IPserver,"58","http://192.168.1.23:1142/start_flash.php?config=none",data);
		}
	}
}

void copy_pureData(void)
{
	while(u8RecBuffer[u8RecCounter] != '\n')
	{
		if (data[u8RecCounter+2] == '\r')
		{
			data[u8RecCounter+2] = '\n';
		}
		u8RecBuffer[u8RecCounter] = data[u8RecCounter+2];
		u8RecCounter ++ ;
	}
	u8RecCounter = 0;
}
