/*************************
* Arquivo: protocolo.h
* Descricao: contem a definicao dos comandos que sao
* enviados/recebidos pelo robo
**************************/

#define ON                0x0001
#define OFF               0x0000

#define ENCODER					0x0020	 	

// Sensores de distancia
#define SENSOR_OPTICO_0       0x0022
#define SENSOR_OPTICO_1       0x0023
#define SENSOR_OPTICO_2       0x0024
#define SENSOR_OPTICO_3       0x0025
#define SENSOR_OPTICO_4       0x0026
#define SENSOR_OPTICO_5       0x0027

// Comandos para controlar PWM
#define RODA_LEFT        0xA0 // Nivel das PWMS de Movimentacao ou passos
#define RODA_RIGHT       0xA1 // Nivel das PWMS de Movimentacao ou passos

// Comando para fazer o robo parar imediatamente
#define STOP             0xFF // Parada De Emergencia !!!!

#define RESET           0x00F0 // Ordem para RESET GLOBAL
// Byte que sinaliza fim do comando
#define FIM_COMANDO	    0x00FE

// Mascara para obter o valor do pwm e o sentido
// o bit mais significativo representa o sentido,
// enquanto os restantes representam o valor de PWM
#define MASCARA_SENTIDO  0x0080  // Mascara que define o bit de sentido da PWM
#define MASCARA_PWM      0x007F  // Mascara que define o bit de valor   da PWM
