#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include "protocolo.h"

int main(int argc,char** argv)
{
	struct termios tio;
	struct termios stdio;
	int i, j, tty_fd;
	int bytes_received = 0;
	int msg_size = 3;
	char msg[8];
	char rcv[1024];
	clock_t last_sync = clock();
	clock_t last_warn = clock();
	fd_set rdset;

	unsigned char c='D';
	// printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
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

	tty_fd=open(argv[1], O_RDWR | O_NONBLOCK);      
	cfsetospeed(&tio,B115200);            // 115200 baud
	cfsetispeed(&tio,B115200);            // 115200 baud

	tcsetattr(tty_fd,TCSANOW,&tio);
	msg[0] = SYNC;
	msg[1] = END_CMD;
	msg[2] = 10;
	int rb = 0;
	while (1)
	{
		if (read(tty_fd,&c,1)>0) {
			rcv[bytes_received++] = c;
			--rb;
			//			write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
		}
		//		if (read(STDIN_FILENO,&c,1)>0)  write(tty_fd,&c,1);                     // if new data is available on the console, send it to the serial port
		if((clock() - last_warn) / (double) CLOCKS_PER_SEC > 1){
			printf("esperando %d bytes\r\n", rb);
			for(i = 0; i < msg_size; ++i) write(tty_fd, &msg[i], 1);
			last_warn = clock();
		}
		if(!rb){
			rb = 34;
			while((clock() - last_sync) / (double)CLOCKS_PER_SEC < 0.5);
			for(i = 0; i < 6; ++i)
				printf("Sensor %d: %d\r\n", rcv[4 * i] - 34, rcv[4 * i + 1]);
			for(i = 0; i < 2; ++i)
				printf("Encoder %d: %d\r\n", rcv[24 + i * 5] - 32, (rcv[25 + i * 5] << 8) | rcv[26 + i * 5]);
			for(i = 0; i < msg_size; ++i) write(tty_fd, &msg[i], 1);
			last_sync = clock();
			last_warn = last_sync;
		}
	}

	close(tty_fd);
}

