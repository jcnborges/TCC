#include <cmath>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 
#include <termios.h>
#include <time.h>
#include "protocolo.h" 
#include "flie/flie.h"
#include "fcm.h"
// -------------------------------------------------------------
// Flags Debug
// -------------------------------------------------------------

//#define DBG_SETUP
//#define DBG_COM2
//#define DBG_PKG
//#define DBG_ACC
#define DBG_INF
#define DBG_SP
#define vLeft v[LEFT]
#define vRight v[RIGHT]
#define FUZZY 1
#define FCM 2

// -------------------------------------------------------------
// Constantes
// -------------------------------------------------------------
enum {LEFT, RIGHT, NENCODERS};
enum {L30, MID, NOT, R30, R60, L60, NSENSORS};

const int NADJUSTMENTS = 2; // Numero de ajustes entre as execucoes do algoritmo
const int SZ_MSG_ENCODER = 5; // Tamanho do pacote de dados de um encoder (bytes)
const int SZ_MSG_SENSOR = 4; // Tamanho do pacote de dados de um sensor (bytes)
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
const int MAX_VELOCITY = 51; // Maximo passo de velocidade do robo
const double TIME_WINDOW = 0.075; // Duracao minima de uma operacao
const char* SNAME[] = { "L30", "MID", "___", "R30", "R60", "L60"};
const char* ENAME[] = { "LEFT", "RIGHT" };

// -------------------------------------------------------------
// Variaveis Globais
// -------------------------------------------------------------
int tty_fd; // File descriptor do terminal (COM2) TS-7260
int v[2] = {0, 0}; // Velocidade atual das rodas
int SP[NENCODERS];
unsigned int last_dist[NSENSORS];
int last_step[NENCODERS];

// -------------------------------------------------------------
// Funcoes Auxiliares
// -------------------------------------------------------------

inline int MIN(int a, int b){ return a < b ? a : b; }
inline int MAX(int a, int b){ return a > b ? a : b; }

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

void show_info(){
	int i;
	printf("Sensor info\nL60  L30  MID  R30  R60  \n");
	printf("%-4d %-4d %-4d %-4d %-4d\n", last_dist[L60], last_dist[L30], last_dist[MID],
			last_dist[R30], last_dist[R60]);
	for(i = 0; i < NENCODERS; ++i) printf("Encoder %6s: %4d%4d [%3d]\n", ENAME[i], last_step[i], SP[i], v[i]);
}

void send_package(char msg[], int msg_size)
{	
	for (int i = 0; i < msg_size; i++) write(tty_fd, &msg[i], 1);
}

void accelerate(int wheel, int dv)
{
	char msg_pwm[4];
#ifdef DBG_ACC
	printf("Accelerate %5s %3d --> ", ENAME[wheel], dv);
#endif
	if (wheel == LEFT)
	{
		msg_pwm[0] = LEFT_WHEEL;
		vLeft = MAX(MIN(vLeft + dv, MAX_VELOCITY), -MAX_VELOCITY);
		msg_pwm[1] = abs(vLeft);
		if(vLeft < 0) msg_pwm[1] |= 0x80;
#ifdef DBG_ACC
		printf("LEFT_WHEEL  %3d\n", vLeft);
#endif
	} else if (wheel == RIGHT) {
		msg_pwm[0] = RIGHT_WHEEL;
		vRight = MAX(MIN(vRight + dv, MAX_VELOCITY), -MAX_VELOCITY);
		msg_pwm[1] = abs(vRight);
		if(vRight < 0) msg_pwm[1] |= 0x80;
#ifdef DBG_ACC
		printf("RIGHT_WHEEL %3d\n", vRight);
#endif
	}
	msg_pwm[2] = END_CMD;
	send_package(msg_pwm, 3);
}

void adjust_velocity(){
	int diff, step;
	for(int i = 0; i < NENCODERS; ++i){
		step = 0;
		if(v[i] < 0) last_step[i] = -last_step[i];
		diff = SP[i] - last_step[i];
		if(abs(diff) > 10) step = 8;
		else if(diff) step = 1;
		if(diff < 0) step = -step;
		accelerate(i, step);
	}
}

void test_speed(){
	static int cnt = 0;
	if(cnt < MAX_VELOCITY || cnt >= 3*MAX_VELOCITY){
		accelerate(LEFT, 1);
		accelerate(RIGHT, 1);
	} else {
		accelerate(LEFT, -1);
		accelerate(RIGHT, -1);
	}
	if(++cnt == MAX_VELOCITY * 4) cnt = 0;
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
#endif
			//while((clock() - last_sync) / (double)CLOCKS_PER_SEC < 0.5);
			for(i = 0; i < NSENSORS; ++i)
			{
				unsigned char high;
				high = 256 + rcv[SZ_MSG_SENSOR * i + 1];
#ifdef DBG_PKG
				printf("Sensor %d: %3d cm (%d)\r\n", rcv[SZ_MSG_SENSOR * i] - 34, distances[high], high);
#endif
				last_dist[i] = distances[high];
			}
			for(i = 0; i < NENCODERS; ++i)
			{
				unsigned char high, low;
				high = 256 + rcv[OFFSET + 1 + i * SZ_MSG_ENCODER];
				low = 256 + rcv[OFFSET + i * SZ_MSG_ENCODER + 2];  
				last_step[i] = ((high << 8) | low);
#ifdef DBG_PKG
				printf("Encoder %d: %6d (%3u %3u)\r\n", rcv[OFFSET + i * 5] - 32, 
						last_step[i], high, low);	
#endif
			}
			last_sync = clock();
			return 1;
		}
	}

}


void set_setpoint(int wheel, int value){
	SP[wheel] = value;
#ifdef DBG_SP
	printf("SP[%5s] = %3d\n", ENAME[wheel], value);
#endif
}

void set_setpoint(int value){
	set_setpoint(LEFT, value);
	set_setpoint(RIGHT, value);
}

void run_fuzzy(){
	static bool first = true;
	static flie flie;
	float angle, velocity;
	if(first){
		first = false;
		flie.flie_setup();
	}
	if(last_dist[R60] == -1) last_dist[R60] = 250;
	if(last_dist[L60] == -1) last_dist[L60] = 250;

	float left = float(MIN(last_dist[L30], last_dist[L60]));
	float right = float(MIN(last_dist[R30], last_dist[R60]));
	float mid = float(last_dist[MID]);

	flie.inference(left, mid, right, velocity, angle);
#ifdef DBG_INF
	printf("%.3f %.3f %.3f ==> ", left, mid, right);
	printf("V = %.3f A = %.3f\n", velocity, angle);
#endif

	int setpoint = int(floor(velocity * MAX_VELOCITY / 85.0 + .5));
	int setLeft, setRight;
	setLeft = setRight = setpoint;
	if(angle >= 90.0) setRight = int(setpoint * (1 - (angle - 90) / 45));
	else setLeft = int(setpoint * (angle / 45 - 1));
	set_setpoint(LEFT, setLeft);
	set_setpoint(RIGHT, setRight);
	return;
}

void run_fcm()
{
	float setRight, setLeft;
	float maxSP = MAX_VELOCITY / 100.0;
	int left = (MIN(last_dist[L30], last_dist[L60]));
	int right = (MIN(last_dist[R30], last_dist[R60]));
	int mid = (last_dist[MID]);
	inference(left, mid, right, setRight, setLeft);
	setRight = floor(setRight * maxSP + 0.5);
	setLeft = floor(setLeft * maxSP + 0.5);
	set_setpoint(LEFT, (int) setLeft);
	set_setpoint(RIGHT, (int) setRight);
}

int main(int argc, char *argv[])
{
	int i, cnt = 0, cnt_t = 0;
	int min_dist;
	int ALGORITHM = -1;
	clock_t op_begin; 
	char command[8];
	open_serial_port();

	set_setpoint(0);
	last_step[LEFT] = last_step[RIGHT] = 0;
	accelerate(LEFT, 0);
	accelerate(RIGHT, 0);
	for(int i = 1; i < argc; ++i){
		printf("ARGUMENT: %s\n", argv[i]);
		if(!strcmp(argv[i], "fuzzy") || atoi(argv[i]) == 1){
			ALGORITHM = FUZZY;
			puts("Running Fuzzy algorithm");
		} else if(!strcmp(argv[i], "fcm") || atoi(argv[i]) == 2){
			ALGORITHM = FCM;
			puts("Running FCM algorithm");
		}
	}
	if(ALGORITHM == -1) puts("No algorithm specified, running test routine");
	do { scanf("%s", command); } while(strcmp(command, "bacon"));
	for(cnt = 0; ; ++cnt){
		op_begin = clock();
		read_package();

		if(ALGORITHM != -1) adjust_velocity();

		if(cnt == NADJUSTMENTS){
#ifndef DBG_PKG
			show_info();
#endif
			if(ALGORITHM == -1){
				test_speed();
			} else if(ALGORITHM == FUZZY){
				run_fuzzy();
			} else if(ALGORITHM == FCM){
				run_fcm();
			} 
			cnt = 0;
		}
		//printf("Operacao completa, tempo: %.3lfs\n", (clock() - op_begin) / (double) CLOCKS_PER_SEC);
		while((clock() - op_begin) / (double) CLOCKS_PER_SEC < TIME_WINDOW);
	}
	close(tty_fd);
}

