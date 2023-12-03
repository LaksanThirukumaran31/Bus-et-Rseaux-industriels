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
Premieèrement nous voulons réaliser la mise en oeuvre du BMP280. Le BMP280 est un capteur de température et de pression développé par Bosh. Ce capteur utilise l'I2C comme protocole de communication. Nous pouvons modifier ou lire les valeurs de certains registres pour avoir l'ID du capteur, configurer le capteur ou récupérer des valeurs. <br> 
Voici les différents registres : 

AJT IMAGE REGISTRE 
-> Les adresses I2C possibles pour réaliser une communication avec le capteur sont:<br>
En écriture : (0x77<<1)
En lecture :  (0x77<<1) | 0x01
-> Le registre qui permet d'identifier le composant est le 0xD0 et la valeur est 0x58. Pour tester l'identification du composant, nous utilisons la fonction ```c devID_BMP()```. Nous utilisons les fonctions ```c
HAL_I2C_Master_Transmit(```) et ```c HAL_I2C_Master_Transmit() ``` pour lire un régistre et récupérer la valeur. 

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

les fonctions permettant le calcul de la température et de la pression compensées, en format entier 32 bits.

-> Pour modifier le fonctionnement du composant, il faut modifier le registre 0xF4. Les registres pour lire la température sont : 0xFA à 0xFC et 0xF7 à 0xF9 pour lire la pression. La température et la pression sont répresentés sur 5 octets ( 8 bits MSB, 8 bits LSB, 4 bits xLSB). 
