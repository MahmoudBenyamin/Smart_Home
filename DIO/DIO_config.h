/*
 * DIO_config.h
 *
 *  Created on: Feb 9, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef DIO_CONFIG_H_
#define DIO_CONFIG_H_

/* Group A Direction : DIO_u8INPUT ,  DIO_u8OUTPUT*/

#define PA0_DIR				DIO_u8OUTPUT
#define PA1_DIR				DIO_u8INPUT
#define PA2_DIR				DIO_u8OUTPUT
#define PA3_DIR				DIO_u8INPUT
#define PA4_DIR				DIO_u8OUTPUT
#define PA5_DIR				DIO_u8OUTPUT
#define PA6_DIR				DIO_u8OUTPUT
#define PA7_DIR				DIO_u8INPUT
//DDRA = 0b01110101
/* Group A Initial value : DIO_u8LOW/ DIO_u8FLOAT
 * 						   DIO_u8HIGH/ DIO_u8PULL_UP*/

#define PA0_INIT_VAL			DIO_u8HIGH
#define PA1_INIT_VAL			DIO_u8FLOAT
#define PA2_INIT_VAL			DIO_u8LOW
#define PA3_INIT_VAL			DIO_u8PULL_UP
#define PA4_INIT_VAL			DIO_u8HIGH
#define PA5_INIT_VAL			DIO_u8FLOAT
#define PA6_INIT_VAL			DIO_u8HIGH
#define PA7_INIT_VAL			DIO_u8PULL_UP


#endif /* DIO_CONFIG_H_ */
