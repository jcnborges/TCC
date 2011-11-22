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
				printf("Sensor %d: %d\r\n", rcv[SZ_MSG_SENSOR * i] - 34, rcv[SZ_MSG_SENSOR * i + 1]);
			for(i = 0; i < 2; ++i)
				printf("Encoder %d: %d\r\n", rcv[OFFSET + i * 5] - 32, 
						(rcv[OFFSET + 1 + i * SZ_MSG_ENCODER] << 8) | rcv[OFFSET + i * SZ_MSG_ENCODER + 2]);
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

