// Project FLIE-Fuzzy Logic Inference Engine - Joƒo Alberto Fabro - out/1996
// Method Definition File rule.cc - Implementation of class fuzzy_rule
#include "iostream.h"
#include "flie.h"

rule::rule()
{
 inputcat1 = inputcat2 = inputcat3 = outputcat = NULL;
 num_inputs = 0;
 at1 = at2 = at3 = 0.0;
}
// definerule(category*,category*) - Defines a rule with just one input set
//				     and one output set!
// Each set must be part of a linguistic variable!
void rule::definerule(trapezoid_category *input1, trapezoid_category *output)
{
inputcat1 = input1;
outputcat = output;
}

// definerule(category*,category*,category*) - Defines a rule with two input sets
//				     and one output set!
void rule::definerule(trapezoid_category *input1,trapezoid_category *input2,trapezoid_category *output)
{
inputcat1 = input1;
inputcat2 = input2;
outputcat = output;
}
// definerule(category*,category*,category*,category*) - Defines a rule with three
// input sets and one output set!
void rule::definerule(trapezoid_category *input1,trapezoid_category *input2,trapezoid_category *input3, trapezoid_category *output)
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
 outputcat->setoutput(inputcat1->getshare(inputval1));
 return(outputcat->getoutput());
}

float rule::evaluaterule_memberships(float memb_input1)
{
 outputcat->setoutput(memb_input1);
 return(outputcat->getoutput());
}

// idem for two inputs! Use a t_norm to calculate!
float rule::evaluaterule(float &inputval1, float &inputval2)
{
 outputcat->setoutput(tnorm(inputcat1->getshare(inputval1), inputcat2->getshare(inputval2)));
 return(outputcat->getoutput());
}

float rule::evaluaterule_memberships(float memb_input1, float memb_input2)
{
 outputcat->setoutput(tnorm(memb_input1, memb_input2));
 return(outputcat->getoutput());
}

// idem for three inputs! Use a t_norm to calculate!
float rule::evaluaterule(float &inputval1, float &inputval2, float &inputval3)
{
 float x=tnorm(inputcat1->getshare(inputval1), inputcat2->getshare(inputval2));
outputcat->setoutput(tnorm(x,inputcat3->getshare(inputval3)));
 return(outputcat->getoutput());
}

float rule::evaluaterule_memberships(float memb_input1, float memb_input2, float memb_input3)
{
 float x=tnorm(memb_input1, memb_input2);
 outputcat->setoutput(tnorm(x,memb_input3));
 return(outputcat->getoutput());
}

// getoutputcategory - return outputcat!
trapezoid_category *rule::getoutputcategory()
{
return(outputcat);
}

void rule::clearinputcategoryactivations()
{
      if(inputcat1!=NULL) {inputcat1->clearoutput();at1 = 0;};
      if(inputcat2!=NULL) {inputcat2->clearoutput();at2 = 0;};
      if(inputcat3!=NULL) {inputcat3->clearoutput();at3 = 0;};
}


void rule::clearoutputcategoryactivations()
{
//       if(outputcat!=NULL)
            outputcat->clearoutput();
}

void rule::save_m(FILE *file)
{
 if(inputcat1)
   fprintf(file, "%d",inputcat1->getnumber()+1);
 if(inputcat2)
   fprintf(file, " %d",inputcat2->getnumber()+1);
 if(inputcat3)
   fprintf(file, " %d",inputcat3->getnumber()+1);
 fprintf(file, ", ");

 if(outputcat)
   fprintf(file, "%d (1) : 1\r",outputcat->getnumber()+1);
}

void rule::save(FILE *file)
{
 if(inputcat1)
   fprintf(file, "%s\n",inputcat1->getname());
 else
   fprintf(file, "NULL\n");
 if(inputcat2)
   fprintf(file, "%s\n",inputcat2->getname());
 else
   fprintf(file, "NULL\n");
 if(inputcat3)
   fprintf(file, "%s\n",inputcat3->getname());
 else
   fprintf(file, "NULL\n");
 if(outputcat)
   fprintf(file, "%s\n",outputcat->getname());
}

void rule::load(FILE *file)
{
 // instead of loading rule, we load the names of the categories of the rule
 // inside the fuzzy_control object!!!
 // and use "insert_rule" from fuzzy_control!!!

}

rule::~rule()
{
/*
 if(inputcat1)
   free(inputcat1);
 if(inputcat2)
   free(inputcat2);
 if(inputcat3)
   free(inputcat3);
 if(outputcat)
   free(outputcat);
*/
}

void rule::Notifica(trapezoid_category *at)
{
float outputactivation=0.0;
if(num_inputs==1)
        {
         at1 = at->getoutput();
         outputactivation = evaluaterule_memberships(at1);
        }
else
if(num_inputs==2)
        {
        if(at ==inputcat1)
         at1 = at->getoutput();
        else
        if(at ==inputcat2)
         at2 = at->getoutput();

        if((at1>0.0)&&(at2>0.0))
         outputactivation = evaluaterule_memberships(at1, at2);
        }
else
if(num_inputs==3)
        {
        if(at ==inputcat1)
          at1 = at->getoutput();
        else
        if(at ==inputcat2)
         at2 = at->getoutput();
        else
        if(at ==inputcat3)
         at3 = at->getoutput();

        if((at1>0.0)&&(at2>0.0)&&(at2>0.0))
         outputactivation = evaluaterule_memberships(at1, at2, at3);
        }
/*---------??????????????-----*/
linguisticvariable *ll;

if(outputactivation>0.0)
 {
        ll = ((subat*)outputcat)->getlv();
        ll->Notifica_Conclusao();
 }
}

void rule::Notifica_Conclusao()
{

}

void rule::inserePremissa(trapezoid_category *tc)
{
subat *teste;
teste = (subat *)tc;
if((num_inputs==0)&&(tamvp<TAM))
        {
        inputcat1 = tc;
        teste->insertrule(this);
        tamvp++;
        num_inputs++;
        }
else
if((num_inputs==1)&&(tamvp<TAM))
        {
        inputcat2 = tc;
        teste->insertrule(this);
        tamvp++;
        num_inputs++;
        }
else
if((num_inputs==2)&&(tamvp<TAM))
        {
        inputcat3 = tc;
        teste->insertrule(this);
        tamvp++;
        num_inputs++;
        }
else
        cout << "Erro na inserção de Premissa na Regra!\n";
}

void rule::insereConclusao(trapezoid_category *tc)
{
subat *teste;
teste = (subat *)tc;
        outputcat = tc;
        teste->insertrule(this);
}

