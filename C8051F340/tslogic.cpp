#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include "protocolo.h"


// -------------------------------------------------------------
//
// Flags Debug
//
// -------------------------------------------------------------

//#define DBG_SETUP
//#define DBG_COM2
#define DBG_PKG

// -------------------------------------------------------------
//
// Constantes
//
// -------------------------------------------------------------
const int NSENSORS = 6; // Numero de sensores
const int NENCODERS = 2; // Numero de encoders
const int SZ_MSG_SENSOR = 4; // Tamanho do pacote de dados de um sensor (bytes)
const int SZ_MSG_ENCODER = 5; // Tamanho do pacote de dados de um encoder (bytes)
const int distances[256] = { // Tabela de conversao de distancias (valor DA para cm)
 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
 150, 150, 150, 150, 150, 150, 150, 150, 146, 143, 140, 137, 134, 131, 128, 126,
 123, 121, 118, 116, 113, 111, 109, 106, 104, 102, 100,  98,  96,  94,  92,  91,
  89,  87,  85,  84,  82,  81,  79,  78,  76,  75,  74,  72,  71,  70,  68,  67,
  66,  65,  64,  63,  62,  61,  60,  59,  58,  57,  56,  56,  55,  54,  53,  52,
  52,  51,  50,  50,  49,  48,  48,  47,  47,  46,  46,  45,  45,  44,  44,  43,
  43,  42,  42,  42,  41,  41,  40,  40,  40,  39,  39,  39,  38,  38,  38,  37,
  37,  37,  36,  36,  36,  35,  35,  35,  35,  34,  34,  34,  34,  33,  33,  33,
  33,  32,  32,  32,  32,  31,  31,  31,  30,  30,  30,  30,  29,  29,  29,  29,
  28,  28,  28,  28,  27,  27,  27,  27,  26,  26,  26,  25,  25,  25,  25,  24,
  24,  24,  23,  23,  23,  23,  22,  22,  22,  21,  21,  21,  20,  20,  20,  20,
  19,  19,  19,  18,  18,  18,  18,  17,  17,  17,  16,  16,  16,  16,  15,  15,
  15,  15,  14,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
};
const int MAX_VELOCITY = 8; // Maximo passo de velocidade do robo
const double TIME_WINDOW = 0.5; // Duracao minima de uma operacao

// -------------------------------------------------------------
//
// Variaveis Globais
//
// -------------------------------------------------------------
int tty_fd; // File descriptor do terminal (COM2) TS-7260
int vLeft = 0, vRight = 0; // Velocidade das rodas
int last_dist[NSENSORS];
int last_enc[NENCODERS];
enum {LEFT, RIGHT};

// -------------------------------------------------------------
//
// Funcoes Auxiliares
//
// -------------------------------------------------------------

int MIN(int a, int b){ return a < b ? a : b; }
int MAX(int a, int b){ return a > b ? a : b; }

// Abre a conexao serial COM2 da TS-7260
int open_serial_port()
{
	struct termios tio;

	// Configuracoes para serial (ver POSIX guide)
	memset(&tio, 0, sizeof(tio));
	tio.c_iflag = 0;
	tio.c_oflag = 0;
	tio.c_cflag = B115200 | CREAD | CLOCAL;
	tio.c_cflag &= ~PARENB;
	tio.c_cflag &= ~CSTOPB;
	tio.c_cflag &= ~CSIZE;
	tio.c_cflag |= CS8;
	tio.c_lflag = 0;
	tio.c_cc[VMIN] = 0;
	tio.c_cc[VTIME] = 10;

#ifdef DBG_SETUP
	printf("Abrindo conexao serial...\r\n");
#endif
	tty_fd=open("/dev/ttyAM1", O_RDWR | O_NONBLOCK);      
#ifdef DBG_SETUP
	if(tty_fd == -1) printf("Nao consegui abrir a porta serial\n");
	else printf("tty_fd = %d\n", tty_fd);
#endif
	cfsetospeed(&tio, B115200);            // 115200 baud
	cfsetispeed(&tio, B115200);            // 115200 baud
#ifdef DBG_SETUP
	printf("Baud rate alterado para 115200\r\n");
#endif
	tcsetattr(tty_fd, TCSAFLUSH, &tio);
	//fcntl(tty_fd, F_SETFL, O_NONBLOCK); // leitura nao bloqueia
#ifdef DBG_SETUP
	printf("Done.\r\n");
#endif
}

void send_package(char msg[], int msg_size)
{	
	for (int i = 0; i < msg_size; i++) write(tty_fd, &msg[i], 1);
}

int read_package()
{
	static int npkg = 0;
	int SZ_PKG = NSENSORS * SZ_MSG_SENSOR + NENCODERS * SZ_MSG_ENCODER;
	int OFFSET = NSENSORS * SZ_MSG_SENSOR;
	int bytes_received = 0, i, j, msg_size = 3;
	char msg[8];
	char rcv[1024];
	unsigned char c='D';
	clock_t last_sync = clock();
	msg[0] = SYNC;
	msg[1] = END_CMD;
	send_package(msg, 2);
#ifdef DBG_COM2
	printf("Lendo pacote\r\n");
#endif
	while (1)
	{
#ifdef DBG_COM2
		printf("Entrei no while\r\n");
#endif
		if((clock() - last_sync) / ((double) CLOCKS_PER_SEC) > 1){
			printf("Tempo de espera expirou, recebi %d de %d bytes\r\n", bytes_received, SZ_PKG);
#ifdef DBG_COM2
			printf("Entrei no if\r\n");
			printf("Enviando SYNC...\r\n");
#endif
			send_package(msg, 2);
#ifdef DBG_COM2
			printf("Done.\r\n");
#endif
			bytes_received = 0;
			last_sync = clock();
		} 
#ifdef DBG_COM2
		else {
			double et = (clock() - last_sync) / (double) CLOCKS_PER_SEC;
			printf("so foi %.3lfs\n", et); 
		}
		printf("Tentando ler um byte da COM2\n");
#endif
		if (read(tty_fd,&c,1)>0) {
			rcv[bytes_received++] = c;
#ifdef DBG_COM2
			printf("LI UM BYTE %d (%c)\r\n", c, c);
#endif
		}
#ifdef DBG_COM2
		else printf("Nada lido\r\n");
#endif
		// Verifica se um pacote completo foi recebido
		if(bytes_received == SZ_PKG){
#ifdef DBG_PKG
			printf("=== Pacote %4d ===\r\n", ++npkg);
			//while((clock() - last_sync) / (double)CLOCKS_PER_SEC < 0.5);
			for(i = 0; i < 6; ++i)
			{
				unsigned char high;
				high = 256 + rcv[SZ_MSG_SENSOR * i + 1];
				printf("Sensor %d: %3d cm (%d)\r\n", rcv[SZ_MSG_SENSOR * i] - 34, distances[high], high);
				last_dist[i] = distances[high];
			}
			for(i = 0; i < 2; ++i)
			{
				unsigned char high, low;
				high = 256 + rcv[OFFSET + 1 + i * SZ_MSG_ENCODER];
				low = 256 + rcv[OFFSET + i * SZ_MSG_ENCODER + 2];  
				printf("Encoder %d: %6d (%3u %3u)\r\n", rcv[OFFSET + i * 5] - 32, 
						high << 8 | low, high, low);	
				last_enc[i] = high << 8 | low;
			}
#endif
			last_sync = clock();
			return 1;
		}
	}

}

void accelerate(int wheel, int dv)
{
	char msg_pwm[4];
	if (wheel == LEFT)
	{
		msg_pwm[0] = LEFT_WHEEL;
//		msg_pwm[1] = vLeft = MAX(MIN(vLeft + dv, MAX_VELOCITY), 0);
		msg_pwm[1] = vLeft = MIN(vRight + 1, MAX_VELOCITY);
	} else if (wheel == RIGHT)
	{
		msg_pwm[0] = RIGHT_WHEEL;
		msg_pwm[1] = vRight = MAX(MIN(vRight + dv, MAX_VELOCITY), 0);
	}
	msg_pwm[2] = END_CMD;
	msg_pwm[3] = 10;
	send_package(msg_pwm, 3);
}

int main()
{
	int i, cnt = 0;
	int min_dist;
	clock_t op_begin; 
	open_serial_port();
	while (1)
	{
		op_begin = clock();
		read_package();
		min_dist = 1024;
		/* {{{ Codigo de teste dos motores */
		for(i = 0; i < 6; ++i)
			if(last_dist[i] != -1 && last_dist[i] < min_dist)
				min_dist = last_dist[i];
		if(min_dist < 50){
			accelerate(LEFT, -MAX_VELOCITY);
			accelerate(RIGHT, -MAX_VELOCITY);
			cnt = 0;
		} else {
			if(!((cnt / MAX_VELOCITY) & 1)){
				accelerate(RIGHT, 1);
				accelerate(LEFT, 1);
			} else{
				accelerate(RIGHT, -1);
				accelerate(LEFT, -1);
			}
		}
		printf("LEFT : %3d \r\nRIGHT: %3d \r\n", vLeft, vRight);
		cnt++;
		/* }}} */
		printf("Operacao completa, tempo: %.3lfs\n", (clock() - op_begin) / (double) CLOCKS_PER_SEC);
		while((clock() - op_begin) / (double) CLOCKS_PER_SEC < TIME_WINDOW);
	}
	close(tty_fd);
}

