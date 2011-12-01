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
// Constantes
//
// -------------------------------------------------------------
const int NSENSORS = 6;
const int NENCODERS = 2;
const int SZ_MSG_SENSOR = 4;
const int SZ_MSG_ENCODER = 5;
const int distances[256] = {
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
const int MAX_VELOCITY = 15;

// -------------------------------------------------------------
//
// Variaveis Globais
//
// -------------------------------------------------------------
struct termios stdio_bak;
int tty_fd;
int vLeft = 0, vRight = 0;
enum {LEFT, RIGHT};

// -------------------------------------------------------------
//
// Funcoes Auxiliares
//
// -------------------------------------------------------------

int MIN(int a, int b){ return a < b ? a : b; }
int MAX(int a, int b){ return a > b ? a : b; }

int open_serial_port()
{
	struct termios tio;
	struct termios stdio;

	tcgetattr(STDOUT_FILENO, &stdio_bak);

	memset(&stdio,0,sizeof(stdio));
	stdio.c_iflag=0;
	stdio.c_oflag=0;
	stdio.c_cflag=0; stdio.c_lflag=0;
	stdio.c_cc[VMIN]=1;
	stdio.c_cc[VTIME]=0;
	tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
	tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

	memset(&tio,0,sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;

	tty_fd=open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK);      
	cfsetospeed(&tio,B115200);            // 115200 baud
	cfsetispeed(&tio,B115200);            // 115200 baud

	tcsetattr(tty_fd,TCSANOW,&tio);
}

void send_package(char msg[], int msg_size)
{	
	for (int i = 0; i < msg_size; i++) write(tty_fd, &msg[i], 1);
}

int read_package()
{
	int SZ_PKG = NSENSORS * SZ_MSG_SENSOR + NENCODERS * SZ_MSG_ENCODER;
	int OFFSET = NSENSORS * SZ_MSG_SENSOR;
	int bytes_received = 0, i, j, msg_size = 3;
	char msg[8];
	char rcv[1024];
	unsigned char c='D';
	clock_t last_sync = clock();
	clock_t last_warn = clock();
	fd_set rdset;
	msg[0] = SYNC;
	msg[1] = END_CMD;
	msg[2] = 10;
	int rb = SZ_PKG;
	int first = 1;
	static int npkg = 0;
	while (1)
	{
		if((clock() - last_warn) / (double) CLOCKS_PER_SEC > 1){
			if (!first)
				printf("Esperando %d bytes, recebidos %d bytes\r\n", rb, bytes_received);
			first = 0;
			//for(i = 0; i < msg_size; ++i) write(tty_fd, &msg[i], 1);
			send_package(msg, 2);
			rb = SZ_PKG;
			bytes_received = 0;
			last_warn = clock();
		}
		if (read(tty_fd,&c,1)>0) {
			rcv[bytes_received++] = c;
			--rb;
		}
		if(!rb){
			printf("=== Pacote %4d ===\r\n", ++npkg);
			while((clock() - last_sync) / (double)CLOCKS_PER_SEC < 0.5);
			for(i = 0; i < 6; ++i)
			{
				unsigned char high;
				high = 256 + rcv[SZ_MSG_SENSOR * i + 1];
				printf("Sensor %d: %3d cm (%d)\r\n", rcv[SZ_MSG_SENSOR * i] - 34, distances[high], high);
			}
			for(i = 0; i < 2; ++i)
			{
				unsigned char high, low;
				high = 256 + rcv[OFFSET + 1 + i * SZ_MSG_ENCODER];
				low = 256 + rcv[OFFSET + i * SZ_MSG_ENCODER + 2];  
				printf("Encoder %d: %d (%u %u)\r\n", rcv[OFFSET + i * 5] - 32, 
						high << 8 | low,
						high, low);	
			}
			last_sync = clock();
			last_warn = last_sync;
			return 1;
		}
		if (read(STDIN_FILENO, &c, 1) > 0 && (c == 'q' || c == 'Q')) return 0;
	}

}

void accelerate(int wheel, int dv)
{
	char msg_pwm[4];
	if (wheel == LEFT)
	{
		msg_pwm[0] = LEFT_WHEEL;
		msg_pwm[1] = vLeft = MAX(MIN(vLeft + dv, MAX_VELOCITY), 0);
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
	int cnt = 0;
	open_serial_port();
	while (1)
	{
		if (read_package() == 0){
			tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio_bak);
			printf("Recebi comando sair\n");
			break;
		}
		if(!((cnt / 15) & 1)){
			accelerate(LEFT, 1);
			accelerate(RIGHT, 1);
		} else{
			accelerate(LEFT, -1);
			accelerate(RIGHT, -1);
		}
		printf("LEFT: %2d \r\nRIGHT: %2d \r\n", vLeft, vRight);
		cnt++;
	}
	close(tty_fd);
}



