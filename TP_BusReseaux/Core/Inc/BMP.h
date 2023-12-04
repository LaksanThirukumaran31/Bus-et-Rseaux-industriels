/*
 * BMP.h
 *TP1 : Bus et Réseaux
 *  Created on: Nov, 2023
 *      Author: Laksan Thirukumaran
 */

#ifndef INC_BMP_H_
#define INC_BMP_H_

#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>

#define BMPAddress  	0x77

typedef uint32_t BMP280_U32_t;
typedef int32_t BMP280_S32_t;
void devID_BMP(void);
void setConfig_BMP(void);
uint32_t temperatureNonCompense(void);
uint32_t pressionNonCompense(void);

BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T);
BMP280_U32_t bmp280_compensate_P_int32(BMP280_S32_t adc_P);
typedef uint32_t BMP280_U32_t;
typedef int32_t BMP280_S32_t;


#endif /* INC_BMP_H_ */
