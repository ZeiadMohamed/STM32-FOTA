/*********************************************************************************/
/* Author    : Zeiad Salah                                                       */
/* Version   : V01                                                               */
/* Date      : 20  DEC 2023                                                       */
/*********************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <string.h>
#include "GPIO_interface.h"
#include "STK_interface.h"
#include "USART_interface.h"

#include "ESP8266_interface.h"
#include "ESP8266_config.h"
#include "ESP8266_private.h"



/**
 * AT Commands:
 1* AT		>> TEST
 2* ATE0		>> STOP ECHO
 3* AT+CWMODE=1 		>>SET STATION MODE
 4* AT+CWJAP="SSID","PASSWORD" 		>> CONNECT TO WIFI
 5* AT+CIPSTART="TCP","IP",80 				>> *replace ip with the website ip* to start connect on server
 6* AT+CIPSEND=numOfBits+1		>>*replace numOfBits with number of http link bits* sto send data
 7* GET http://iot-arm.freevar.com/status.txt	>> put link of website
 * >>> rebeat again from line num 7 OF "AT+CIPSTART" because connection will closed after recive data
 * **note you recive data as form:
 * 		+IPD,LengthOfData:content....CLOSED
 */

uint8 volatile Iterator;
uint8 volatile  DataCome[200];
uint8 error = 0;

void ESP8266_voidInit(void)
{
	if(error == 0)
	{
		/* Sending AT Command To Check ESP8266 Is Working Or Not  */
		ESP8266_voidSendCmd("AT\r\n");
		ESP8266_voidClearBuffer();
	}
	if(error == 0)
	{
		/* Stop ECHO */
		ESP8266_voidSendCmd("ATE0\r\n");
	}
	if(error == 0)
	{
		/* Set station mode */
		ESP8266_voidSendCmd("AT+CWMODE=1\r\n");
	}

}


void ESP8266_voidConnectToWiFi(uint8* SSID , uint8 * Password )
{
	if (error == 0)
	{
		/* Clear ESP8266 Buffer */
		ESP8266_voidClearBuffer();
		
		uint8 local_u8String[200];
		strcat(local_u8String,"AT+CWJAP=\"");
		strcat(local_u8String,SSID);
		strcat(local_u8String,"\",\"");
		strcat(local_u8String,Password);
		strcat(local_u8String,"\"\r\n");
		ESP8266_voidSendCmd(local_u8String);
		
	}
}



/* input of fun is string with " "  */
void ESP8266_voidConnectToserver(uint8* IP)
{
	if(error == 0)
	{
		uint8 local_u8IP[100] = {0};
		strcat(local_u8IP,"AT+CIPSTART=\"TCP\",\"");
		strcat(local_u8IP,IP);
		strcat(local_u8IP,"\",1142\r\n");
		ESP8266_voidSendCmd(local_u8IP);
	}
	
}

/* input of fun is string with " " 

	 add 5 for datalegth because "GET " + 1

*/
void ESP8266_voidSendDataLength(uint8* local_u8DataLength)
{
	if (error == 0)
	{
		uint8 local_u8Length[30] = {0};
		/* Clear ESP8266 Buffer */
		ESP8266_voidClearBuffer();
		 /* add legth of "GET " + 1 */
		strcat(local_u8Length,"AT+CIPSEND=");
		strcat(local_u8Length,local_u8DataLength);/* add legth of "GET " + 1 */
		strcat(local_u8Length,"\r\n");
		
		

		ESP8266_voidSendCmd(local_u8Length);

		
	}
}



/* input of fun is string with " "  */
void ESP8266_voidSendDataURL(uint8* local_u8DataURL)
{
	if (error == 0)
	{
		uint8 local_u8url[200] = {0};
		
	/* Clear ESP8266 Buffer */
		ESP8266_voidClearBuffer();
		strcat(local_u8url,"GET ");
		strcat(local_u8url,local_u8DataURL);
		strcat(local_u8url,"\r\n");
		
		ESP8266_voidClearBuffer();
		ESP8266_voidSendCmd(local_u8url);
		
	}
	
	
}

void ESP8266_voidRecieveData( uint8* RecievedData)
{
	if (error == 0)
	{
	 uint8 Dummy = 0, flag = 0, local_timeout = 0, length = 0;
	 uint8 local_u8flagOfStartData = 0, local_u8flagOfEndData = 0 ,local_u8flagOfLength = 0;
		
	/* Clear ESP8266 Buffer */
		ESP8266_voidClearBuffer();
		do
		{			
			Dummy = USART_charReceive(UART);
			DataCome[Iterator] = Dummy;
			
			if( Dummy == '+' && flag == 0)
			{
				local_u8flagOfLength = Iterator; /* +IPD, */
				flag = 1;
			}
			if( Dummy == ':' && flag == 1)
			{
				local_u8flagOfStartData = Iterator;
				flag = 2;
				length = 0;
				for(uint8 i=local_u8flagOfLength+5 ; i < (local_u8flagOfStartData) ; i++)
				{
					length *=10;
					length += (DataCome[i] - 0x30); /* '0' =0x30 */
				}
				local_u8flagOfEndData =local_u8flagOfStartData + length;
			}
			if( (Iterator - 7) > local_u8flagOfEndData && flag == 2)
			{
				if(DataCome[local_u8flagOfEndData+6] == 'D' )
				{
					break;
				}
			}
			if(Dummy !=255)
				Iterator++;
			else
				local_timeout++;
			
		}while(local_timeout < 1000);
		
		for(uint8 i= local_u8flagOfStartData+1 ; i <= local_u8flagOfEndData ; i++)
		{
			RecievedData[i-local_u8flagOfStartData-1] = DataCome[i];
		}
	}
	
}

/* input of fun is string with " " 

	 add 5 for datalegth because "GET " + 1

*/
void ESP8266_voidSendData(uint8* IP , uint8* local_u8DataLength, uint8* local_u8DataURL , uint8* RecievedData)
{
	if(error == 0)
	{
		/* send data length ex: AT+CIPSEND=42 */
		ESP8266_voidSendDataLength(local_u8DataLength);
		/* send data ex: GET http://iotarm.freevar.com/status.txt */
		ESP8266_voidSendDataURL(local_u8DataURL);
		/*recieve data ex: +IPD,1:4CLOSED */
		ESP8266_voidRecieveData(RecievedData);
	}
	
}


uint8 ESP8266_u8ValidateCmd(void)
{

	ESP8266_voidClearBuffer();
	
	uint8 Dummy = 0;
	uint32 local_timeout =0;
	while(Dummy != 'K' && local_timeout < 100) /* OK || FAIL */
		{			
			Dummy = USART_charReceive(UART);
			DataCome[Iterator] = Dummy;
			
			if(Dummy !=255)
			{
				Iterator++;
			}
			else 
				local_timeout++;
			if(Iterator >140)
				ESP8266_voidClearBuffer();
		}

	/* check if receive ok */
	if(local_timeout == 100) /* .... ok\n */
	{
		error = 1;
		return 0;
	}
	else
	{
		error = 0;
		return 1;
	}

}

void ESP8266_voidClearBuffer ( void )
{

	u8 LOC_u8Iterator1 = 0 ;
	Iterator     = 0 ;

	for( LOC_u8Iterator1 = 0 ; LOC_u8Iterator1 < 150 ; LOC_u8Iterator1++ ){

		DataCome[ LOC_u8Iterator1 ] = 0 ;

	}

}

void ESP8266_voidSendCmd(uint8* cmd)
{
	u8 Local_u8Result = 0,  local_timeout = 0;
	while(Local_u8Result == 0 )//&& local_timeout<5
	{
		USART_voidTransmit(UART,cmd,STRING);
		Local_u8Result = ESP8266_u8ValidateCmd();
		local_timeout++;
	}
	
}
void ESP8266_voidResetESP(void)
{
	USART_voidTransmit(UART,"AT+RST\r\n",STRING);
	for (int i=0; i<=5000000; i++); /*DELAY*/
	error = 0;
	ESP8266_voidInit();
}
