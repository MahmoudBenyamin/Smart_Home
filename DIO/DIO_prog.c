/*
 * DIO_prog.c
 *
 *  Created on: Feb 9, 2022
 *      Author: Ahmed El-Gaafrawy
 */

//public libraries
#include "../stdTypes.h"
//#include "BIT_MATH"
#include "../errorState.h"

//lower layer libraries

//own libraries
#include "DIO_config.h"
#include "DIO_priv.h"


ES_t DIO_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	DDRA = CONC(PA7_DIR,PA6_DIR,PA5_DIR,PA4_DIR,PA3_DIR,PA2_DIR,PA1_DIR,PA0_DIR);

	PORTA = CONC(PA7_INIT_VAL,PA6_INIT_VAL,PA5_INIT_VAL,PA4_INIT_VAL,PA3_INIT_VAL,PA2_INIT_VAL,PA1_INIT_VAL,PA0_INIT_VAL);
/*
#if PA0_DIR == DIO_u8INPUT

#elif PA0_DIR == DIO_u8OUTPUT

#endif
*/
	return Local_enuErrorState;
}

ES_t DIO_enuSetPinDirection(u8 Copy_u8GroupID , u8 Copy_u8PinID, u8 Copy_u8Direction)
{
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_u8GroupID <= DIO_u8GROUP_D &&
			Copy_u8PinID <= DIO_u8PIN7 &&
			Copy_u8Direction <= DIO_u8OUTPUT)
	{
		switch (Copy_u8GroupID)
		{
		case DIO_u8GROUP_A:
			DDRA &= ~(DIO_BIT_MASK<<Copy_u8PinID);
			DDRA |=  (Copy_u8Direction<<Copy_u8PinID);
			break;
		case DIO_u8GROUP_B:
			DDRB &= ~(DIO_BIT_MASK<<Copy_u8PinID);
			DDRB |=  (Copy_u8Direction<<Copy_u8PinID);
			break;
		case DIO_u8GROUP_C:
			DDRC &= ~(DIO_BIT_MASK<<Copy_u8PinID);
			DDRC |=  (Copy_u8Direction<<Copy_u8PinID);
			break;
		case DIO_u8GROUP_D:
			DDRD &= ~(DIO_BIT_MASK<<Copy_u8PinID);
			DDRD |=  (Copy_u8Direction<<Copy_u8PinID);
			break;
		}
		Local_enuErrorState = ES_OK;
	}
	else
	{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorState;
}

ES_t DIO_enuSetPinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID, u8 Copy_u8Value)
{
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_u8GroupID <= DIO_u8GROUP_D &&
			Copy_u8PinID <= DIO_u8PIN7 &&
			Copy_u8Value <= DIO_u8OUTPUT)
	{
		switch (Copy_u8GroupID)
		{
		case DIO_u8GROUP_A:
			PORTA &= ~(DIO_BIT_MASK<<Copy_u8PinID);
			PORTA |=  (Copy_u8Value<<Copy_u8PinID);
			break;
		case DIO_u8GROUP_B:
			PORTB &= ~(DIO_BIT_MASK<<Copy_u8PinID);
			PORTB |=  (Copy_u8Value<<Copy_u8PinID);
			break;
		case DIO_u8GROUP_C:
			PORTC &= ~(DIO_BIT_MASK<<Copy_u8PinID);
			PORTC |=  (Copy_u8Value<<Copy_u8PinID);
			break;
		case DIO_u8GROUP_D:
			PORTD &= ~(DIO_BIT_MASK<<Copy_u8PinID);
			PORTD |=  (Copy_u8Value<<Copy_u8PinID);
			break;
		}
		Local_enuErrorState = ES_OK;
	}
	else
	{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorState;
}

ES_t DIO_enuTogglePinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID)
{
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_u8GroupID <= DIO_u8GROUP_D &&
				Copy_u8PinID <= DIO_u8PIN7)
		{
			switch (Copy_u8GroupID)
			{
			case DIO_u8GROUP_A:
				PORTA ^=  (DIO_BIT_MASK<<Copy_u8PinID);
				break;
			case DIO_u8GROUP_B:
				PORTB ^=  (DIO_BIT_MASK<<Copy_u8PinID);
				break;
			case DIO_u8GROUP_C:
				PORTC ^=  (DIO_BIT_MASK<<Copy_u8PinID);
				break;
			case DIO_u8GROUP_D:
				PORTD ^=  (DIO_BIT_MASK<<Copy_u8PinID);
				break;
			}
			Local_enuErrorState = ES_OK;
		}
		else
		{
			Local_enuErrorState = ES_OUT_OF_RANGE;
		}

	return Local_enuErrorState;
}

ES_t DIO_enuGetPinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID, u8 *Copy_pu8Value)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_pu8Value != NULL)
	{
		if(Copy_u8GroupID <= DIO_u8GROUP_D &&
				Copy_u8PinID <= DIO_u8PIN7)
		{
			switch (Copy_u8GroupID)
			{
			case DIO_u8GROUP_A:
				*Copy_pu8Value = ( (PINA>>Copy_u8PinID) & DIO_BIT_MASK);
				break;
			case DIO_u8GROUP_B:
				*Copy_pu8Value = ( (PINB>>Copy_u8PinID) & DIO_BIT_MASK);
				break;
			case DIO_u8GROUP_C:
				*Copy_pu8Value = ( (PINC>>Copy_u8PinID) & DIO_BIT_MASK);
				break;
			case DIO_u8GROUP_D:
				*Copy_pu8Value = ( (PIND>>Copy_u8PinID) & DIO_BIT_MASK);
				break;
			}

			Local_enuErrorState = ES_OK;
		}
		else
		{
			Local_enuErrorState = ES_OUT_OF_RANGE;
		}
	}
	else
	{
		Local_enuErrorState = ES_NULL_POINTER;
	}

	return Local_enuErrorState;
}
