#include <cstdio>
#include <ctime>
#include "protocolo.h"

// constantes
const int MAX_VELOCITY = 15;

// Variaveis globais para controle do robo
enum { LEFT, RIGHT };
int vLeft = 0, vRight = 0;

int MIN(int a, int b){ return a < b ? a : b; }
int MAX(int a, int b){ return a > b ? a : b; }

// {{{ Comunicacao com o robo
void send_message(char cmd, char value = 0){
	putc(cmd, stdout);
	putc(value, stdout);
	putc(END_CMD, stdout);
	putc(10, stdout);
}
// }}}

// {{{ Funcoes de leitura dos dados do robo
void read_distances(){

} 
void read_encoders(){

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
	for(int cnt = 0; ; ++cnt){
		clock_t start = clock();
		// Envia o pedido de sincronizacao
		send_message(SYNC);
		// Ler as informacoes dos sensores de distancia read_distances();
		read_distances();
		// Ler as informacoes dos encoders
		read_encoders();
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
		while((clock()- start)/(double)CLOCKS_PER_SEC < 0.5);
	}
	return 0;
}
// }}}
