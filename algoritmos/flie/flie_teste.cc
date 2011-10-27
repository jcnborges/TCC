// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996

// File flie.cc



#include <stdlib.h> // Random!

#include "flie.h"



int main()

{

category cat[4];

linguisticvariable SensorDistancia, AcaoMotor;

rule infrule[2];

fuzzy_control fc;

float ValorDistancia, ValorMotor;


cat[0].setname("Esquerda");

cat[0].setval(-45,-15,15);

cat[1].setname("Direita");

cat[1].setval(-15,15,45);

cat[2].setname("VireEsquerda");

cat[2].setval(-45,-15,15);

cat[3].setname("VireDireita");

cat[3].setval(-15,15,45);


SensorDistancia.setname("SensorDistancia");

SensorDistancia.includecategory(&cat[0]);

SensorDistancia.includecategory(&cat[1]);

AcaoMotor.setname("AcaoMotor");

AcaoMotor.includecategory(&cat[2]);

AcaoMotor.includecategory(&cat[3]);

fc.set_defuzz(AVERAGEOFMAX);

fc.definevars(SensorDistancia,AcaoMotor);

fc.insert_rule("Esquerda","VireDireita");

fc.insert_rule("Direita","VireEsquerda");

ValorDistancia = 7;

ValorMotor = fc.make_inference(ValorDistancia);

cout << "\n" << "First Defuzzified Output =" << ValorMotor<<"\n\n\n";

return 0;
}

