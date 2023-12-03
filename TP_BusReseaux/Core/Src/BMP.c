/*
 * BMP.c
 * TP1 : Bus et Réseaux
 * -> Bus I2C
 * -> Capteur BMP280
 *  Created on: Nov, 2023
 *      Author: Laksan Thirukumaran & Clément DU 
 */

#include "BMP.h"


/*
 * Variable pour la partie config et devID
 */

const uint8_t setF4 = 0x57;
const uint8_t CtrlmesAddress = 0xF4;
uint8_t dev_ID = 0xD0;
uint8_t dataBuffer[8];
uint8_t Sendback[8];
uint8_t TabConfig[2]={CtrlmesAddress,setF4};



/*
 * Variable pour la partie température
 */
uint8_t TMsbAdress = 0xFA;
uint8_t TLsbAdress = 0xFB;
uint8_t TxLsbAdress = 0xFC;
uint8_t T_Msb[8];
uint8_t T_Lsb[8];
uint8_t T_xLsb[4];
uint32_t TempValue = 0;

/*
 * Variable pour la partie Pression
 */
uint8_t PMsbAdress = 0xF7;
uint8_t PLsbAdress = 0xF8;
uint8_t PxLsbAdress = 0xF9;
uint8_t P_Msb[8];
uint8_t P_Lsb[8];
uint8_t P_xLsb[4];
uint32_t PressValue = 0;


/*
 * Calibration
 */
uint16_t dig_T1 = 0x10;
int16_t  dig_T2 = 0xB6;
int16_t  dig_T3 = 0x32;
uint16_t dig_P1 = 0xB3;
int16_t  dig_P2 = 0x99;
int16_t  dig_P3 = 0xD0;
int16_t  dig_P4 = 0xCE;
int16_t  dig_P5 = 0x6D;
int16_t  dig_P6 = 0xF9;
int16_t  dig_P7 = 0x8C;
int16_t  dig_P8 = 0xF8;
int16_t  dig_P9 = 0x70;

void devID_BMP(void)
{
	printf("Debut de l'ID\r\n");
	HAL_I2C_Master_Transmit(&hi2c1, BMPAddress << 1,&dev_ID, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, (BMPAddress << 1)|0x01, dataBuffer, sizeof(dataBuffer), HAL_MAX_DELAY);
	printf("%s",dataBuffer);
	printf("Fin ID\r\n");
}

void setConfig_BMP(void)
{
	printf("Debut de la config\r\n");
	HAL_I2C_Master_Transmit(&hi2c1, BMPAddress << 1,TabConfig, 2, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, (BMPAddress << 1) | 0x01, Sendback, sizeof(Sendback), HAL_MAX_DELAY);
	uint8_t newvalue= Sendback[0];
	printf("%02X\r\n",newvalue);
}


uint32_t temperatureNonCompense(void){
	HAL_I2C_Master_Transmit(&hi2c1, BMPAddress << 1,&TMsbAdress, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, (BMPAddress << 1) | 0x01, T_Msb, sizeof(T_Msb), HAL_MAX_DELAY);
	uint8_t MSB= T_Msb[0];

	HAL_I2C_Master_Transmit(&hi2c1, BMPAddress << 1,&TLsbAdress, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, (BMPAddress << 1) | 0x01, T_Lsb, sizeof(T_Lsb), HAL_MAX_DELAY);
	uint8_t LSB= T_Lsb[0];

	HAL_I2C_Master_Transmit(&hi2c1, BMPAddress << 1,&TxLsbAdress, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, (BMPAddress << 1) | 0x01, T_xLsb, sizeof(T_xLsb), HAL_MAX_DELAY);
	uint8_t xLSB= T_xLsb[0];

	TempValue |= ((uint32_t)MSB << 12);
	TempValue |= ((uint32_t)LSB << 4);
	TempValue |= (xLSB & 0x0F);

	printf("La temprature non compense est de : %lX\r\n", TempValue);
	printf("\n");
	return TempValue;
}
uint32_t pressionNonCompense(void){
	HAL_I2C_Master_Transmit(&hi2c1, BMPAddress << 1,&PMsbAdress, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, (BMPAddress << 1) | 0x01, P_Msb, sizeof(P_Msb), HAL_MAX_DELAY);
	uint8_t PMSB= P_Msb[0];

	HAL_I2C_Master_Transmit(&hi2c1, BMPAddress << 1,&PLsbAdress, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, (BMPAddress << 1) | 0x01, P_Lsb, sizeof(P_Lsb), HAL_MAX_DELAY);
	uint8_t PLSB= P_Lsb[0];

	HAL_I2C_Master_Transmit(&hi2c1, BMPAddress << 1,&PxLsbAdress, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, (BMPAddress << 1) | 0x01, P_xLsb, sizeof(P_xLsb), HAL_MAX_DELAY);
	uint8_t PxLSB= P_xLsb[0];

	PressValue |= ((uint32_t)PMSB << 12);
	PressValue |= ((uint32_t)PLSB << 4);
	PressValue |= (PxLSB & 0x0F);

	printf("La pression non compense  est de : %lX\r\n", PressValue);
	printf("\n");
	return PressValue;

}
BMP280_S32_t t_fine;
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
	BMP280_S32_t var1, var2, T;
	var1 = ((((adc_T>>3) - ((BMP280_S32_t)dig_T1<<1))) * ((BMP280_S32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((BMP280_S32_t)dig_T1)) * ((adc_T>>4) - ((BMP280_S32_t)dig_T1))) >> 12) *
			((BMP280_S32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}
// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
BMP280_U32_t bmp280_compensate_P_int32(BMP280_S32_t adc_P)
{
	BMP280_S32_t var1, var2;
	BMP280_U32_t p;
	var1 = (((BMP280_S32_t)t_fine)>>1) - (BMP280_S32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((BMP280_S32_t)dig_P6);
	var2 = var2 + ((var1*((BMP280_S32_t)dig_P5))<<1);
	var2 = (var2>>2)+(((BMP280_S32_t)dig_P4)<<16);
	var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((BMP280_S32_t)dig_P2) * var1)>>1))>>18;
	var1 =((((32768+var1))*((BMP280_S32_t)dig_P1))>>15);
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = (((BMP280_U32_t)(((BMP280_S32_t)1048576)-adc_P)-(var2>>12)))*3125;
	if (p < 0x80000000)
	{
		p = (p << 1) / ((BMP280_U32_t)var1);
	}
	else
	{
		p = (p / (BMP280_U32_t)var1) * 2;
	}
	var1 = (((BMP280_S32_t)dig_P9) * ((BMP280_S32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((BMP280_S32_t)(p>>2)) * ((BMP280_S32_t)dig_P8))>>13;
	p = (BMP280_U32_t)((BMP280_S32_t)p + ((var1 + var2 + dig_P7) >> 4));
	return p;
}




