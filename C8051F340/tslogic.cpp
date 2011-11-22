#include <cassert>
#include <cstdio>
#include <ctime>
#include "protocolo.h"

#define DELAY
// constantes
const int MAX_VELOCITY = 15;

// Variaveis globais para controle do robo
enum { LEFT, RIGHT };
int vLeft = 0, vRight = 0;
int distances[8];
int encoder_count[2];

// Funcoes auxiliares
int MIN(int a, int b){ return a < b ? a : b; }
int MAX(int a, int b){ return a > b ? a : b; }

// {{{ Comunicacao com o robo
void send_message(char cmd, char value = 0){
	putc(cmd, stdout);
	if(cmd != SYNC) putc(value, stdout);
	putc(END_CMD, stdout);
	putc(10, stdout);
}
// }}}

// {{{ Funcoes de leitura dos dados do robo
void read_distances(){
	char cmd, end, z;
	for(int sensor = 0; sensor < 6; ++sensor){
		cmd = getc(stdin);
		assert(cmd == OPTICAL_SENSOR_0 + sensor);
		z = getc(stdin);
		end = getc(stdin);
		assert(end == END_CMD);
		getc(stdin);
		distances[sensor] = z;
	}
} 
void read_encoders(){
	char lhw, uhw;
	for(int encoder = 0; encoder < 2; ++encoder){
		scanf("%*c%c%c%*c\n", &uhw, &lhw);
		encoder_count[encoder] = (int(uhw) << 8) | lhw;
	}
}

// }}} Fim funcoes leitura

// {{{ Funcoes para controlar o robo
void execute_action(){

}
void accelerate(int wheel, int dv){
	if(wheel == LEFT){
		vLeft = MAX(MIN(vLeft + dv, MAX_VELOCITY), 0);
		send_message(LEFT_WHEEL, vLeft);
	}
	else if(wheel == RIGHT){
		vRight = MAX(MIN(vRight + dv, MAX_VELOCITY), 0);
		send_message(RIGHT_WHEEL, vRight);
	}
}
// }}}

// {{{ Loop de controle
int main(void){
	clock_t start = clock();
#ifdef DELAY
	while((clock() - start)/(double)CLOCKS_PER_SEC < 10);
#endif
	for(int cnt = 0; ; ++cnt){
		// Envia o pedido de sincronizacao
		start = clock();
		send_message(SYNC);
		while((clock() - start)/(double)CLOCKS_PER_SEC < 0.1);
		// Ler as informacoes dos sensores de distancia read_distances();
		read_distances();
		// Ler as informacoes dos encoders
		//read_encoders();
		// Chamar algoritmo navegacao para tratar dados
		// TODO

		// Executa as acoes definidas pelo algoritmo
		if(!((cnt / 15) & 1)){
			accelerate(LEFT, 1);
			accelerate(RIGHT, 1);
		} else{
			accelerate(LEFT, -1);
			accelerate(RIGHT, -1);
		}
		execute_action();
		// Espera completar 100ms para executar a rotina novamente
		start = clock();
		while((clock()- start)/(double)CLOCKS_PER_SEC < 0.5);
	}
	return 0;
}
// }}}
