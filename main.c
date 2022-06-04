/*
 * main.c
 *
 *  Created on: May 19, 2022
 *      Author: mahmo
 */
#include "stdTypes.h"
#include "errorState.h"
#include "interrupt.h"
#include <string.h>
#include <stdlib.h>
#include "DIO/DIO_int.h"
#include "LCD/LCD_int.h"
#include "Keypad/Keypad_int.h"
#include "ADC/ADC_int.h"
#include "GIE/GIE_int.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/semphr.h"
/* declaring Tasks and app functions */
void LCD (void *pv);
void KeyPad (void *pv);
void Door_Action (void *pv);
void ADC_Read (void *pv);
void adc_isr(void);
void ReadTemp (void *pv);
void Fan (void *pv);
/*global variables and handlers */
u16 pass = 0;
char doorStatus [4] = "off";
char warning [16] = "";
char FanState [4] = "off";
u16 current_value = 2;
u16 Digital [2] = {0};
u16 Temp = 0;
u16 Gas = 0;
/*xTaskHandle keypad_Handler;
xTaskHandle Door_Handler;
xTaskHandle LCD_Handler;*/
/*xTaskHandle ADC_Handler;
xTaskHandle TEMP_Handler;*/
xSemaphoreHandle LCD_semphr;
xSemaphoreHandle ADC_semphr;
int main ()
{
	// initialize  all modules
	LCD_enuInit();
	Keypad_enuInit();
	DIO_enuSetPinDirection(DIO_u8GROUP_B, DIO_u8PIN4, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(DIO_u8GROUP_B, DIO_u8PIN5, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(DIO_u8GROUP_B, DIO_u8PIN7, DIO_u8OUTPUT);
	/********************** init ADC ***************************************
	 * 1- set the pin
	 * 2- init the adc
	 * 3- enable global interrupt
	 * 4- enable ADC interrupt
	 * 5- set callback
	 * 6- enable ADC
	 * 7- start conversion
	 */
	ADC_enuSelectChannel(7);
	DIO_enuSetPinDirection(DIO_u8GROUP_A , DIO_u8PIN7 , DIO_u8INPUT);
	DIO_enuSetPinValue(DIO_u8GROUP_A , DIO_u8PIN7 , DIO_u8FLOAT);
	DIO_enuSetPinDirection(DIO_u8GROUP_A , DIO_u8PIN0 , DIO_u8INPUT);
	DIO_enuSetPinValue(DIO_u8GROUP_A , DIO_u8PIN0 , DIO_u8FLOAT);
	ADC_enuInit();
	ADC_enuEnableADC();
	ADC_enuEnableADC_INT();
	GIE_enuEnable();
	ADC_enuCallBack(adc_isr);
	ADC_enuStartConversion();
	/******************** create the tasks ******************
	 * 1 - Task pointer
	 * 2- Task name
	 * 3- stack depth
	 * 4- parameters
	 * 5- priority
	 * Task Handler
	 */
	ADC_semphr = xSemaphoreCreateCounting(1,0);
	xTaskCreate(LCD , NULL , 200 , NULL,3,NULL);
	xTaskCreate(KeyPad , NULL , 200 , NULL,2,NULL);
	xTaskCreate(Door_Action , NULL , 80 , NULL,2,NULL);
	xTaskCreate(ReadTemp , NULL , 80 , NULL,2,NULL);
	xTaskCreate(ADC_Read , NULL , 80 , NULL,3,NULL);
	//xTaskCreate(Fan , NULL , 80 , NULL,2,NULL);
	// initiate semaphore
	vSemaphoreCreateBinary(LCD_semphr);
	//vTaskSuspend(LCD_Handler);
	//start the scheduler
	vTaskStartScheduler();
	while(1);
}
void LCD (void *pv)
{
		LCD_enuWriteCommand(0x01);
		LCD_enuGoToPosition(1, 1);
		LCD_enuWriteString("pass:");
		LCD_enuGoToPosition(2, 1);
		LCD_enuWriteString("Door:");
		LCD_enuGoToPosition(2, 9);
		LCD_enuWriteString("|fan:");
		LCD_enuGoToPosition(3, 1);
		LCD_enuWriteString("Temp:");
		LCD_enuGoToPosition(3, 9);
		LCD_enuWriteString("|Gas:");
	while (1)
	{
		if(xSemaphoreTake(LCD_semphr , 35) == pdPASS )
		{
			LCD_enuGoToPosition(2, 6);
			LCD_enuWriteString(doorStatus);
			if(strlen(doorStatus) < 3)
				LCD_enuWriteData(' ');
			LCD_enuGoToPosition(2, 14);
			LCD_enuWriteString(FanState);
			if(strlen(FanState) < 3)
				LCD_enuWriteData(' ');
			LCD_enuGoToPosition(3, 6);
			LCD_enuWriteIntegerNum(Temp);
			if(Temp < 10)
				LCD_enuWriteString(" ");
			LCD_enuGoToPosition(3, 14);
			LCD_enuWriteIntegerNum(Gas);
			if(Gas < 100)
				LCD_enuWriteString("  ");
			LCD_enuGoToPosition(4, 1);
			for(u8 i = 1 ; i<=16;i++)
			{
				LCD_enuWriteData(' ');
			}
			LCD_enuGoToPosition(4, 1);
			LCD_enuWriteString(warning);
			strcpy(warning,"");
			xSemaphoreGive(LCD_semphr);
		}
		vTaskDelay(40);
	}
}
void KeyPad (void *pv)
{
	u8  value [5] ;
	while (1)
	{
		if(xSemaphoreTake(LCD_semphr , 35) == pdPASS)
		{
			LCD_enuGoToPosition(1, 6);
			//(pass != 0) ? LCD_enuWriteString("Enter Pass:") :LCD_enuWriteString("Select Pass:");
			for (u8 i = 0 ; i <4;i++)
				{
				Keypad_GetPressedKey(&value[i]);
				if(value[i] == 0xff)
					i--;
				else
					{
					(pass != 0) ? LCD_enuWriteData('*') :LCD_enuWriteData(value[i]) ;
					}
				}
		value[4] = '\0';
		if(pass == 0)
			pass = atoi(value);
		else
			{
				current_value = atoi(value);
				current_value = (current_value == pass);
			}
	//	LCD_enuGoToPosition(2, 1);
		if (current_value == 1)
		{
			//LCD_enuWriteString("pass is correct");
			xSemaphoreGive(LCD_semphr);
			//vTaskResume(LCD_Handler);
			//vTaskDelete(keypad_Handler);
		}
		else if (current_value == 0)
			{
				//LCD_enuWriteString("pass incorrect");
			}
		else if (current_value == 2)
		{
			//LCD_enuWriteString("pass saved");
		}
		xSemaphoreGive(LCD_semphr);
		}
		vTaskDelay(41);
	}
}
void Door_Action (void *pv)
{
	u8 error = 0;
	while(1)
	{
			if(current_value == 2)
			{
				strcpy(doorStatus,"off");
			}
			else if( (current_value == 1) )
			{
				error = 0;
				strcpy(doorStatus,"on");
				DIO_enuSetPinValue(DIO_u8GROUP_B, DIO_u8PIN4, DIO_u8HIGH);
				vTaskDelay(10);
				DIO_enuSetPinValue(DIO_u8GROUP_B, DIO_u8PIN4, DIO_u8LOW);
				//vTaskDelete(Door_Handler);
			}
			else
			{
				strcpy(doorStatus,"off");
				error ++;
				if(error > 2)
				{
					DIO_enuSetPinValue(DIO_u8GROUP_B, DIO_u8PIN5, DIO_u8HIGH);
					vTaskDelay(10);
					DIO_enuSetPinValue(DIO_u8GROUP_B, DIO_u8PIN5, DIO_u8LOW);
					error = 0;
					strcpy(warning,"pass wrong*3");
				}
			}
		vTaskDelay(30);
	}
}
void ADC_Read (void *pv)
{
	u8 current = 0;
	while(1)
	{
		if (xSemaphoreTake(ADC_semphr,5) == pdPASS)
		{
			if(current == 0)
			{
				ADC_enuRead(&Digital[0]);
				ADC_enuSelectChannel(0);
				current = 1;
			}
			else
			{
				ADC_enuRead(&Digital[1]);
				ADC_enuSelectChannel(7);
				current = 0;
			}

			ADC_enuStartConversion();
		}
		vTaskDelay(10);
	}
}
void adc_isr(void)
{
		xSemaphoreGive(ADC_semphr);
}
void ReadTemp (void *pv)
{
	while(1)
	{
		Temp = Digital[0] / 2.f;
		Gas = (Digital[1]*5000ul )/ 1024.f;
		vTaskDelay(15);
	}
}
void Fan (void *pv)
{
	if(Temp > 24)
	{
		strcpy(FanState,"on");
		DIO_enuSetPinValue(DIO_u8GROUP_B, DIO_u8PIN7, DIO_u8HIGH);
	}
	else
	{
		strcpy(FanState,"off");
		DIO_enuSetPinValue(DIO_u8GROUP_B, DIO_u8PIN7, DIO_u8LOW);
	}
	vTaskDelay(23);
}
