// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996
// Class Methods File fuzzy_control.cc - Implementation of class fuzzy_control

#include "flie.h"
// definevars  - Three methods to define the linguistic variables of input and
//		 output that control the behavior of a system
int fuzzy_control::definevars(linguisticvariable &input1,linguisticvariable &output)
{
lingvarinput1 = input1;
lingvaroutput = output;
}

int fuzzy_control::definevars(linguisticvariable &input1,linguisticvariable &input2,linguisticvariable &output)
{
lingvarinput1 = input1;
lingvarinput2 = input2;
lingvaroutput = output;
}

int fuzzy_control::definevars(linguisticvariable &input1,linguisticvariable &input2, 
                              linguisticvariable &input3,linguisticvariable &output)
{
lingvarinput1 = input1;
lingvarinput2 = input2;
lingvarinput3 = input3;
lingvaroutput = output;
}

//-------------------------------------------------------------

// insert_rule - Three methods to insert fuzzy rules in the control algorithm
int fuzzy_control::insert_rule(char *setofinplingvar1, char *setofoutlingvar)
{
	if(numrules<MAXRULES)
	{
         rulebase[numrules] = new rule;
         rulebase[numrules]->definerule(&lingvarinput1.getcat(setofinplingvar1),
                                        &lingvaroutput.getcat(setofoutlingvar));
         numrules++;
         return(0);
	}
	else
         return(1);
}

int fuzzy_control::insert_rule(char *setofinplingvar1,char *setofinplingvar2, 
                        	char *setofoutlingvar)
{
	if(numrules<MAXRULES)
	{
         rulebase[numrules] = new rule;
         rulebase[numrules]->definerule(&lingvarinput1.getcat(setofinplingvar1),
                                        &lingvarinput2.getcat(setofinplingvar2),
                                        &lingvaroutput.getcat(setofoutlingvar));
         numrules++;
         return(0);
	}
	else
         return(1);
}

int fuzzy_control::insert_rule(char *setofinplingvar1,char *setofinplingvar2,
                        	char *setofinplingvar3,char *setofoutlingvar)
{
	if(numrules<MAXRULES)
	{
         rulebase[numrules] = new rule;
         rulebase[numrules]->definerule(&lingvarinput1.getcat(setofinplingvar1),
                                	&lingvarinput2.getcat(setofinplingvar2),
                                        &lingvarinput3.getcat(setofinplingvar3),
                                        &lingvaroutput.getcat(setofoutlingvar));
         numrules++;
         return(0);
	}
	else
         return(1);
}

// insert_rule(rule&) - polimorphic method to insert rules already created!
int fuzzy_control::insert_rule(rule& ruletoinsert)
{
	if(numrules<MAXRULES)
	{
           	rulebase[numrules] = new rule;
                *rulebase[numrules] = ruletoinsert;
                numrules++;
		return(0);
	}
	else
		return(1);

}


//-------------------------------------------------------------

// make_inference - Three methods to dispath the inference mecanism!

float fuzzy_control::make_inference(float inputval1)
{
  for(int i=0;i<numrules;i++)
   {
     activationofrule[i]=rulebase[i]->evaluaterule(inputval1);
   }
	
 return(defuzzify());
}

float fuzzy_control::make_inference(float inputval1,float inputval2)
{
  for(int i=0;i<numrules;i++)
   {
     activationofrule[i]=rulebase[i]->evaluaterule(inputval1, inputval2);
   }
 
 return(defuzzify());

}

float fuzzy_control::make_inference(float inputval1,float inputval2,float inputval3)
{
  for(int i=0;i<numrules;i++)
   {
     activationofrule[i]=rulebase[i]->evaluaterule(inputval1, inputval2, inputval3);
   } 
 return(defuzzify());
}

//-------------------------------------------------------------

// defuzzify - method that choose among the three different methods of 
//             defuzzification - the default is MAXIMUM, to
//	       set a new method, use the constructor or set_defuzz with the
//	       predefined constants : MAXIMUM, AVERAGEOFMAX and CENTEROFAREA
float fuzzy_control::defuzzify()
{
float defuzzout=0.0;

 if(kindofdefuzzification== MAXIMUM)
 {
  defuzzout=defuzzifyMAX();
 }
else if(kindofdefuzzification== AVERAGEOFMAX)
 {
 defuzzout = defuzzifyMOM();
 }
else if(kindofdefuzzification== CENTEROFAREA)
 {
 defuzzout = defuzzifyCOA(); 
 } 
 return(defuzzout);
}


// defuzzifyMAX - defuzzification by the MAXIMUM
float fuzzy_control::defuzzifyMAX()
{
category *outcat;
float x=0.0;
float biggest=0.0;
float defuzzout;
 
  for(int i=0;i<numrules;i++)
   {
     if((x=activationofrule[i])>biggest)
      {
        biggest = x;
        outcat = rulebase[i]->getoutputcategory();
        defuzzout = outcat->getmidval(); 
#ifdef DEBUG
        cout << i << "\t" << x <<"\t" << defuzzout <<"\t" << outcat->getname()<<"\n";
#endif
      }
   }
 return(defuzzout);  
}

// defuzzifyMOM - defuzzification by the AVERAGEOFMAX method
float fuzzy_control::defuzzifyMOM()
{
category *outcat;
float x=0.0;
float defuzzout;
float sumativations=0.0;
int countrulesfiring=0;
int numcats;
     
   for(int i=0;i<numrules;i++)
    {
     if((x=activationofrule[i])>0.0)
      {
	countrulesfiring++;
  	outcat = rulebase[i]->getoutputcategory(); 
        outcat->setoutput(x);
#ifdef DEBUG
        cout << i << "\t" << x << "\t" << outcat->getname()<<outcat->getoutput()<<"\n";
#endif
      }
    }

  defuzzout=0.0; 
  numcats=lingvaroutput.getnumofcategories();
  for(int i=0;i<numcats; i++)
  {
   outcat=&lingvaroutput.getcat(i);
   defuzzout+=outcat->getoutput()*outcat->getmidval();
   sumativations += outcat->getoutput();
#ifdef DEBUG
   cout << outcat->getoutput() << "\t" <<  outcat->getmidval();
   cout << outcat->getname() << defuzzout << "\n";
#endif
  }
 defuzzout = defuzzout/sumativations;  	

 return(defuzzout);
}

// defuzzifyCOA - defuzzification by the CENTEROFAREA method
float fuzzy_control::defuzzifyCOA()
{
category *outcat;
float x=0.0;
float defuzzout;

#ifdef DEBUG
  for(int i=0;i<numrules;i++)
   {
     if((x=activationofrule[i])>0.0)
      {
        outcat = rulebase[i]->getoutputcategory(); 
        cout << i << "\t" << x << "\t" << outcat->getname()<<"\n";
      }
   }
#endif
 outcat = &lingvaroutput.getcat(0);
 float init=outcat->getlowval();
 outcat = &lingvaroutput.getcat(((lingvaroutput.getnumofcategories())-1));
 float final=outcat->gethighval();
 float step=(final-init)/(100);

float localsum;
float globalsum=0;
float valuessum=0.0;
float aux=0.0;
  
 for(float i=init;i<=final;i+=step)
 {
   localsum=0.0;
   for(int j=0;j<lingvaroutput.getnumofcategories();j++)
   {
   outcat=&lingvaroutput.getcat(j);
    aux=outcat->getoutput(); 
    if(aux!=0.0) 
       localsum+=tnorm(outcat->getshare(i),aux);//MUST be min??? 07/11/96 Hans
   }
   globalsum+=localsum*i;
   valuessum+=localsum;
 }
 defuzzout = globalsum/valuessum;
 
 return(defuzzout);
}

