// Projeto FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro
// Adaptação/Utilização para o Trabalho de Conclusão de Curso - Algoritmos de Navegação Fuzzy: Uma Análise Qualitativa

//#define DOS
//#define DBG
#include <stdio.h>
#include <stdlib.h>

#ifdef DOS
#include <dir.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#endif

#include "flie.h"
#include "motor.h"
#include "FIE.h"

void flie::flie_setup()
{
	//Configuraçoes das funções de pertinência
	//Minimo e máximo para as entradas (sensores)
	float rlow = 0;
	float rhigh = 200;
	//Definição dos Trapézios para fuzzificação das leituras dos sensores
	float range_perto[4] = {16,16,40,75};
	float range_medio[4] = {40,75,100,150};
	float range_longe[4] = {100,150,175,200};
	//Definição das Velocidades para Defuzzificação
	float range_vellow = 0;
	float range_velhigh = 100;
	float lento = 33;
	float medio = 67;
	float rapido = 100;
	//Frente
	cat[0].setname("Perto");
	cat[0].setrange(rlow,rhigh);
	cat[0].setval(range_perto[0],range_perto[1],range_perto[2],range_perto[3]);
	cat[1].setname("Medio");
	cat[1].setrange(rlow,rhigh);
	cat[1].setval(range_medio[0],range_medio[1],range_medio[2],range_medio[3]);
	cat[2].setname("Longe");
	cat[2].setrange(rlow,rhigh);
	cat[2].setval(range_longe[0],range_longe[1],range_longe[2],range_longe[3]);
	//Lateral Esquerda (2 sensores)
	cat[3].setname("PertoEsquerda");
	cat[3].setrange(rlow,rhigh);
	cat[3].setval(range_perto[0],range_perto[1],range_perto[2],range_perto[3]);
	cat[4].setname("MedioEsquerda");
	cat[4].setrange(rlow,rhigh);
	cat[4].setval(range_medio[0],range_medio[1],range_medio[2],range_medio[3]);
	cat[5].setname("LongeEsquerda");
	cat[5].setrange(rlow,rhigh);
	cat[5].setval(range_longe[0],range_longe[1],range_longe[2],range_longe[3]);
	//Lateral Direita (2 sensores)
	cat[6].setname("PertoDireita");
	cat[6].setrange(rlow,rhigh);
	cat[6].setval(range_perto[0],range_perto[1],range_perto[2],range_perto[3]);
	cat[7].setname("MedioDireita");
	cat[7].setrange(rlow,rhigh);
	cat[7].setval(range_medio[0],range_medio[1],range_medio[2],range_medio[3]);
	cat[8].setname("LongeDireita");
	cat[8].setrange(rlow,rhigh);
	cat[8].setval(range_longe[0],range_longe[1],range_longe[2],range_longe[3]);
	//Motor Velocidade
	cat[9].setname("Rapido");
	cat[9].setrange(0,100);
	cat[9].setval(medio,rapido,rapido,rapido);
	cat[10].setname("Medio");
	cat[10].setrange(0,100);
	cat[10].setval(lento,medio,medio,rapido);
	cat[11].setname("Lento");
	cat[11].setrange(0,100);
	cat[11].setval(0,0,lento,medio);
	//Motor Angulo
	cat[12].setname("ViraEsquerda");
	cat[12].setrange(0,180);
	cat[12].setval(0,0,0,45);
	cat[13].setname("ViraPoucoEsquerda");
	cat[13].setrange(0, 180);
	cat[13].setval(0, 45, 45, 90);
	cat[14].setname("Reto");
	cat[14].setrange(0,180);
	cat[14].setval(45, 90, 90, 135);
	cat[15].setname("ViraPoucoDireita");
	cat[15].setrange(0, 180);
	cat[15].setval(90, 135, 135, 180);
	cat[16].setname("ViraDireita");
	cat[16].setrange(0,180);
	cat[16].setval(135,180,180,180);

	SensorFrente.setname("SensorFrente");

	SensorFrente.includecategory(&cat[0]);
	cat[0].define_lingvar(&SensorFrente);
	SensorFrente.includecategory(&cat[1]);
	cat[1].define_lingvar(&SensorFrente);
	SensorFrente.includecategory(&cat[2]);
	cat[2].define_lingvar(&SensorFrente);

	SensorEsquerda.setname("SensorEsquerda");

	SensorEsquerda.includecategory(&cat[3]);
	cat[3].define_lingvar(&SensorEsquerda);
	SensorEsquerda.includecategory(&cat[4]);
	cat[4].define_lingvar(&SensorEsquerda);
	SensorEsquerda.includecategory(&cat[5]);
	cat[5].define_lingvar(&SensorEsquerda);

	SensorDireita.setname("SensorDireita");
	SensorDireita.includecategory(&cat[6]);
	cat[6].define_lingvar(&SensorDireita);
	SensorDireita.includecategory(&cat[7]);
	cat[7].define_lingvar(&SensorDireita);
	SensorDireita.includecategory(&cat[8]);
	cat[8].define_lingvar(&SensorDireita);

	VelMotor.setname("Velocidade Motor");
	VelMotor.includecategory(&cat[9]);
	cat[9].define_lingvar(&VelMotor);
	VelMotor.includecategory(&cat[10]);
	cat[10].define_lingvar(&VelMotor);
	VelMotor.includecategory(&cat[11]);
	cat[11].define_lingvar(&VelMotor);

	AngMotor.setname("Angulo Motor");
	AngMotor.includecategory(&cat[12]);
	cat[12].define_lingvar(&AngMotor);
	AngMotor.includecategory(&cat[13]);
	cat[13].define_lingvar(&AngMotor);
	AngMotor.includecategory(&cat[14]);
	cat[14].define_lingvar(&AngMotor);
	AngMotor.includecategory(&cat[15]);
	cat[15].define_lingvar(&AngMotor);
	AngMotor.includecategory(&cat[16]);
	cat[16].define_lingvar(&AngMotor);


	fc.set_defuzz(AVERAGEOFMAX);
	fc.definevars(SensorEsquerda,SensorFrente,SensorDireita,VelMotor);

	fc2.set_defuzz(AVERAGEOFMAX);
	fc2.definevars(SensorEsquerda,SensorFrente,SensorDireita,AngMotor);

	char lingesquerda[64];
	char lingfrente[64];
	char lingdireita[64];
	char lingvel[64];
	char lingdirecao[64];
	FILE *regras = freopen("regras.txt","r",stdin);

	for(int i = 0; scanf("%s%s%s%s%s", lingesquerda, lingfrente, lingdireita, lingvel, lingdirecao) == 5; ++i){
		insertRule(i,lingesquerda,lingfrente,lingdireita,lingvel,lingdirecao);
#ifdef DBG
		printf("Regra %d [%s, %s, %s, %s, %s] inserida\n", i, lingesquerda, lingfrente, lingdireita,
				lingvel, lingdirecao);
#endif
	}

}

void flie::insertRule(int n,char *se, char *sf, char *sd, char *vm, char *d){
	fc.insert_rule(se,sf,sd,vm);
	infrule[n].inserePremissa(SensorEsquerda.getcatptr(se));
	infrule[n].inserePremissa(SensorFrente.getcatptr(sf));
	infrule[n].inserePremissa(SensorDireita.getcatptr(sd));
	infrule[n].insereConclusao(VelMotor.getcatptr(vm));
	fc2.insert_rule(se,sf,sd,d);
	infrule2[n].inserePremissa(SensorEsquerda.getcatptr(se));
	infrule2[n].inserePremissa(SensorFrente.getcatptr(sf));
	infrule2[n].inserePremissa(SensorDireita.getcatptr(sd));
	infrule2[n].insereConclusao(AngMotor.getcatptr(d));
}

void flie::inference(float se, float sf, float sd, float &vel, float &ang){
	vel = fc.make_inference(se,sf,sd);
	ang = fc2.make_inference(se,sf,sd);
}
