// Project FLIE-Fuzzy Logic Inference Engine - Jo�o Alberto Fabro - out/1996
// Header file flie.h
#include <iostream.h>
#include "fuzzfier.h"
#include "lingvar.h"
#include "rule.h"
#include "fuzzy_control.h"

//#define DEBUG 

#define tnorm(x,y) x<y?x:y //min : macros that define the T-norm
#define snorm(x,y) x<y?y:x //max : and the S-norm used in the fuzzy inference
