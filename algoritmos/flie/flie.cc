// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996

//#define DOS
// File flie.cc
#include <stdio.h> // Random!
#include <stdlib.h> // Random!
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

/* DEFINES ANTIGOS, apagar depois
// In the main program, we have to instantiate the variables to contain
// all the fuzzy sets, and define them!
fuzzy_control fc, fc1, fc2; // Only global Variable!!!! Used in FlieGA();

//trapezoid_category
subat cat[23];

// And, for purpose of ease of use, we define linguistic variables to
//put the set into!

linguisticvariable Distance, Speed, Direction, Angle, AngletoTarget;

// We also have to define the rules of inference that will define the
// behavior of the control system! We also have to instantiate them!

rule infrule[49];
rule infrule1[49];
rule infrule2[49];

// And last, we define one control system that will behave by that rules!
//fuzzy_control fc;

// And some variables that will contain the inputs and outputs(defuzzified) of
// the system under control!
*/

subat cat[21];

linguisticvariable SensorEsquerda,SensorFrente,SensorDireita, VelMotor;

rule infrule[49];
rule infrule2[49];

fuzzy_control fc,fc2;

float ValorEsquerda,ValorFrente,ValorDireita, PercPwm;

void insertRule(int, char *, char *, char *, char*);

int main()//_FIE()
{

/*CODIGO ANTIGO, apagar depois
    float x =0;
    int i=0;
    int j=0;
    int k=0;

#ifdef DOS
    clrscr();
#endif


float SumError, ControlOutput;

// Define fuzzy sets for the linguistic variable Distance

cat[0].setname("VeryNear");
cat[0].setrange(0,50);
cat[0].setval(0,0, 0,5);
cat[1].setname("Near");
cat[1].setrange(0,50);
cat[1].setval(0,5,5,10);

cat[2].setname("Far");
cat[2].setrange(0,50);
cat[2].setval(5,10,20);

cat[3].setname("VeryFar");
cat[3].setrange(0,50);
cat[3].setval(10,20,50,50);

// Define the linguistic variable

Distance.setname("Distance");

Distance.includecategory(&cat[0]);
cat[0].define_lingvar(&Distance);
Distance.includecategory(&cat[1]);
cat[1].define_lingvar(&Distance);
Distance.includecategory(&cat[2]);
cat[2].define_lingvar(&Distance);
Distance.includecategory(&cat[3]);
cat[3].define_lingvar(&Distance);


cat[4].setname("VerySlow");
cat[4].setrange(0,20);
cat[4].setval(0,0,2.5,5);

cat[5].setname("Slow");
cat[5].setrange(0,20);
cat[5].setval(0,2.5,5,7.5);

cat[6].setname("Medium");
cat[6].setrange(0,20);
cat[6].setval(2.5, 10, 10, 20);

cat[7].setname("Maximum");
cat[7].setrange(0,20);
cat[7].setval(10,20,20,20);

// Define Linguistic Variable Speed

Speed.setname("Speed");

Speed.includecategory(&cat[4]);
cat[4].define_lingvar(&Speed);
Speed.includecategory(&cat[5]);
cat[5].define_lingvar(&Speed);
Speed.includecategory(&cat[6]);
cat[6].define_lingvar(&Speed);
Speed.includecategory(&cat[7]);
cat[7].define_lingvar(&Speed);


// Define the fuzzy_control(by the fuzzy inputs (Distance) and
// output (Speed)
fc.definevars(Distance, Speed);

// Include each fuzzy rule in the fuzzy_control!(linguisticly!)

fc.insert_rule("VeryFar","Maximum");
infrule[0].inserePremissa(Distance.getcatptr("VeryFar"));
//infrule[0].inserePremissa(DeltaError.getcatptr("NB"));
infrule[0].insereConclusao(Speed.getcatptr("Maximum"));

fc.insert_rule("Far","Medium");
infrule[1].inserePremissa(Distance.getcatptr("Far"));
infrule[1].insereConclusao(Speed.getcatptr("Medium"));

fc.insert_rule("Near","Slow");
infrule[2].inserePremissa(Distance.getcatptr("Near"));
infrule[2].insereConclusao(Speed.getcatptr("Slow"));

fc.insert_rule("VeryNear","VerySlow");
infrule[3].inserePremissa(Distance.getcatptr("VeryNear"));
infrule[3].insereConclusao(Speed.getcatptr("VerySlow"));


// Define fuzzy sets for the linguistic variable Angle

cat[8].setname("VL");
cat[8].setrange(-90,0);
cat[8].setval(-90,-90,-45,-20);

cat[9].setname("LL");
cat[9].setrange(-90,0);
cat[9].setval(-45,-20,-20, 0);

cat[10].setname("FT");
cat[10].setrange(-90,+90);
cat[10].setval(-20,0,0, 20);

cat[11].setname("LR");
cat[11].setrange(0,+90);
cat[11].setval(0,20, 20, 45);

cat[12].setname("VR");
cat[12].setrange(0,+90);
cat[12].setval(20,45,90,90);

// Define the linguistic variable Angle
Angle.setname("Angle");

Angle.includecategory(&cat[8]);
cat[8].define_lingvar(&Angle);
Angle.includecategory(&cat[9]);
cat[9].define_lingvar(&Angle);
Angle.includecategory(&cat[10]);
cat[10].define_lingvar(&Angle);
Angle.includecategory(&cat[11]);
cat[11].define_lingvar(&Angle);
Angle.includecategory(&cat[12]);
cat[12].define_lingvar(&Angle);

// Define fuzzy sets for the linguistic variable  Direction

cat[13].setname("HL");
cat[13].setrange(-90,0);
cat[13].setval(-90, -90, -60, 0);

cat[14].setname("LL");
cat[14].setrange(-90,0);
cat[14].setval(-60,-20,-10,0);

cat[15].setname("FT");
cat[15].setrange(-90,90);
cat[15].setval(-10,0,10);

cat[16].setname("LR");
cat[16].setrange(0,90);
cat[16].setval(0,10,20, 60);

cat[17].setname("HR");
cat[17].setrange(0,90);
cat[17].setval(0,60,90, 90);


// Define the linguistic variable Direction
Direction.setname("Direction");

Direction.includecategory(&cat[13]);
cat[13].define_lingvar(&Direction);
Direction.includecategory(&cat[14]);
cat[14].define_lingvar(&Direction);
Direction.includecategory(&cat[15]);
cat[15].define_lingvar(&Direction);
Direction.includecategory(&cat[16]);
cat[16].define_lingvar(&Direction);
Direction.includecategory(&cat[17]);
cat[17].define_lingvar(&Direction);


//if obstacle is in at the right and close, turn to the left;
//if obstacle is a little to the left and far away, turn slowly to the right;

// Define the fuzzy_control(by the fuzzy inputs (Angle(VL, LL, FT, LR, VR))
// and Distance(VeryNear, Near, Far, VeryFar)) and
// output (Direction(HL, LL, FT, LR, HR))
fc1.definevars(Angle, Distance, Direction);

// Include each fuzzy rule in the fuzzy_control!(linguisticly!)

fc1.insert_rule("FT","VeryNear","HL");
infrule1[0].inserePremissa(Angle.getcatptr("FT"));
infrule1[0].inserePremissa(Distance.getcatptr("VeryNear"));
infrule1[0].insereConclusao(Direction.getcatptr("HL"));

fc1.insert_rule("FT","Near","LL");
infrule1[1].inserePremissa(Angle.getcatptr("FT"));
infrule1[1].inserePremissa(Distance.getcatptr("Near"));
infrule1[1].insereConclusao(Direction.getcatptr("LL"));

fc1.insert_rule("FT","Far","FT");
infrule1[2].inserePremissa(Angle.getcatptr("FT"));
infrule1[2].inserePremissa(Distance.getcatptr("Far"));
infrule1[2].insereConclusao(Direction.getcatptr("FT"));

fc1.insert_rule("FT","VeryFar","FT");
infrule1[3].inserePremissa(Angle.getcatptr("FT"));
infrule1[3].inserePremissa(Distance.getcatptr("VeryFar"));
infrule1[3].insereConclusao(Direction.getcatptr("FT"));


fc1.insert_rule("LR","VeryNear","HL");
infrule1[4].inserePremissa(Angle.getcatptr("LR"));
infrule1[4].inserePremissa(Distance.getcatptr("VeryNear"));
infrule1[4].insereConclusao(Direction.getcatptr("HL"));

fc1.insert_rule("LR","Near","LL");
infrule1[5].inserePremissa(Angle.getcatptr("LR"));
infrule1[5].inserePremissa(Distance.getcatptr("Near"));
infrule1[5].insereConclusao(Direction.getcatptr("LL"));

fc1.insert_rule("LR","Far","LL");
infrule1[6].inserePremissa(Angle.getcatptr("LR"));
infrule1[6].inserePremissa(Distance.getcatptr("Far"));
infrule1[6].insereConclusao(Direction.getcatptr("LL"));

fc1.insert_rule("LR","VeryFar","FT");
infrule1[7].inserePremissa(Angle.getcatptr("LR"));
infrule1[7].inserePremissa(Distance.getcatptr("VeryFar"));
infrule1[7].insereConclusao(Direction.getcatptr("FT"));


fc1.insert_rule("VR","VeryNear","HL");
infrule1[8].inserePremissa(Angle.getcatptr("VR"));
infrule1[8].inserePremissa(Distance.getcatptr("VeryNear"));
infrule1[8].insereConclusao(Direction.getcatptr("HL"));

fc1.insert_rule("VR","Near","HL");
infrule1[9].inserePremissa(Angle.getcatptr("VR"));
infrule1[9].inserePremissa(Distance.getcatptr("Near"));
infrule1[9].insereConclusao(Direction.getcatptr("HL"));

fc1.insert_rule("VR","Far","LL");
infrule1[10].inserePremissa(Angle.getcatptr("VR"));
infrule1[10].inserePremissa(Distance.getcatptr("Far"));
infrule1[10].insereConclusao(Direction.getcatptr("LL"));

fc1.insert_rule("VR","VeryFar","LL");
infrule1[11].inserePremissa(Angle.getcatptr("VR"));
infrule1[11].inserePremissa(Distance.getcatptr("VeryFar"));
infrule1[11].insereConclusao(Direction.getcatptr("LL"));


fc1.insert_rule("LL","VeryNear","HR");
infrule1[12].inserePremissa(Angle.getcatptr("LL"));
infrule1[12].inserePremissa(Distance.getcatptr("VeryNear"));
infrule1[12].insereConclusao(Direction.getcatptr("HR"));

fc1.insert_rule("LL","Near","LR");
infrule1[13].inserePremissa(Angle.getcatptr("LL"));
infrule1[13].inserePremissa(Distance.getcatptr("Near"));
infrule1[13].insereConclusao(Direction.getcatptr("LR"));

fc1.insert_rule("LL","Far","LR");
infrule1[14].inserePremissa(Angle.getcatptr("LL"));
infrule1[14].inserePremissa(Distance.getcatptr("Far"));
infrule1[14].insereConclusao(Direction.getcatptr("LR"));

fc1.insert_rule("LL","VeryFar","FT");
infrule1[15].inserePremissa(Angle.getcatptr("LL"));
infrule1[15].inserePremissa(Distance.getcatptr("VeryFar"));
infrule1[15].insereConclusao(Direction.getcatptr("FT"));

fc1.insert_rule("VL","VeryNear","HR");
infrule1[16].inserePremissa(Angle.getcatptr("VL"));
infrule1[16].inserePremissa(Distance.getcatptr("VeryNear"));
infrule1[16].insereConclusao(Direction.getcatptr("HR"));

fc1.insert_rule("VL","Near","HR");
infrule1[17].inserePremissa(Angle.getcatptr("VL"));
infrule1[17].inserePremissa(Distance.getcatptr("Near"));
infrule1[17].insereConclusao(Direction.getcatptr("HR"));

fc1.insert_rule("VL","Far","LR");
infrule1[18].inserePremissa(Angle.getcatptr("VL"));
infrule1[18].inserePremissa(Distance.getcatptr("Far"));
infrule1[18].insereConclusao(Direction.getcatptr("LR"));

fc1.insert_rule("VL","VeryFar","LR");
infrule1[19].inserePremissa(Angle.getcatptr("VL"));
infrule1[19].inserePremissa(Distance.getcatptr("VeryFar"));
infrule1[19].insereConclusao(Direction.getcatptr("LR"));

/*
fc1.insert_rule("LR","VeryNear","HL");
infrule1[4].inserePremissa(Angle.getcatptr("LR"));
infrule1[4].inserePremissa(Distance.getcatptr("VeryNear"));
infrule1[4].insereConclusao(Direction.getcatptr("HL"));

fc1.insert_rule("LL","VeryFar","HR");
infrule1[5].inserePremissa(Angle.getcatptr("LL"));
infrule1[5].inserePremissa(Distance.getcatptr("VeryFar"));
infrule1[5].insereConclusao(Direction.getcatptr("HR"));


// if the target is a lot to the left, turn hard to the left

// Define the fuzzy_control(by the fuzzy inputs (Angle(VL, LL, FT, LR, VR))
// output (Direction(HL, LL, FT, LR, HR))

// Define fuzzy sets for the linguistic variable  Direction

cat[18].setname("VL");
cat[18].setrange(-90,0);
cat[18].setval(-90, -90, -60, 0);

cat[19].setname("LL");
cat[19].setrange(-90,0);
cat[19].setval(-60,-20,-10,0);

cat[20].setname("FT");
cat[20].setrange(-90,90);
cat[20].setval(-10,0,10);

cat[21].setname("LR");
cat[21].setrange(0,90);
cat[21].setval(0,10,20, 60);

cat[22].setname("VR");
cat[22].setrange(0,90);
cat[22].setval(0,60,90,90);


// Define the linguistic variable Direction
AngletoTarget.setname("AngletoTarget");

AngletoTarget.includecategory(&cat[18]);
cat[18].define_lingvar(&AngletoTarget);
AngletoTarget.includecategory(&cat[19]);
cat[19].define_lingvar(&AngletoTarget);
AngletoTarget.includecategory(&cat[20]);
cat[20].define_lingvar(&AngletoTarget);
AngletoTarget.includecategory(&cat[21]);
cat[21].define_lingvar(&AngletoTarget);
AngletoTarget.includecategory(&cat[22]);
cat[22].define_lingvar(&AngletoTarget);


fc2.definevars(AngletoTarget, Direction);

fc2.insert_rule("VL","HR");
infrule2[0].inserePremissa(AngletoTarget.getcatptr("VL"));
//infrule2[0].inserePremissa(Distance.getcatptr("VeryNear"));
infrule2[0].insereConclusao(Direction.getcatptr("HR"));

fc2.insert_rule("LL","LR");
infrule2[1].inserePremissa(AngletoTarget.getcatptr("LL"));
//infrule2[1].inserePremissa(Distance.getcatptr("VeryNear"));
infrule2[1].insereConclusao(Direction.getcatptr("LR"));

fc2.insert_rule("FT","FT");
infrule2[2].inserePremissa(AngletoTarget.getcatptr("FT"));
//infrule2[2].inserePremissa(Distance.getcatptr("VeryNear"));
infrule2[2].insereConclusao(Direction.getcatptr("FT"));

fc2.insert_rule("LR","LL");
infrule2[3].inserePremissa(AngletoTarget.getcatptr("LR"));
//infrule2[3].inserePremissa(Distance.getcatptr("VeryNear"));
infrule2[3].insereConclusao(Direction.getcatptr("LL"));

fc2.insert_rule("VR","HL");
infrule2[4].inserePremissa(AngletoTarget.getcatptr("VR"));
//infrule2[4].inserePremissa(Distance.getcatptr("VeryNear"));
infrule2[4].insereConclusao(Direction.getcatptr("HL"));


float Ang, AngtoTarget, Dist, Direc, Sp;

// Define the defuzzification method!

fc.set_defuzz(MAXIMUM);


// Here you can put the readings of sensors of your system!!!
Dist = 50.0;
Sp = 0;

// Third try to defuzzification of outputs - CENTEROFAREA

fc.set_defuzz(CENTEROFAREA);
Sp = fc.make_inference(Dist);
cout << "\n" << "Defuzzified Output =" << Sp<<"\n\n\n";


Distance.setinputcategoryactivations(Dist);
Sp = Speed.getdefuzzyfiedoutput();
cout << "\n" << "Second Defuzzified Output =" << Sp<<"\n\n\n";

// And in the output variable you have the defuzzified control action!
// That's all folks! Now just build your own fuzzy control! Seeya!

//}

//float Ang, Direc;

Ang = -90;
Dist = 50;//2.5;
fc1.set_defuzz(CENTEROFAREA);
//fc1.save_m("Control1", 0);

        cout << "Angle = " << Ang << " Distance =" << Dist<<"\n";

        for(int i=0;i<20;i++)
        {
            infrule1[i].clearinputcategoryactivations();
            infrule1[i].clearoutputcategoryactivations();
        }
        Direc = fc1.make_inference(Ang,Dist);

        cout << "First Defuzzified Output =" << Direc<<"\n";

        Angle.setinputcategoryactivations(Ang);
        Distance.setinputcategoryactivations(Dist);

        Direc = Direction.getdefuzzyfiedoutput();
        cout <<"Second Defuzzified Output =" << Direc<<"\n";

        getchar();

*/


//Frente
cat[0].setname("Perto");//Set Range para cada categoria
cat[0].setrange(0,200);
cat[0].setval(16,16,20,50);
cat[1].setname("Medio");
cat[1].setrange(0,200);
cat[1].setval(20,50,100,150);
cat[2].setname("Longe");
cat[2].setrange(0,200);
cat[2].setval(100,150,175,200);
//Lateral Esquerda (2 sensores)
cat[3].setname("Perto Tras");
cat[3].setrange(-200,200);
cat[3].setval(-16,-16,-20,-50);
cat[4].setname("Medio Tras");
cat[4].setrange(-200,200);
cat[4].setval(-20,-50,-100,-150);
cat[5].setname("Longe Tras");
cat[5].setrange(-200,200);
cat[5].setval(-100,-150,-175,-200);
cat[6].setname("Perto Frente");
cat[6].setrange(-200,200);
cat[6].setval(16,16,20,50);
cat[7].setname("Medio Frente");
cat[7].setrange(-200,200);
cat[7].setval(20,50,100,150);
cat[8].setname("Longe Frente");
cat[8].setrange(-200,200);
cat[8].setval(100,150,175,200);
//Lateral Direita (2 sensores)
cat[9].setname("Perto Tras");
cat[9].setrange(-200,200);
cat[9].setval(-16,-16,-20,-50);
cat[10].setname("Medio Tras");
cat[10].setrange(-200,200);
cat[10].setval(-20,-50,-100,-150);
cat[11].setname("Longe Tras");
cat[11].setrange(-200,200);
cat[11].setval(-100,-150,-175,-200);
cat[12].setname("Perto Frente");
cat[12].setrange(-200,200);
cat[12].setval(16,16,20,50);
cat[13].setname("Medio Frente");
cat[13].setrange(-200,200);
cat[13].setval(20,50,100,150);
cat[14].setname("Longe Frente");
cat[14].setrange(-200,200);
cat[14].setval(100,150,175,200);
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
cat[18].setname("Vira Esquerda");
cat[18].setrange(-90,90);
cat[18].setval(-90,-90,-90,0);
cat[19].setname("Frente");
cat[19].setrange(-90,90);
cat[19].setval(-90,0,0,90);
cat[20].setname("Vira Direita");
cat[20].setrange(-90,90);
cat[20].setval(0,90,90,90);

SensorFrente.setname("SensorFrente");

SensorFrente.includecategory(&cat[0]);
cat[0].define_lingvar(&SensorFrente);
SensorFrente.includecategory(&cat[1]);
cat[1].define_lingvar(&SensorFrente);
SensorFrente.includecategory(&cat[2]);
cat[2].define_lingvar(&SensorFrente);

SensorEsquerda.setname("SensorEsquerda");

SensorEsquerda.includecategory(&cat[3]);//Define Ling var para cada categoria
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

fc.set_defuzz(AVERAGEOFMAX);
fc.definevars(SensorEsquerda,SensorFrente,SensorDireita,VelMotor);

insertRule(0,"Perto Frente","Perto","Perto Frente","Lento");
insertRule(1,"Medio Frente","Medio","Medio Frente","Medio");
insertRule(2,"Longe Frente","Longe","Longe Frente","Rapido");
insertRule(3,"Perto Tras","Perto","Perto Tras","Lento");
insertRule(4,"Medio Tras","Medio","Medio Tras","Medio");
insertRule(5,"Longe Tras","Longe","Longe Tras","Rapido");

ValorEsquerda = 21;
ValorFrente = 21;
ValorDireita = 21;

float valortestes[11] = {15,20,25,30,40,50,75,100,125,150,160};
float valortestes2[11] = {15,20,25,30,40,50,75,100,125,150,160};
float valortestes3[11] = {15,20,25,30,40,50,75,100,125,150,160};
for(int i = 0;i<11;i++){
	PercPwm = fc.make_inference(valortestes[i],valortestes2[i],valortestes3[i]);
	printf("(%f,%f,%f) : %f\n",valortestes[i],valortestes2[i],valortestes3[i],PercPwm);
}

return 0;
}

void insertRule(int n,char *se, char *sf, char *sd, char *vm){
	fc.insert_rule(se,sf,sd,vm);
	infrule[n].inserePremissa(SensorEsquerda.getcatptr(se));
	infrule[n].inserePremissa(SensorFrente.getcatptr(sf));
	infrule[n].inserePremissa(SensorDireita.getcatptr(sd));
	infrule[n].insereConclusao(VelMotor.getcatptr(vm));
}
