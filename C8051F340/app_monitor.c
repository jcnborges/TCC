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
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 167, 161, 156, 151, 147, 142, 138, 134,
 130, 127, 123, 120, 117, 114, 111, 108, 106, 103, 101,  99,  96,  94,  92,  90,
  88,  86,  85,  83,  81,  80,  78,  77,  75,  74,  73,  71,  70,  69,  68,  66,
  65,  64,  63,  62,  61,  60,  59,  58,  57,  56,  56,  55,  54,  53,  52,  52,
  51,  50,  50,  49,  48,  48,  47,  46,  46,  45,  45,  44,  43,  43,  42,  42,
  41,  41,  40,  40,  39,  39,  38,  38,  38,  37,  37,  36,  36,  36,  35,  35,
  34,  34,  34,  33,  33,  33,  32,  32,  32,  31,  31,  31,  30,  30,  30,  30,
  29,  29,  29,  28,  28,  28,  28,  27,  27,  27,  27,  26,  26,  26,  26,  26,
  25,  25,  25,  25,  25,  24,  24,  24,  24,  24,  23,  23,  23,  23,  23,  22,
  22,  22,  22,  22,  22,  21,  21,  21,  21,  21,  21,  20,  20,  20,  20,  20,
  20,  20,  19,  19,  19,  19,  19,  19,  19,  19,  18,  18,  18,  18,  18,  18,
  18,  18,  17,  17,  17,  17,  17,  17,  17,  17,  17,  16,  16,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
};

// -------------------------------------------------------------
//
// Variaveis Globais
//
// -------------------------------------------------------------
struct termios stdio_bak;
int tty_fd;

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
	//int primeira_vez = 1;
	while (1)
	{
		if((clock() - last_warn) / (double) CLOCKS_PER_SEC > 1){
			if (!first)
				printf("Esperando %d bytes, recebidos %d bytes\r\n", rb, bytes_received);
			first = 0;
			for(i = 0; i < msg_size; ++i) write(tty_fd, &msg[i], 1);
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
				printf("Sensor %d: %d cm\r\n", rcv[SZ_MSG_SENSOR * i] - 34, distances[high]);
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

int main()
{
	open_serial_port();
	while (1)
	{
		if (read_package() == 0){
			tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio_bak);
			printf("Recebi comando sair\n");
			break;
		}
	}
	close(tty_fd);
}

