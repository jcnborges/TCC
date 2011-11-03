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

void send_message(char cmd, char value){
	putc(cmd, stdout);
	putc(value, stdout);
	putc(FIM_COMANDO, stdout);
	putc(10, stdout);
}
// {{{ Funcoes de leitura
void read_distances(){

}

void read_encoders(){

}

void execute_action(){

}
// }}} Fim funcoes leitura
// Funcao para alterar velocidade de uma roda
void accelerate(int wheel, int dv){
	if(wheel == LEFT){
		vLeft = MAX(MIN(vLeft + dv, MAX_VELOCITY), 0);
		send_message(RODA_LEFT, vLeft);
	}
	else if(wheel == RIGHT){
		vRight = MAX(MIN(vRight + dv, MAX_VELOCITY), 0);
		send_message(RODA_RIGHT, vRight);
	}
}

int main(void){
	for(int cnt = 0; ; ++cnt){
		clock_t start = clock();
		if(!((cnt / 15) & 1)){
			accelerate(LEFT, 1);
			accelerate(RIGHT, 1);
		} else{
			accelerate(LEFT, -1);
			accelerate(RIGHT, -1);
		}
		// Ler as informacoes dos sensores de distancia read_distances();
		read_distances();
		// Ler as informacoes dos encoders
		read_encoders();
		// Chamar algoritmo navegacao para tratar dados
		// TODO
		// Executa as acoes definidas pelo algoritmo
		execute_action();
		// Espera completar 100ms para executar a rotina novamente
		while((clock()- start)/(double)CLOCKS_PER_SEC < 0.5);
	}
	return 0;
}
