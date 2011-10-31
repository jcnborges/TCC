// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996

// File flie.cc



#include <stdlib.h> // Random!

#include "flie.h"



int main()

{

category cat[4];

linguisticvariable SensorEsquerda,SensorFrente,SensorDireita, AcaoMotor;

rule infrule[2];

fuzzy_control fc;

float ValorEsquerda,ValorFrente,ValorDireita, ValorMotor;


cat[0].setname("MuitoLonge");

cat[0].setval(140,150,160,170);

SensorEsquerda.setname("SensorEsquerda");

SensorEsquerda.includecategory(&cat[0]);

SensorEsquerda.setname("SensorDireita");

SensorEsquerda.includecategory(&cat[0]);

SensorEsquerda.setname("SensorFrente");

SensorEsquerda.includecategory(&cat[0]);

AcaoMotor.setname("AcaoMotor");

fc.set_defuzz(AVERAGEOFMAX);

fc.definevars(SensorEsquerda,SensorFrente,SensorDireita,AcaoMotor);

//fc.insert_rule("Esquerda","VireDireita");

//fc.insert_rule("Direita","VireEsquerda");

//ValorMotor = fc.make_inference(ValorDistancia);

return 0;
}

