/*********************
 * Projeto RoboMovel
 * Arquivo: protocolo.h
 * Descricao: Reune o mapeamentos de dispositivos a setem utilizados
 *            na comunicacao serial com a interace de controle
 * Autor:Douglas Melchioretto
 *
 * Versao Atualizada: 02/06/2010
*********************/

// ATENCAO ! OS VALORES COMPREENDIDOS DE 0X00h A 0X1Fh SAO RESERVADOS
// PADRAO RS-232.
// MAIS INFORMACOES: http://easysw.com/~mike/serial/serial.html#advanced
// Sera que precisaremos converter em string (char[]) antes de transmitir
// para evitar o uso de simbolos reservados ASCII ?

#define ON                0x0001
//#define TRUE		      0x0001
#define OFF               0x0000
//#define FALSE		      0x0000

#define ENCODER					0x0020	 	
//#define ENCODER_DIREITO       	0x0020
//#define ENCODER_ESQUERDO      	0x0021

// Se implementar leitura em funcao de sensor, precisa contatenar o estado..
// ON ou OFF ou a medida 1BYTE (0 a 255 CM)
// Identificacoo dos Sensores

#define SENSOR_OPTICO_0       0x0022
#define SENSOR_OPTICO_1       0x0023
#define SENSOR_OPTICO_2       0x0024
#define SENSOR_OPTICO_3       0x0025
#define SENSOR_OPTICO_4       0x0026
#define SENSOR_OPTICO_5       0x0027

#define ENCODER_VALUE_SIZE    0x0004 // 4 bytes (32 bits)
// Numero de Bytes - 32 bits - O Robo vai ter que andar 1385 km para estourar esse contador :D
// Tendo em vista que o encoder eh de 2000 pulsos por volta da roda que tem 64,5 cm de comprimento
// Se colocar apenas 2 bytes, soh podermos medir 21 m e 3 bytes (nada usual) - 5,4 km
// Ainda podemos economizar esse bytes na transmissao, transmitindo apenas a diferenca....
// Dessa maneira utilizando 1 byte como tamanho maximo, pode-se descrever um deslocamento de ate 8,256 cm
// Vale lembrar que caso sejam modificadas as caracteristicas mecanicas do sistema de propulsao (rodas) essa conta
// deve ser refeita.

// Implementacao dos estados dos sensores ON/OFF

#define SENSOR_ONOFF_1    0x0030
#define SENSOR_ONOFF_2    0x0031
#define SENSOR_ONOFF_3    0x0032
#define SENSOR_ONOFF_4    0x0034
#define SENSOR_ONOFF_5    0x0035
#define SENSOR_ONOFF_6    0x0036
#define SENSOR_ONOFF_7    0x0037
#define SENSOR_ONOFF_8    0x0038
#define SENSOR_ONOFF_9    0x0039
#define SENSOR_ONOFF_10   0x003A
#define SENSOR_ONOFF_11   0x003B
#define SENSOR_ONOFF_12   0x003C
#define SENSOR_ONOFF_13   0x003D
#define SENSOR_ONOFF_14   0x003E
#define SENSOR_ONOFF_15   0x003F

#define SONAR_PAN         0x0050 // IDENTIFICADOR DO ANGULO PAN (PWM) DO SONAR ( 1 Byte 0 - 255 )
#define SONAR_DISTANCIA   0x0051 // IDENTIFICADOR DA DISTANCIA DO SONAR (2 Bytes)
#define BUSSOLA           0x0052 // IDENTIFICADOR DA BUSSOLA ELETRONICA
                                 // Precisa identificar qual serao os dados transmitidos
#define GPS		          0x53   // IDENTIFICADOR DE INFORMACAO DO GPS
#define NMEA               183   // VERSAO NMEA
#define NMEA_SPEED        4800 // TAXA DE TRANSMISSAO PADRAO NMEA-0183
// Mais informacoes uteis sobre NMEA-0183
// http://www.kh-gps.de/nmea.faq

// Tags que identificam comandos a serem enviados ao ROBO

#define RODA_LEFT        0xA0 // Nivel das PWMS de Movimentacao ou passos
#define RODA_RIGHT       0xA1 // Nivel das PWMS de Movimentacao ou passos

//#define QQCOISA	 0xA2
//#define QQCOISA2	 0xA3

#define CAMERA_PAN       0xA4
// Vai definir o Angulo Panoramico (Esquerda-Direita) 127 = CENTRO
#define CAMERA_TILT      0xA5
// Informacao Absoluta - Vai definir Angulo de Inclinacao (Acima-Abaixo) 127 = CENTRO
#define CAMERA_ZOOM      0xA6 // Reservado para possivel uso futuro...

#define SONAR_PAN_SET    0xA7

#define STOP             0xFF // Parada De Emergencia !!!!
// Caso a conexao tcp caia ou um evento watchdog ocorra ( timeout nos pacotes keep-alive
// do sinal de monitoramento ) parara imediatamente o robo !

#define SONAR_DISPARO	 0x00F1

#define DISPAROS_MEDIA   0x000A
// Solicita um disparo do sonar de maneira assincrona, ou seja fora do modo de varredura.
//
#define SONAR_VARREDURA  0x00F2
// Ativa modo de varredura automatica e envio das informacoes dos encoders, do sonar (e do angulo de PAN do sonar, tendo em vista que no modo varredura ele se modifica automaticamente)

#define SONAR_MEDIDA    0x00F3

#define RESET           0x00F0 // Ordem para RESET GLOBAL
#define FIM_COMANDO	    0x00FE

#define MASCARA_SENTIDO  0x0080  // Mascara que define o bit de sentido da PWM
#define MASCARA_PWM      0x007F  // Mascara que define o bit de valor   da PWM
