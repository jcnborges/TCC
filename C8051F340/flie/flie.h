// Projeto FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro

#ifndef flieheader
#define flieheader
#include <iostream>
#include "motor.h"
#include "fuzzfier.h"
#include "lingvar.h"
#include "rule.h"
#include "fuzzy_control.h"
#include "stdlib.h"
#include "string.h"
#include "FIE.h"
using namespace std;

#define tnorm(x,y) ((x)<(y)?(x):(y)) //min : macros that define the T-norm
#define snorm(x,y) ((x)<(y)?(y):(x)) //max : and the S-norm used in the fuzzy inference

class flie
{
	public:
	subat cat[21];

	linguisticvariable SensorEsquerda,SensorFrente,SensorDireita, VelMotor, AngMotor;

	rule infrule[54];
	rule infrule2[54];

	fuzzy_control fc,fc2;

	float PercPwm, Ang;

	void flie_setup();
	void insertRule(int, char *, char *, char *, char*, char*);
	void inference(float , float , float , float &, float &);

};
#endif
