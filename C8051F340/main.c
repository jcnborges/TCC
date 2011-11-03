#include "reg52_ext.h"              /* define 8052 registers */
//#include <stdio.h>                  /* define I/O functions  */
#include "protocolo.h"				/* arquivo de definicoes de constantes */


//#define DEBUG 				// SE COMENTAR ESSE DEFINE, GERA O CODIGO SEM AS MENSAGEMS DE DEBUG !!!
#define PWM_RIGHT_FORWARD P1_0	// DEFINE O PINO NO QUE GERA A PWM PARA A RODA DIREITA GIRAR PARA FRENTE
#define PWM_RIGHT_REVERSE P1_2	// DEFINE O PINO NO QUE GERA A PWM PARA A RODA DIREITA GIRAR PARA TRAS
#define PWM_LEFT_FORWARD  P1_4	// DEFINE O PINO NO QUE GERA A PWM PARA A RODA ESQUERA GIRAR PARA FRENTE
#define PWM_LEFT_REVERSE  P1_6	// DEFINE O PINO NO QUE GERA A PWM PARA A RODA ESQUERA GIRAR PARA TRAS

#define TERRA   0x1F
#define VCC     0x1F
#define CANAL_0 0x00 		// DIRECIONA A LEITURA DO CANAL 0 PARA o PINO P2.0
#define CANAL_1 0x01 		// DIRECIONA A LEITURA DO CANAL 1 PARA o PINO P2.1
#define CANAL_2 0x02 		// DIRECIONA A LEITURA DO CANAL 2 PARA o PINO P2.2
#define CANAL_3 0x03 		// DIRECIONA A LEITURA DO CANAL 3 PARA o PINO P2.3
#define CANAL_4 0x04		// DIRECIONA A LEITURA DO CANAL 4 PARA o PINO P2.5 
#define CANAL_5 0x05		// DIRECIONA A LEITURA DO CANAL 5 PARA o PINO P2.6

#define SYSCLK       48000000   // SYSCLK frequency in Hz
#define BAUDRATE0      115200   // Baud rate of UART0 in bps
#define BAUDRATE1      115200   // Baud rate of UART1 in bps
sfr16   SBRL1 = 0xB4;

#define PASSO_DE_VELOCIDADE 5	// DEFINE O PASSO DE VELOCIDADE. PODE SER DIMINUIDO PARA OBTER MAIS NIVEIS
#define TMAX 75			// PWMS com duty cycle de 75 overflows do timer0

unsigned int pwm_right=0;	// PWM RODA DIREITA
unsigned int pwm_left=0;	// PWM RODA ESQUERDA

unsigned char sensorCount = 0;	// Marca qual sensor acabou de ter sua leitura convertida.
unsigned char valores_velhos[6];// armazena as conversoes mais antigas
unsigned char valores_novos[6];	// Armazena as conversoes mais recentes.
bit flag_nova_conversao=0;	// Indica que ocorreu uma varredura completa dos sensores.
bit flag_novo_comando=0;	// Indica que foi recebido um novo comando.
unsigned int timer3count = 0;	// Contador para gerar delay no Timer 3.

unsigned char Rx_Buff;
unsigned char Tx_Buff;

bit sentido;					// SENTIDO GERAL DO ROBO... TALVEZ NAO SEJA MAIS NECESSARIA LOGO...

bit flag_pode_enviar = 0;
bit sentido_roda_esquerda=1;	// DEFINE O SENTIDO DE ROTACAO DA RODA ESQUERA - MUDA NA EXECUCAO DO SOFTWARE
bit sentido_roda_direita=1;		// DEFINE O SENTIDO DE ROTACAO DA RODA DIREITA - MUDA NA EXECUCAO DO SOFTWARE
bit flag_nova_configuracao_do_tempo=1;
char comando[8];				// RECEBE O COMANDO PELA PORTA SERIAL PELO HIPER-TERMINAL...
								// PROVAMENTE SERA MODIFICADO PARA UM VETOR DE CHAR QUANDO DE DESEJAR 
								// OBTER DADOS DO ROBO OU COMANDOS DE MOVIMENTACAO MAIS COMPLEXOS...

unsigned char i;				// INDICE AUXILIAR PARA LOOPS


bit UART = 0;

unsigned int encoder_right_count = 0;
unsigned int encoder_left_count = 0;


char putchar (char c)  {

   if (UART == 0) {

      if (c == '\n')  {                // check for newline character
         while (!flag_pode_enviar);    // wait until UART0 is ready to transmit
         flag_pode_enviar = 0;         // clear interrupt flag
         SBUF0 = 0x0d;                 // output carriage return command
      }
      while (!flag_pode_enviar);                    // wait until UART0 is ready to transmit
      flag_pode_enviar = 0;                         // clear interrupt flag
      return (SBUF0 = c);              // output <c> using UART 0
   }

   else if (UART == 1) {
      if (c == '\n')  {                // check for newline character
         while (!(SCON1 & 0x02));      // wait until UART1 is ready to transmit
         SCON1 &= ~0x02;               // clear TI1 interrupt flag
         SBUF1 = 0x0d;                 // output carriage return
      }
      while (!(SCON1 & 0x02));         // wait until UART1 is ready to transmit
      SCON1 &= ~0x02;                  // clear TI1 interrupt flag
      return (SBUF1 = c);              // output <c> using UART 1
   }
}

void pwm_setup(){
    // 0000 0011
	TMOD = (TMOD | 0x03);	// Programa o Timer0 para para Operacao em modo 3, como 2 timers de 8
							// bits cada. [ TCOM.M0=1 ] [ TCOM.M1=1 ]
							// operando na forma de Timer ( Counter(1) / Timer(0) = 0) [ TCOM.C/T ]
							// controlados por software  ( GATE=0 )
	ET0 = 1;				// Enable or disable the Timer 0 overflow interrupt.
	ET1 = 0;				// Disable the Timer 1 overflow interrupt. Agora controlando o timer0 "TL0"
	TR0 = 1;				// Ativa Timer 0 No registro controlador do Timer
	TR1 = 0;				// Desativa Timer 1 No registro controlador do Timer.Agora controlando o timer0 "TL0"
	EA = 1;                 // Ativa Todas as Interrupcoes
}


void Delay(void)
{
   int x;
   for(x = 0;x < 500;x)
      x++;
}

void SYSTEMCLOCK_Init (void)			//Retirado do exemplo de Comunica??o UART do Kit da SLI LABS
										//C8051F340
{
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency and enable
                                       // missing clock detector

   CLKMUL  = 0x00;                     // Select internal oscillator as
                                       // input to clock multiplier

   CLKMUL |= 0x80;                     // Enable clock multiplier
   Delay();                            // Delay for clock multiplier to begin
   CLKMUL |= 0xC0;                     // Initialize the clock multiplier
   Delay();                            // Delay for clock multiplier to begin

   while(!(CLKMUL & 0x20));            // Wait for multiplier to lock
   CLKSEL  = 0x03;                     // Select system clock
}

void PORT_Init (void)					//Retirado do exemplo de comunica??o UART do Kit
{  
   XBR0 = 0x01;                        // route UART 0 to crossbar
   XBR2 = 0x01;							// route UART 1 to crossbar
   XBR1 = 0x40;							// enable crossbar
   P0MDIN  = 0xFF;						// Port P0 = entrada digital
   P0MDOUT = 0x00;                     // Port P0 = saida OpenDrain
   P1MDOUT = 0x00; 					   // Mantem todas os pinos do Port 1 em OpenDrain (0) ou PushPull (1)
   P1MDIN  = 0xFF;					   // Mantem todas os pinos do Port 1 como entradas Digitais
   P2MDOUT |= 0x04;                    // set LED to push-pull
}

void serial_setup(){

   // 0001 0000
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits

	// UartBaudRate =  T1 CLK / ( 256 - T1H ) * 0.5

   if (SYSCLK/BAUDRATE0/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE0/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE0/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE0/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01                 
      CKCON |=  0x09;
   } else if (SYSCLK/BAUDRATE0/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE0/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE0/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;      // init Timer1
   // 0000 1111 - apaga os 4 primeiros bits
   TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit autoreload
   // seta 0010 0000
   TMOD |=  0x20;                       
   TR1 = 1;        // START Timer1
   TI0 = 1;        // Indicate TX0 ready
   ES0 = 1;
}

void encoder_setup(){	
	IT01CF = 0x31;	// INT0 e INT1 ativo alto, P0.1 = INT0, P0.3 = INT1
	IT0 = 0;		// Edge sensitive INT0
	IT1 = 0;		// Edge sensitive INT1
	IE0 = 0;		// Apaga flag interrupcao INT0
	IE1 = 0;		// Apaga flag interrupcao INT1	
	EX0 = 1;		// Habilita INT0 (interrupcao externa)
	EX1 = 1;		// Habilita INT1 (interrupcao externa)

}
		
void ativa_direita(void){
	if (sentido_roda_direita) {		// Avalia sentido da roda direita (em frente == 1)
		PWM_RIGHT_REVERSE = OFF;	// Desliga o PWM para tras
		PWM_RIGHT_FORWARD = ON;		// Liga o PWM para frente
	}
	else {
		PWM_RIGHT_FORWARD = OFF;	// Desliga o PWM para frente
		PWM_RIGHT_REVERSE = ON;		// Liga o PWM para tras
	}
}

void desativa_direita(void){
	PWM_RIGHT_REVERSE = OFF;		// Desliga o PWM para tras
	PWM_RIGHT_FORWARD = OFF;		// Desliga o PWM para frente
}

void ativa_esquerda(void){
	if (sentido_roda_esquerda) {    // Avalia sentido da roda direita (em frente == 1)
		PWM_LEFT_REVERSE = OFF;		// Desliga o PWM para tras
		PWM_LEFT_FORWARD = ON;		// Liga o PWM para frente
	}
	else {
		PWM_LEFT_FORWARD = OFF;		// Desliga o PWM para frente
		PWM_LEFT_REVERSE = ON;		// Liga o PWM para tras
	}
}

void desativa_esquerda(void){
	PWM_LEFT_REVERSE = OFF;		// Desliga o PWM para tras
	PWM_LEFT_FORWARD = OFF;		// Desliga o PWM para frente
}

// Versao Nova - Usado no Micro 8051F340 (48Mhz)
void timer0_IT() interrupt 1 {
	static unsigned int pwm_aux=TMAX;
	static unsigned int pwm_left_temp=0;
	static unsigned int pwm_right_temp=0;

	TR0 = OFF;							// Desliga o Timer 0 - Obrigatorio, pois precisamos 
										// setar os valores do timer a cada vez que este este 
										// estra na rotina de tratamento da interrupcao,ora com 
										// valores maiores ora com valores menores.

	pwm_aux++;  // Incrementa contador de overflows
	if ( pwm_aux == TMAX ) {
		// Fim de Ciclo de Geracao da PWM ( 251 Hz )
		pwm_left_temp=pwm_left;   // Recarega com valores de pwm
		pwm_right_temp=pwm_right; // Recarega com valores de pwm
		pwm_aux=0;				  // Reseta contador
	}
	else {
		if (pwm_left_temp>0) { 
			ativa_esquerda(); 
		} 
		else { 
			desativa_esquerda();
		}
		if (pwm_right_temp>0) {
			ativa_direita();
		}
		else { 
			desativa_direita();
		}
		if (pwm_left_temp >0) pwm_left_temp--; 
		if (pwm_right_temp>0) pwm_right_temp--; 
	}

	TL0=0;      // Zera Timer
	TF0 = 0;	// Limpa Flag de Interrupcao do Timer 0    
	TR0 = 1;	// Ligar o Timer 0
}

void ADC_setup () { 	// USAR TIMER 3
	REF0CN = 0x08;		// Seleciona VDD como tensao de referencia.
	AMX0N = TERRA;		// Define a entrada negativa do ADC0 como GND.
	ADC0CF = 11<<3; 	// Configura o CLOCK do passo da conversao como 4.8MHZ
	ADC0CF |= 0x04;		// Configura o ADC0 para ignorar os 2 bits menos
						// significativos da conversao
	AMX0P = CANAL_0;	// Seleciona a entrada da conversao como o canal 0 (P2.0)
    TMR3CN &= ~0x01;
	CKCON &= 0x3F;
	TMR3L = 0xFF;		// Configura o valor maximo da contagem como 255
	TMR3RLL = 0;
	EIE1 |= 0x88;
	TMR3CN = 0x28;		// Habilita interrupcao do timer 3 pelo low byte.
	ADC0CN = 0x80;		// Seleciona inicio de conversao no set do bit ADCBUSY.
}

void interrupt_timer3() interrupt 14 {	// Trata a interrupcao do Timer 3

	// 784 - 50 ms
	// 3136 - 200 ms
	if(timer3count == 3136 * 5) { // Contar para gerar um delay de 200ms
		AD0BUSY =1;				  // Iniciar as conversoes no ADC0.
		timer3count = 0;
	}
	else timer3count++;
	TMR3CN &= ~0x40;
}

void interrupt_ADC() interrupt 10 { // Tratamento da interrupcao do ADC0
	switch(sensorCount){
		case 0:
			AMX0P =  CANAL_1;	// Seleciona a entrada da conversao como o canal 1 (P2.1)
		break;
		case 1:
			AMX0P =  CANAL_2;	// Seleciona a entrada da conversao como o canal 2 (P2.2)
		break;
		case 2:
			AMX0P =  CANAL_3;	// Seleciona a entrada da conversao como o canal 3 (P2.3)
		break;
		case 3:
			AMX0P =  CANAL_4;	// Seleciona a entrada da conversao como o canal 4 (P2.5)
		break;
		case 4:
			AMX0P =  CANAL_5;	// Seleciona a entrada da conversao como o canal 5 (P2.6)
		break;
		case 5:
			AMX0P =  CANAL_0;	// Seleciona a entrada da conversao como o canal 0 (P2.0)
			flag_nova_conversao = 1;
		break;
	}
	valores_novos[sensorCount] = ADC0H;
	if(sensorCount == 5) { 
		sensorCount = 0xFF; // Sinaliza que nao havera mais conversoes
	}
	sensorCount++;
	if(sensorCount != 0) AD0BUSY = 1; // Dispara a proxima conversao.
	AD0INT = 0;
}

void enviar_distancias() {

	unsigned char comando_send[4];
	unsigned char indice;
	unsigned char indice2;
	for(indice = 0; indice < 6; indice++){
		if ( valores_velhos[indice] != valores_novos[indice] ) {
			comando_send[0] = SENSOR_OPTICO_0 + indice;
			comando_send[1] = valores_novos[indice] + (valores_novos[indice] == 0);
			comando_send[2] = FIM_COMANDO;
			comando_send[3] = '\n';
			for(indice2 = 0; indice2 < 4 ; indice2++) {		
				while(!flag_pode_enviar);
				flag_pode_enviar = 0;
				SBUF0 = comando_send[indice2];
			}
		}
	}
}

void enviar_info_encoders(unsigned char arg) {

	unsigned char comando_send[4];
	unsigned char indice;
	comando_send[0] = ENCODER + arg;
	comando_send[1] = FIM_COMANDO;
	comando_send[2] = '\n';
	for(indice = 0; indice < 3 ; indice++) {		
		while(!flag_pode_enviar);
		flag_pode_enviar = 0;
		SBUF0 = comando_send[indice];
	}
}

void interrupt_encoder_right() interrupt 0 {
	//TODO
	//PORT0.1
	encoder_right_count++;
	IE0 = 0;
	if (encoder_right_count > 180) 
	{
		encoder_right_count = 0;
		enviar_info_encoders(0);
	}
}

void interrupt_encoder_left() interrupt 2 {
	//TODO
	//PORT0.3
	encoder_left_count++;
	IE1 = 0;
	if (encoder_left_count > 180)
	{
		encoder_left_count = 0;
		enviar_info_encoders(1);
	}
}

void interrupt_serial() interrupt 4 { // Tratamento da interrupcao da UART0
	static short int indice = 0;
	if (RI0 == 1){					
		comando[indice] = SBUF0;
		if(comando[indice] == '\n' || indice ==5) { // Limitando comando em 6 caracteres
			comando[indice] = '\0';
			indice = 0;
			flag_novo_comando = 1;
		}	
		else indice++;
		RI0=0;
	}

	if(TI0 == 1) {
		TI0 = 0;
		flag_pode_enviar = 1;
	}

}
void main (void)  {     /* main program */

    PCA0MD &= ~0x40;    // Disable Watchdog timer

	SYSTEMCLOCK_Init(); // Inicializa??o do Clock da Serial
	PORT_Init();		

	serial_setup();		// Configura a comunicacao serial
	pwm_setup();		// Configura o PWM
	encoder_setup();	// Configura o encoder

	ADC_setup();		// Configura o ADC0
	EA = 1;				// Habilita todas as interrupcoes

//	pwm_left  = 35; 	// hardcode - teste da PWM
//	pwm_right = 35; 	// hardcode - teste da PWM
	sentido_roda_esquerda = 0;
	sentido_roda_direita = 0;

  while (1)  { // Loop infinito....
	if(flag_nova_conversao){	// Novos valores de conversao disponiveis
		//enviar_distancias();
		flag_nova_conversao = 0;
		for (i=0; i<6; i++) {
			valores_velhos[i] = valores_novos[i]; // Armazena valores velhos
		}
	}

	if(flag_novo_comando){

#ifdef DEBUG
	    printf("Comando Recebido : %c %c %c %c -",comando[0],comando[1],comando[2],comando[3]);
#endif
		flag_novo_comando = 0;
		switch (comando[0]) {
			case RODA_LEFT:
				#ifdef DEBUG
					printf("RODA_LEFT\n");
				#endif
				sentido_roda_esquerda = (comando[1] & MASCARA_SENTIDO);
				pwm_left = (comando[1] & MASCARA_PWM) * PASSO_DE_VELOCIDADE;
				break;
			case RODA_RIGHT:
				#ifdef DEBUG
					printf("RODA_RIGHT\n");
				#endif
				sentido_roda_direita=(comando[1] & MASCARA_SENTIDO);
				pwm_right = (comando[1] & MASCARA_PWM) * PASSO_DE_VELOCIDADE;
				break;
		
			case STOP: 
				pwm_right=pwm_left=0;
				#ifdef DEBUG
					printf("STOP ! \n");
				#endif
				break;
		
			default:
				#ifdef DEBUG
					printf("Comando Invalido !");
				#endif
			break;
		} 
	}
 }
}


