/*************************
* Arquivo: protocolo.h
* Descricao: contem a definicao dos comandos que sao
* enviados/recebidos pelo robo
**************************/

#define ON					0x01
#define OFF					0x00

// Encoder (encoder indice 0 -> 0x20, encoder 1 -> 0x21)
#define ENCODER				0x20	 	

// Sensores de distancia
#define SENSOR_OPTICO_0		0x22
#define SENSOR_OPTICO_1		0x23
#define SENSOR_OPTICO_2		0x24
#define SENSOR_OPTICO_3		0x25
#define SENSOR_OPTICO_4		0x26
#define SENSOR_OPTICO_5		0x27

// Comandos para controlar PWM
#define RODA_LEFT			0xA0 // Nivel das PWMS de Movimentacao ou passos
#define RODA_RIGHT			0xA1 // Nivel das PWMS de Movimentacao ou passos

// Comando para sincronizacao
#define SYNC				0xB0

// Comando para fazer o robo parar imediatamente
#define STOP				0xFF // Parada De Emergencia !!!!

#define RESET				0xF0 // Ordem para RESET GLOBAL

// Byte que sinaliza fim do comando
#define FIM_COMANDO			0xFE

// Mascara para obter o valor do pwm e o sentido
// o bit mais significativo representa o sentido,
// enquanto os restantes representam o valor de PWM
#define MASCARA_SENTIDO		0x80  // Mascara que define o bit de sentido da PWM
#define MASCARA_PWM			0x7F  // Mascara que define o bit de valor   da PWM

