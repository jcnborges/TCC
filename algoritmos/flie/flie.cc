// Projeto FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro
// Adaptação/Utilização para o Trabalho de Conclusão de Curso Bellator

//#define DOS
//#define DBG
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

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
	float rlow = 0;
	float rhigh = 200;
	float range_perto[4] = {16,16,20,50};
	float range_medio[4] = {20,50,100,150};
	float range_longe[4] = {100,150,175,200};
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
	cat[3].setname("PertoTras");
	cat[3].setrange(rlow,rhigh+200);
	cat[3].setval(range_perto[0]+200,range_perto[1]+200,range_perto[2]+200,range_perto[3]+200);
	cat[4].setname("MedioTras");
	cat[4].setrange(rlow,rhigh+200);
	cat[4].setval(range_medio[0]+200,range_medio[1]+200,range_medio[2]+200,range_medio[3]+200);
	cat[5].setname("LongeTras");
	cat[5].setrange(rlow,rhigh+200);
	cat[5].setval(range_longe[0]+200,range_longe[1]+200,range_longe[2]+200,range_longe[3]+200);
	cat[6].setname("PertoFrente");
	cat[6].setrange(rlow,rhigh+200);
	cat[6].setval(range_perto[0],range_perto[1],range_perto[2],range_perto[3]);
	cat[7].setname("MedioFrente");
	cat[7].setrange(rlow,rhigh+200);
	cat[7].setval(range_medio[0],range_medio[1],range_medio[2],range_medio[3]);
	cat[8].setname("LongeFrente");
	cat[8].setrange(rlow,rhigh+200);
	cat[8].setval(range_longe[0],range_longe[1],range_longe[2],range_longe[3]);
	//Lateral Direita (2 sensores)
	cat[9].setname("PertoTras");
	cat[9].setrange(rlow,rhigh+200);
	cat[9].setval(range_perto[0]+200,range_perto[1]+200,range_perto[2]+200,range_perto[3]+200);
	cat[10].setname("MedioTras");
	cat[10].setrange(rlow,rhigh+200);
	cat[10].setval(range_medio[0]+200,range_medio[1]+200,range_medio[2]+200,range_medio[3]+200);
	cat[11].setname("LongeTras");
	cat[11].setrange(rlow,rhigh+200);
	cat[11].setval(range_longe[0]+200,range_longe[1]+200,range_longe[2]+200,range_longe[3]+200);
	cat[12].setname("PertoFrente");
	cat[12].setrange(rlow,rhigh+200);
	cat[12].setval(range_perto[0],range_perto[1],range_perto[2],range_perto[3]);
	cat[13].setname("MedioFrente");
	cat[13].setrange(rlow,rhigh+200);
	cat[13].setval(range_medio[0],range_medio[1],range_medio[2],range_medio[3]);
	cat[14].setname("LongeFrente");
	cat[14].setrange(rlow,rhigh+200);
	cat[14].setval(range_longe[0],range_longe[1],range_longe[2],range_longe[3]);
	//Motor Velocidade
	cat[15].setname("Rapido");
	cat[15].setrange(0,100);
	cat[15].setval(50,100,100,100);
	cat[16].setname("Medio");
	cat[16].setrange(0,100);
	cat[16].setval(10,50,50,100);
	cat[17].setname("Lento");
	cat[17].setrange(0,100);
	cat[17].setval(0,0,10,50);
	//Motor Angulo
	cat[18].setname("ViraEsquerda");
	cat[18].setrange(0,180);
	cat[18].setval(0,0,0,90);
	cat[19].setname("Reto");
	cat[19].setrange(0,180);
	cat[19].setval(0,90,90,180);
	cat[20].setname("ViraDireita");
	cat[20].setrange(0,180);
	cat[20].setval(90,180,180,180);

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
	SensorEsquerda.includecategory(&cat[6]);
	cat[6].define_lingvar(&SensorEsquerda);
	SensorEsquerda.includecategory(&cat[7]);
	cat[7].define_lingvar(&SensorEsquerda);
	SensorEsquerda.includecategory(&cat[8]);
	cat[8].define_lingvar(&SensorEsquerda);

	SensorDireita.setname("SensorDireita");
	SensorDireita.includecategory(&cat[9]);
	cat[9].define_lingvar(&SensorDireita);
	SensorDireita.includecategory(&cat[10]);
	cat[10].define_lingvar(&SensorDireita);
	SensorDireita.includecategory(&cat[11]);
	cat[11].define_lingvar(&SensorDireita);
	SensorDireita.includecategory(&cat[12]);
	cat[12].define_lingvar(&SensorDireita);
	SensorDireita.includecategory(&cat[13]);
	cat[13].define_lingvar(&SensorDireita);
	SensorDireita.includecategory(&cat[14]);
	cat[14].define_lingvar(&SensorDireita);

	VelMotor.setname("Velocidade Motor");
	VelMotor.includecategory(&cat[15]);
	cat[15].define_lingvar(&VelMotor);
	VelMotor.includecategory(&cat[16]);
	cat[16].define_lingvar(&VelMotor);
	VelMotor.includecategory(&cat[17]);
	cat[17].define_lingvar(&VelMotor);

	AngMotor.setname("Angulo Motor");
	AngMotor.includecategory(&cat[18]);
	cat[18].define_lingvar(&AngMotor);
	AngMotor.includecategory(&cat[19]);
	cat[19].define_lingvar(&AngMotor);
	AngMotor.includecategory(&cat[20]);
	cat[20].define_lingvar(&AngMotor);


	fc.set_defuzz(AVERAGEOFMAX);
	fc.definevars(SensorEsquerda,SensorFrente,SensorDireita,VelMotor);

	fc2.set_defuzz(AVERAGEOFMAX);
	fc2.definevars(SensorEsquerda,SensorFrente,SensorDireita,AngMotor);

	char lingesquerda[16];
	char lingfrente[16];
	char lingdireita[16];
	char lingvel[16];
	char lingdirecao[16];
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
