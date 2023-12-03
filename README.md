# Bus_Reseaux-industriels

# Presentation 

Le TP consiste à utiliser plusieurs bus et réseaux de communication et de mettre en place tous ces composants : <br>
AJOUTER IMAGE TP ENSEMBLE

Le TP est divisé en 5 parties :<br>
1. Interrogation des capteurs par le bus I²2C<br>
2.Interfaçage STM32 <-> Raspberry Pi<br>
3.Interface Web sur Raspberry Pi<br>
4.Interface API Rest & pilotage d'actionneur par bus CAN<br>

# BUS I2C 
L'objectif de cette partie est d'un interfacer une carte STM32 avec deux capteurs I2C : <br>
AJOUTER UNE IMAGE DE LA COM 

# BMP280
Premièrement, nous voulons réaliser la mise en oeuvre du BMP280. Le BMP280 est un capteur de température et de pression développé par Bosh. Ce capteur utilise l'I2C comme protocole de communication. Nous pouvons modifier ou lire les valeurs de certains registres pour avoir l'ID du capteur, configurer le capteur ou récupérer des valeurs. <br> 
Voici les différents registres : <br>

AJT IMAGE REGISTRE 
-> Les adresses I2C possibles pour réaliser une communication avec le capteur sont:<br>
En écriture : (0x77<<1) <br>
En lecture :  (0x77<<1) | 0x01 <br>
-> Le registre qui permet d'identifier le composant est le 0xD0 et la valeur est 0x58. Pour tester l'identification du composant, nous utilisons la fonction ```c devID_BMP()```. Nous utilisons les fonctions ```c
HAL_I2C_Master_Transmit()``` et ```c HAL_I2C_Master_Transmit() ``` pour lire un régistre et récupérer la valeur. <br>

```c
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
  /* La varibales SendBack contient la valeur du registre d'indentification 0xD0 */

}
```

 fonctions permettant le calcul de la température et de la pression compensées, en format entier 32 bits.les

-> Pour modifier le fonctionnement du composant, il faut modifier le registre 0xF4. Les registres pour lire la température sont : 0xFA à 0xFC et 0xF7 à 0xF9 pour lire la pression. La température et la pression sont répresentés sur 5 octets ( 8 bits MSB, 8 bits LSB, 4 bits xLSB). Nous utilisons les fonctions ```c temperatureNonCompense() et pressionNonCompense() ``` pour récupérer les valeurs des registres de température et de pression puis, nous affichons les valeurs non compensés de la pression et de la température. <br>
```c
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
/* On récupère les valeurs (MSB, LSB, xLSB) puis on fais un décallage pour récupérer la valeur sur 5 octets */
```
