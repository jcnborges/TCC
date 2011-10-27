// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996

// File flie.cc



#include <stdlib.h> // Random!

#include "flie.h"



int main()

{

// In the main program, we have to instantiate the variables to contain

// all the fuzzy sets, and define them!

category cat[20];



// And, for purpose of ease of use, we define linguistic variables to

//put the set into!

linguisticvariable SenseTemp, SenseLevel, HeatKnob;



// We also have to define the rules of inference that will define the

// behavior of the control system! We also have to instantiate them!

rule infrule[25];



// And last, we define one control system that will behave by that rules!

fuzzy_control fc;



// And some variables that will contain the inputs and outputs(defuzzified) of

// the system under control!

float LevelSense, TempSense, KnobHeat;



// Define fuzzy sets for the linguistic variable LevelSense

cat[0].setname("Xsmall");

cat[0].setval(0,1,2);

cat[1].setname("Small");

cat[1].setval(1.5,2.75,4);

cat[2].setname("Medium");

cat[2].setval(3,5,7);

cat[3].setname("Large");

cat[3].setval(6,7.25,8.5);

cat[4].setname("Xlarge");

cat[4].setval(7.5,8.75,10);



// Define the linguistic variable LevelSense

SenseLevel.setname("LevelSense");

SenseLevel.includecategory(&cat[0]);

SenseLevel.includecategory(&cat[1]);

SenseLevel.includecategory(&cat[2]);

SenseLevel.includecategory(&cat[3]);

SenseLevel.includecategory(&cat[4]);



// Define fuzzy sets for the linguistic variable TempSense

cat[5].setname("Xsmall");

cat[5].setval(0,10,20);

cat[6].setname("Small");

cat[6].setval(10,22.5,35);

cat[7].setname("Medium");

cat[7].setval(30,52.5,75);

cat[8].setname("Large");

cat[8].setval(60,77.5,95);

cat[9].setname("Xlarge");

cat[9].setval(85,105,125);



// Define Linguistic Variable TempSense

SenseTemp.setname("TempSense");

SenseTemp.includecategory(&cat[5]);

SenseTemp.includecategory(&cat[6]);

SenseTemp.includecategory(&cat[7]);

SenseTemp.includecategory(&cat[8]);

SenseTemp.includecategory(&cat[9]);





// Define fuzzy sets for the linguistic variable HeatKnob

cat[10].setname("Verylittle");

cat[10].setval(0,1,2);

cat[11].setname("Alittle");

cat[11].setval(1.5,2.75,4);

cat[12].setname("Agoodamount");

cat[12].setval(3,5,7);

cat[13].setname("Alot");

cat[13].setval(6,7.25,8.5);

cat[14].setname("Awholelot");

cat[14].setval(7.5,8.75,10);



// Define the linguistic variable HeatKnob

HeatKnob.setname("HeatKnob");

HeatKnob.includecategory(&cat[10]);

HeatKnob.includecategory(&cat[11]);

HeatKnob.includecategory(&cat[12]);

HeatKnob.includecategory(&cat[13]);

HeatKnob.includecategory(&cat[14]);



// Define the defuzzification method!

fc.set_defuzz(MAXIMUM);

// Define the fuzzy_control(by the fuzzy inputs (SenseTemp, SenseLevel) and

// output (HeatKnob)

fc.definevars(SenseTemp, SenseLevel, HeatKnob);





// Include each fuzzy rule in the fuzzy_control!(linguisticly!)



// If SenseTemp IS Xsmall and SenseLevel IS Xsmall THEN HeatKnob IS Agoodamount

fc.insert_rule("Xsmall","Xsmall","Agoodamount");



// If SenseTemp IS Xsmall and SenseLevel IS Small THEN HeatKnob IS Alot

fc.insert_rule("Xsmall","Small","Alot");



// If SenseTemp IS Xsmall and SenseLevel IS Medium THEN HeatKnob IS Awholelot

fc.insert_rule("Xsmall","Medium","Awholelot");



// If SenseTemp IS Xsmall and SenseLevel IS Large THEN HeatKnob IS Awholelot

fc.insert_rule("Xsmall","Large","Awholelot");

// If SenseTemp IS Xsmall and SenseLevel IS Xlarge THEN HeatKnob IS Awholelot

fc.insert_rule("Xsmall","Xlarge","Awholelot");



// If SenseTemp IS Small and SenseLevel IS Xsmall THEN HeatKnob IS Alittle

fc.insert_rule("Small","Xsmall","Alittle");



// If SenseTemp IS Small and SenseLevel IS Small THEN HeatKnob IS Agoodamount

fc.insert_rule("Small","Small","Agoodamount");



// If SenseTemp IS Small and SenseLevel IS Medium THEN HeatKnob IS Alot

fc.insert_rule("Small","Medium","Alot");



// If SenseTemp IS Small and SenseLevel IS Large THEN HeatKnob IS Alot

fc.insert_rule("Small","Large","Alot");



// If SenseTemp IS Small and SenseLevel IS Xlarge THEN HeatKnob IS Alot

fc.insert_rule("Small","Xlarge","Alot");





// If SenseTemp IS Medium and SenseLevel IS Xsmall THEN HeatKnob IS Verylittle

fc.insert_rule("Medium","Xsmall","Verylittle");



// If SenseTemp IS Medium and SenseLevel IS Small THEN HeatKnob IS Verylittle

fc.insert_rule("Medium","Small","Verylittle");



// If SenseTemp IS Medium and SenseLevel IS Medium THEN HeatKnob IS Agoodamount

fc.insert_rule("Medium","Medium","Agoodamount");



// If SenseTemp IS Medium and SenseLevel IS Large THEN HeatKnob IS Alot

fc.insert_rule("Medium","Large","Alot");



// If SenseTemp IS Medium and SenseLevel IS Xlarge THEN HeatKnob IS Alot

fc.insert_rule("Medium","Xlarge","Alot");





// If SenseTemp Large and SenseLevel IS Small THEN HeatKnob IS Verylittle

fc.insert_rule("Large","Small","Verylittle");



// If SenseTemp IS Large and SenseLevel IS Medium THEN HeatKnob IS Verylittle

fc.insert_rule("Large","Medium","Verylittle");



// If SenseTemp IS Large and SenseLevel IS Large THEN HeatKnob IS Alittle

fc.insert_rule("Large","Large","Alittle");



// If SenseTemp IS Large and SenseLevel IS Xlarge THEN HeatKnob IS Agoodamount

fc.insert_rule("Large","Xlarge","Agoodamount");





// Here you can put the readings of sensors of your system!!!

LevelSense = 7.0;

TempSense = 65.0;





// First try to defuzzification of outputs - set of maximum ativation!!!!

KnobHeat = fc.make_inference(TempSense, LevelSense);

cout << "\n" << "First Defuzzified Output =" << KnobHeat<<"\n\n\n";



// Second try to defuzzification of outputs - AVERAGEOFMAX

fc.set_defuzz(AVERAGEOFMAX);

KnobHeat = fc.make_inference(TempSense, LevelSense);

cout << "\n" << "Second Defuzzified Output = " << KnobHeat<<"\n\n\n";



// Third try to defuzzification of outputs - CENTEROFAREA

fc.set_defuzz(CENTEROFAREA);

KnobHeat = fc.make_inference(TempSense, LevelSense);

cout << "\n" << "Third Defuzzified Output = " << KnobHeat<<"\n\n";





// And in the output variable you have the defuzzified control action!

// That's all folks! Now just build your own fuzzy control! Seeya!



//} 



fc.set_defuzz(CENTEROFAREA);



float Waterin,Waterout;

int i=0;

srand(1967);



 for(i=0;i<100;i++)

 {

   cout << "\n" << "LevelSense =" << LevelSense;

   cout << "\n" << "TempSense  =" << TempSense;



   KnobHeat = fc.make_inference(TempSense, LevelSense);

   cout << "\n" << "Defuzzified Output =" << KnobHeat;



   Waterin=random();

   Waterin=Waterin/(float)RAND_MAX;

   Waterout=random();

   Waterout=Waterout/(float)RAND_MAX;

   cout << "\n" << Waterin << "\n"<< Waterout<< RAND_MAX;

   TempSense+=KnobHeat-8*Waterin;

   LevelSense=LevelSense-2*Waterout+2*Waterin;

 }
return 0;
}

