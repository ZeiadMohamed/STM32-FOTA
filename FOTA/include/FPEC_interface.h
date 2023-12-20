
/*********************************************************************************/
/* Author    : Zeiad Salah                                                       */
/* Version   : V01                                                               */
/* Date      : 20  DEC 2023                                                       */
/*********************************************************************************/


#ifndef FPEC_INTERFACE_H
#define FPEC_INTERFACE_H

#include "STD_TYPES.h"

void FPEC_voidEraseAppArea(void);
	
void FPEC_voidFlashPageErase(uint8 Copy_u8PageNumber);

void FPEC_voidFlashWrite(uint32 Copy_u32Address, uint16* Copy_u16Data, uint8 Copy_u8Length);




#endif // FPEC_INTERFACE_H
