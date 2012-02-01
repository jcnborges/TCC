// Project FLIE-Fuzzy Logic Inference Engine - Joƒo Alberto Fabro - out/1996
// Class Methods File fuzzy_control.cc - Implementation of class fuzzy_control

#include "flie.h"
#include "iostream.h"
#define DEBUG
// definevars  - Three methods to define the linguistic variables of input and
//		 output that control the behavior of a system
int fuzzy_control::definevars(linguisticvariable &input1,linguisticvariable &output)
{
numinputvars = 1;        
lingvarinput1 = input1;
lingvaroutput = output;
return(1);
}

int fuzzy_control::definevars(linguisticvariable &input1,linguisticvariable &input2,linguisticvariable &output)
{
numinputvars = 2;
lingvarinput1 = input1;
lingvarinput2 = input2;
lingvaroutput = output;
return(1);
}

int fuzzy_control::definevars(linguisticvariable &input1,linguisticvariable &input2,
                              linguisticvariable &input3,linguisticvariable &output)
{
numinputvars = 3;
lingvarinput1 = input1;
lingvarinput2 = input2;
lingvarinput3 = input3;
lingvaroutput = output;
return(1);
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

// make_inference - Three methods to dispatch the inference mecanism!

float fuzzy_control::make_inference(float inputval1)
{
for(int i=0;i<numrules;i++)
   {
    rulebase[i]->clearinputcategoryactivations();
    rulebase[i]->clearoutputcategoryactivations();
   }
  for(int i=0;i<numrules;i++)
   {
     activationofrule[i]=rulebase[i]->evaluaterule(inputval1);
#ifdef DEBUG
     cout << "Activ of Rule " << i << "= ";
     cout << activationofrule[i] << "\n";
#endif
   }

 return(defuzzify());
}

float fuzzy_control::make_inference(float inputval1,float inputval2)
{
for(int i=0;i<numrules;i++)
   {
    rulebase[i]->clearinputcategoryactivations();
    rulebase[i]->clearoutputcategoryactivations();
   }

  for(int i=0;i<numrules;i++)
   {
     activationofrule[i]=rulebase[i]->evaluaterule(inputval1, inputval2);
#ifdef DEBUG
     cout << "Activation of Rule " << i << "= ";
     cout << activationofrule[i] << "\n";
#endif
   }

 return(defuzzify());

}

float fuzzy_control::make_inference(float inputval1,float inputval2,float inputval3)
{
for(int i=0;i<numrules;i++)
   {
    rulebase[i]->clearinputcategoryactivations();
    rulebase[i]->clearoutputcategoryactivations();
   }

for(int i=0;i<numrules;i++)
   {
     activationofrule[i]=rulebase[i]->evaluaterule(inputval1, inputval2, inputval3);
#ifdef DEBUG
     cout << "Activ of Rule " << i << "= ";
     cout << activationofrule[i] << "\n";
#endif
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
trapezoid_category *outcat;
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
        cout << "\nRule " << i << "\tActivation " << x <<"\tDefuzzMax " << defuzzout <<"\tCategory " << outcat->getname()<<"\n";
#endif
      }
   }
 return(defuzzout);  
}

// defuzzifyMOM - defuzzification by the AVERAGEOFMAX method
float fuzzy_control::defuzzifyMOM()
{
trapezoid_category *outcat;
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
        cout << "Rule: " << i << "\tActivation: " << x << "\t" << outcat->getname()<< "\t" <<outcat->getoutput()<<"\n";
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
   cout << "Output: " << outcat->getoutput() << "\tMidval: " <<  outcat->getmidval();
   cout << "\tCategory: " << outcat->getname() << "\tDefuzzMOM: " << defuzzout << "\t Sumativ: " << sumativations << "\n";
#endif

  }
 if(sumativations!=0.0)
   defuzzout = defuzzout/sumativations;  	
 else
   defuzzout = 0.0;

 return(defuzzout);
}

// defuzzifyCOA - defuzzification by the CENTEROFAREA method
float fuzzy_control::defuzzifyCOA()
{
trapezoid_category *outcat;
float x=0.0;
float defuzzout;

 for(int i=0;i<numrules;i++)
   {
     if((x=activationofrule[i])>0.0)
      {
        outcat = rulebase[i]->getoutputcategory();
        outcat->setoutput(x);
      }
   }

#ifdef DEBUG
  for(int i=0;i<numrules;i++)
   {
     if((x=activationofrule[i])>0.0)
      {
        outcat = rulebase[i]->getoutputcategory();
        cout << "\nRule " << i << "\tActivation " << x << "\tCategory " << outcat->getname()<<"\n";
      }
   }
#endif

 outcat = &lingvaroutput.getcat(0);
 float init=outcat->getlowval();
 outcat = &lingvaroutput.getcat(((lingvaroutput.getnumofcategories())-1));
 float final=outcat->gethighval();
 float step=(final-init)/(100);

 #ifdef DEBUG
        cout << "\nBegin: " << init << "\tEnd: " << final <<"\tStep: " << step <<"\n";
#endif

float localsum;
float globalsum=0;
float valuessum=0.0;
float aux=0.0;
float aux1=0.0;
  
 for(float i=init;i<=final;i+=step)
 {
   localsum=0.0;
   for(int j=0;j<lingvaroutput.getnumofcategories();j++)
   {
    outcat=&lingvaroutput.getcat(j);
    aux=outcat->getoutput(); 
    aux1=outcat->getshare(i);
    if(aux!=0.0) 
       localsum+=tnorm(aux1,aux);//MUST be min??? 07/11/96 Hans
#ifdef DEBUG
        cout << "\nI= " << i << "\tJ=" << j << "\tAux=Activ: " << aux <<"\tGetshare(i)=" << aux1 << "\tSum " << localsum <<"\tCat " << outcat->getname();
#endif       
   }
#ifdef DEBUG
   cout << "\n";   
#endif
   globalsum+=localsum*i;
   valuessum+=localsum;
#ifdef DEBUG
        cout << "\nLocalsum: " << localsum <<"\tValuesum " << valuessum;
#endif
 }
 if(valuessum!=0.0)
  defuzzout = globalsum/valuessum;
 else
  defuzzout = 0.0;

 return(defuzzout);
}


void fuzzy_control::save_m(char *name, float error)
{
  FILE *fileFC;

  char fileName[30];
  int i, j;

  strcpy(fileName, name);
  strcat(fileName, ".fis");
  if ( (fileFC = fopen ( fileName, "w" )) == NULL )
    {
      cout << "Error in file creation:" << fileName << "\n";
      exit(1);
    }
fprintf(fileFC,"[System]\nName='%s'\rType='mamdani'\rVersion=2.0\r", name);
fprintf(fileFC,"NumInputs=%d\rNumOutputs=1\rNumRules=%d\r", numinputvars, numrules);
fprintf(fileFC,"AndMethod='min'\rOrMethod='max'\rImpMethod='min'\rAggMethod='max'\r");
if(kindofdefuzzification == 3)
        fprintf ( fileFC, "DefuzzMethod='centroid'\r");
else
if(kindofdefuzzification == 2)
        fprintf ( fileFC, "DefuzzMethod='mom'\r");
else
if(kindofdefuzzification == 1)
        fprintf ( fileFC, "DefuzzMethod='bisector'\r");

  if (numinputvars>0)
    lingvarinput1.save_m( fileFC, "Input1" );

  if (numinputvars>1)
    lingvarinput2.save_m( fileFC, "Input2" );
  if (numinputvars>2)
    lingvarinput3.save_m( fileFC, "Input3" );

  lingvaroutput.save_m( fileFC , "Output1" );

  fprintf ( fileFC, "\r\r[Rules]\r");

  for(int i=0;i<numrules;i++)
  {
     rulebase[i]->save_m( fileFC);
  }
  
  fclose (fileFC);
}


void fuzzy_control::save(char *name, float error)
{
  FILE *fileFC;

  char fileName[30];
  int i, j;

  strcpy(fileName, name);
  strcat(fileName, ".fc");
  if ( (fileFC = fopen ( fileName, "w" )) == NULL )
    {
      cout << "Error in file creation:" << fileName << "\n";
      exit(1);
    }
  fprintf ( fileFC, "%d\n", kindofdefuzzification );
  fprintf ( fileFC, "%d\n", numinputvars );

  if (numinputvars>0)
    lingvarinput1.save( fileFC );
  if (numinputvars>1)
    lingvarinput2.save( fileFC );
  if (numinputvars>2)
    lingvarinput3.save( fileFC );

  lingvaroutput.save( fileFC );

  fprintf ( fileFC, "%d\n",  numrules);

  for(int i=0;i<numrules;i++)
  {
     rulebase[i]->save( fileFC);
  }

  fprintf ( fileFC, "Error with this controller = %f\n", error );

  fclose (fileFC);
}

void fuzzy_control::load(char *name)
{
  FILE *fileFC;
  char line[30];
  char fileName[30];
  int i, j;


  if (numrules > 0)
  {
     for(i=0;i<numrules;i++)
       free(rulebase[i]);
     numrules = 0;
  }

  strcpy(fileName, name);
  strcat(fileName, ".fc");
  if ( (fileFC = fopen ( fileName, "r" )) == NULL )
    {
      cout << "Error in file opening:" << fileName << "\n";
      exit(1);
    }

  fgets ( line, 30, fileFC); line[strlen(line)-1] = '\0';
  kindofdefuzzification = atoi(line);

  fgets ( line, 30, fileFC); line[strlen(line)-1] = '\0';
  numinputvars = atoi(line);


  if (numinputvars>0)
    lingvarinput1.load( fileFC );
  if (numinputvars>1)
    lingvarinput2.load( fileFC );
  if (numinputvars>2)
    lingvarinput3.load( fileFC );

  lingvaroutput.load( fileFC );

  fgets ( line, 30, fileFC); line[strlen(line)-1] = '\0';
  j = atoi(line);//numrules!!

/*  for(int i=0;i<numrules;i++)
  {
     rulebase[i] = new rule;
     rulebase[i]->load( fileFC);
  }
*/
  for(i=0;i<j;i++)
    loadrulebase(fileFC);

  fclose (fileFC);
}

void fuzzy_control::loadrulebase(FILE *file)
{
char cat1[30], cat2[30], cat3[30], cat4[30];
  fgets (cat1, 30, file); cat1[strlen(cat1)-1] = '\0';
  fgets (cat2, 30, file); cat2[strlen(cat2)-1] = '\0';
  fgets (cat3, 30, file); cat3[strlen(cat3)-1] = '\0';
  fgets (cat4, 30, file); cat4[strlen(cat4)-1] = '\0';
  if(!strcmp(cat3, "NULL"))
  {
     if(!strcmp(cat2, "NULL"))
       insert_rule(cat1, cat4);
     else
       insert_rule(cat1, cat2, cat4);
  }
  else
    insert_rule(cat1, cat2, cat3, cat4);
}



//************* Merged Controller **************

void fuzzy_merged_control::merge(fuzzy_control fc1, fuzzy_control fc2)
{
if(fc1.getkindofdefuzzification() == fc2.getkindofdefuzzification())
        kindofdefuzzification = fc1.getkindofdefuzzification();
else
   {
    printf("Incompatible fuzzy controllers being merged - KindOfDefuzzification Error!\n");
    exit(0);
   }
if(fc1.getnuminputvars() == fc2.getnuminputvars())
        numinputvars = fc1.getnuminputvars();
else
   {
    printf("Incompatible fuzzy controllers being merged - NumInputvars Error!\n");
    exit(0);
   }

if(!strcmp(fc1.getlingvarinput1()->getname(), fc2.getlingvarinput1()->getname()))
   {
        lingvarinput1 = *(fc1.getlingvarinput1());
        lingvarinput1merge1 = *(fc1.getlingvarinput1());
        lingvarinput1merge2 = *(fc2.getlingvarinput1());
   }
else
   {
    printf("Incompatible fuzzy controllers being merged - LingVarInput1 Error!\n");
    exit(0);
   }

if(numinputvars>1)
 if(!strcmp(fc1.getlingvarinput2()->getname(), fc2.getlingvarinput2()->getname()))
   {
        lingvarinput2 = *(fc1.getlingvarinput2());
        lingvarinput2merge1 = *(fc1.getlingvarinput2());
        lingvarinput2merge2 = *(fc2.getlingvarinput2());
   }
 else
   {
    printf("Incompatible fuzzy controllers being merged - LingVarInput2 Error!\n");
    exit(0);
   }

if(numinputvars>2)
 if(!strcmp(fc1.getlingvarinput3()->getname(), fc2.getlingvarinput3()->getname()))
   {
        lingvarinput3 = *(fc1.getlingvarinput3());
        lingvarinput3merge1 = *(fc1.getlingvarinput3());
        lingvarinput3merge2 = *(fc2.getlingvarinput3());
   }
 else
   {
    printf("Incompatible fuzzy controllers being merged - LingVarInput3 Error!\n");
    exit(0);
   }

if(!strcmp(fc1.getlingvaroutput()->getname(), fc2.getlingvaroutput()->getname()))
   {
        lingvaroutput = *(fc1.getlingvaroutput());
        lingvaroutputmerge1 = *(fc1.getlingvaroutput());
        lingvaroutputmerge2 = *(fc2.getlingvaroutput());
   }
else
   {
    printf("Incompatible fuzzy controllers being merged - LingVarOutput Error!\n");
    exit(0);
   }

/*if(fc1.getnumrules() == fc2.getnumrules())
        numrules = fc1.getnumrules();
else
   {
    printf("Incompatible fuzzy controllers being merged - NumRules Error!\n");
    exit(0);
   }
*/

//for(int i = 0; i<numrules; i++)
//        rulebase[i] = fc1.getrule(i);
{
// Include each fuzzy rule in the fuzzy_control!(linguisticly!)

// If NextStepError IS Negative Big and DeltaError IS Negative Big THEN Control IS Positive Big
insert_rule("NB","NB","PB");
insert_rule("NB","NS","PB");
insert_rule("NB","NVS","PB");
insert_rule("NB","QZ","PB");
insert_rule("NB","PVS","PB");
insert_rule("NB","PS","PB");
insert_rule("NB","PB","PB");


// If NextStepError IS Negative Small and DeltaError IS Negative Big THEN Control IS Positive Big
insert_rule("NS","NB","PB");
insert_rule("NS","NS","PB");
insert_rule("NS","NVS","PB");
insert_rule("NS","QZ","PS");
insert_rule("NS","PVS","PVS");
insert_rule("NS","PS","QZ");
insert_rule("NS","PB","NVS");


// If NextStepError IS Negative Very Small and DeltaError IS Negative Big THEN Control IS Positive Big
insert_rule("NVS","NB","PB");
insert_rule("NVS","NS","PB");
insert_rule("NVS","NVS","PS");
insert_rule("NVS","QZ","PVS");
insert_rule("NVS","PVS","QZ");
insert_rule("NVS","PS","NVS");
insert_rule("NVS","PB","NS");


// If NextStepError IS  Quasi Zero and DeltaError IS Negative Big THEN Control IS Positive Big
insert_rule("QZ","NB","PB");
insert_rule("QZ","NS","PS");
insert_rule("QZ","NVS","PVS");
insert_rule("QZ","QZ","QZ");
insert_rule("QZ","PVS","NVS");
insert_rule("QZ","PS","NS");
insert_rule("QZ","PB","NB");


// If NextStepError IS Positive Very Small and DeltaError IS Negative Big THEN Control IS Positive Small
insert_rule("PVS","NB","PS");
insert_rule("PVS","NS","PVS");
insert_rule("PVS","NVS","QZ");
insert_rule("PVS","QZ","NVS");
insert_rule("PVS","PVS","NS");
insert_rule("PVS","PS","NB");
insert_rule("PVS","PB","NB");


// If NextStepError IS Positive Small and DeltaError IS Negative Big THEN Control IS Positive Very Small
insert_rule("PS","NB","PVS");
insert_rule("PS","NS","QZ");
insert_rule("PS","NVS","NVS");
insert_rule("PS","QZ","NS");
insert_rule("PS","PVS","NB");
insert_rule("PS","PS","NB");
insert_rule("PS","PB","NB");


// If NextStepError IS  Positive Big and DeltaError IS Negative Big THEN Control IS Quasi Zero
insert_rule("PB","NB","NB");
insert_rule("PB","NS","NB");
insert_rule("PB","NVS","NB");
insert_rule("PB","QZ","NB");
insert_rule("PB","PVS","NB");
insert_rule("PB","PS","NB");
insert_rule("PB","PB","NB");

}

}

float fuzzy_merged_control::make_inference(float inputval1, float percent)
{
}

float fuzzy_merged_control::make_inference(float inputval1,float inputval2, float percent)
{
trapezoid_category *tmptc, *tmptc1, *tmptc2;
float output;
// for linguisticvariable lingvarinput1
for(int i=0;i<lingvarinput1.getnumofcategories();i++)
   {
     tmptc = lingvarinput1.getcatptr(i);
     tmptc1 = lingvarinput1merge1.getcatptr(i);
     tmptc2 = lingvarinput1merge2.getcatptr(i);

     tmptc->setval(
     tmptc1->getlowval()*percent + tmptc2->getlowval()*(1.0-percent),
     tmptc1->getmidvallow()*percent + tmptc2->getmidvallow()*(1.0-percent),
     tmptc1->getmidvalhigh()*percent + tmptc2->getmidvalhigh()*(1.0-percent),
     tmptc1->gethighval()*percent + tmptc2->gethighval()*(1.0-percent)
                  );
   }

// for linguisticvariable lingvarinput2
for(int i=0;i<lingvarinput2.getnumofcategories();i++)
   {
     tmptc = lingvarinput2.getcatptr(i);
     tmptc1 = lingvarinput2merge1.getcatptr(i);
     tmptc2 = lingvarinput2merge2.getcatptr(i);

     tmptc->setval(
     tmptc1->getlowval()*percent + tmptc2->getlowval()*(1-percent),
     tmptc1->getmidvallow()*percent + tmptc2->getmidvallow()*(1-percent),
     tmptc1->getmidvalhigh()*percent + tmptc2->getmidvalhigh()*(1-percent),
     tmptc1->gethighval()*percent + tmptc2->gethighval()*(1-percent)
                  );
   }

// for linguisticvariable lingvaroutput
for(int i=0;i<lingvaroutput.getnumofcategories();i++)
   {
     tmptc = lingvaroutput.getcatptr(i);
     tmptc1 = lingvaroutputmerge1.getcatptr(i);
     tmptc2 = lingvaroutputmerge2.getcatptr(i);

     tmptc->setval(
     tmptc1->getlowval()*percent + tmptc2->getlowval()*(1-percent),
     tmptc1->getmidvallow()*percent + tmptc2->getmidvallow()*(1-percent),
     tmptc1->getmidvalhigh()*percent + tmptc2->getmidvalhigh()*(1-percent),
     tmptc1->gethighval()*percent + tmptc2->gethighval()*(1-percent)
                  );
   }

output = fuzzy_control::make_inference(inputval1, inputval2);
return(output);
}

float fuzzy_merged_control::make_inference(float inputval1,float inputval2,float inputval3, float percent)
{
}

