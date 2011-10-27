// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996
// Method Definition File rule.cc - Implementation of class fuzzy_rule

#include "flie.h"

// definerule(category*,category*) - Defines a rule with just one input set
//				     and one output set!
// Each set must be part of a linguistic variable!
void rule::definerule(category *input1, category *output)
{
inputcat1 = input1;
outputcat = output;
}

// definerule(category*,category*,category*) - Defines a rule with two input sets
//				     and one output set!
void rule::definerule(category *input1,category *input2,category *output)
{
inputcat1 = input1;
inputcat2 = input2;
outputcat = output;
}
// definerule(category*,category*,category*,category*) - Defines a rule with three
// input sets and one output set!
void rule::definerule(category *input1,category *input2,category *input3, category *output)
{
inputcat1 = input1;
inputcat2 = input2;
inputcat3 = input3;
outputcat = output;
}

// evaluaterule(float) - Evaluates the rule and return the result(pertinence of the 
//			 output set!)
float rule::evaluaterule(float &inputval1)
{
 return(inputcat1->getshare(inputval1));
}

// idem for two inputs! Use a t_norm to calculate!
float rule::evaluaterule(float &inputval1, float &inputval2)
{
 return(tnorm(inputcat1->getshare(inputval1), inputcat2->getshare(inputval2)));
}

// idem for three inputs! Use a t_norm to calculate!
float rule::evaluaterule(float &inputval1, float &inputval2, float &inputval3)
{
 float x=tnorm(inputcat1->getshare(inputval1), inputcat2->getshare(inputval2));
 return(tnorm(x,inputcat3->getshare(inputval3)));
}

// getoutputcategory - return outputcat!
category *rule::getoutputcategory()
{
return(outputcat);
}


