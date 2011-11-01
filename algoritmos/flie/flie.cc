// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996

// File flie.cc



#include <stdlib.h> // Random!

#include "flie.h"



int main()

{

category cat[12];

linguisticvariable SensorEsquerda,SensorFrente,SensorDireita, VelMotor;

rule infrule[2];

fuzzy_control fc,fc2;

float ValorEsquerda,ValorFrente,ValorDireita, PercPwm;

//Frente
cat[0].setname("Perto");
cat[0].setval(0,15,25,50);
cat[1].setname("Medio");
cat[1].setval(25,50,100,150);
cat[2].setname("Longe");
cat[2].setval(100,150,175,200);
//Laterais
cat[3].setname("Perto Tras");
cat[3].setval(0,-15,-25,-50);
cat[4].setname("Medio Tras");
cat[4].setval(-25,-50,-100,-150);
cat[5].setname("Longe Tras");
cat[5].setval(-100,-150,-175,-200);
cat[6].setname("Perto Frente");
cat[6].setval(0,15,25,50);
cat[7].setname("Medio Frente");
cat[7].setval(25,50,100,150);
cat[8].setname("Longe Frente");
cat[8].setval(100,150,175,200);
//Motor Velocidade
cat[10].setname("Rapido");
cat[10].setval(10,90,99,100);
cat[11].setname("Lento");
cat[11].setval(0,1,10,90);
//Motor Velocidade


SensorEsquerda.setname("SensorEsquerda");

SensorEsquerda.includecategory(&cat[3]);
SensorEsquerda.includecategory(&cat[4]);
SensorEsquerda.includecategory(&cat[5]);
SensorEsquerda.includecategory(&cat[6]);
SensorEsquerda.includecategory(&cat[7]);
SensorEsquerda.includecategory(&cat[8]);

SensorEsquerda.setname("SensorDireita");

SensorEsquerda.includecategory(&cat[3]);
SensorEsquerda.includecategory(&cat[4]);
SensorEsquerda.includecategory(&cat[5]);
SensorEsquerda.includecategory(&cat[6]);
SensorEsquerda.includecategory(&cat[7]);
SensorEsquerda.includecategory(&cat[8]);

SensorEsquerda.setname("SensorFrente");

SensorEsquerda.includecategory(&cat[0]);
SensorEsquerda.includecategory(&cat[1]);
SensorEsquerda.includecategory(&cat[2]);

VelMotor.setname("Velocidade Motor");

SensorEsquerda.includecategory(&cat[10]);
SensorEsquerda.includecategory(&cat[11]);

fc.set_defuzz(AVERAGEOFMAX);
fc.definevars(SensorEsquerda,SensorFrente,SensorDireita,VelMotor);
fc.insert_rule("Perto F","Perto","Perto F","Lento");

ValorEsquerda = 25;
ValorFrente = 25;
ValorDireita = 25;

PercPwm = fc.make_inference(ValorEsquerda, ValorFrente, ValorDireita);
//fc.insert_rule("Esquerda","VireDireita");

//fc.insert_rule("Direita","VireEsquerda");

//ValorMotor = fc.make_inference(ValorDistancia);

return 0;
}

