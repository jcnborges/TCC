// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996

// Class Methods File lingvar.cc - Implementation of class linguisticvariable

#include <string.h>
#include <cassert>
#include "flie.h"

// Contructor - Just initialize number of categories(fuzzy sets) of variable.
linguisticvariable::linguisticvariable()
{
	numcategories=0;
	defuzzyfiedoutput = 0.0;
}

linguisticvariable& linguisticvariable::operator=(const linguisticvariable& l)
{
	if (this != &l) {  // make sure not same object
		strcpy(name, l.name);              // copy new name
		numcategories = l.numcategories;
		for(int i=0;i<numcategories;i++)
			cats[i] = new trapezoid_category(*(l.cats[i]));
	}
	return *this; // return ref for multiple assignment
}//end operator=

// setname - Initalize name of variable
void linguisticvariable::setname(char *namevar)
{
	strcpy(name,namevar);
}


// getname - Returns the name of the linguistic variable
char *linguisticvariable::getname()
{
	return(name);
}

// includecategory - Receive a pointer to a "category" object, and include this category
// 		     in the linguistic variable(this category should have been define priorly)
int linguisticvariable::includecategory(trapezoid_category *namecat)
{
	if(numcategories<MAXCATEGORIES)
	{
		cats[numcategories] = namecat;
		namecat->setnumber(numcategories);
		numcategories++;
		return(0);
	}
	else
		return(1);
}

// Insert a Pointer to trapezoid_category at the position "num", if there is at least num+1 categories
int linguisticvariable::setcatptr(int n, trapezoid_category *cat )
{
	if(n<MAXCATEGORIES)
	{
		if(n>numcategories) numcategories = n;
		cats[n] = cat;
		return(0);
	}
	else
	{
		cout << "Error in setcatptr: inserted after de end of categories vector\n";
		return(-1);
	}
}

// getcat(char *) - Returns the category object that have "namecat"!

trapezoid_category &linguisticvariable::getcat(char *namecat)
{
	char *tmp = NULL;

	for(int i=0;i<numcategories;i++)
	{
		tmp=cats[i]->getname();
		if(!strcmp(namecat,tmp)) return(*cats[i]);
	}
	assert(false);
}



// getcat(int) - Returns the category object of position "numcat"!
//                If numcat>numcategories, ERROR!
trapezoid_category &linguisticvariable::getcat(int numcat)
{
	if(numcat<numcategories)
		return(*cats[numcat]);
	else
		cout<<"ERROR!!! in linguisticvariable::getcat(int) with numcat=" << numcat <<
			" and number of categories =" << numcategories <<"\n";
	//return(NULL);
}

trapezoid_category *linguisticvariable::getcatptr(int numcat)
{
	if(numcat<numcategories)
		return(cats[numcat]);
	else
		cout<<"ERROR!!! in linguisticvariable::getcat(int) with numcat=" << numcat <<
			" and number of categories =" << numcategories <<"\n";
	return(NULL);
}

trapezoid_category *linguisticvariable::getcatptr(char *nm)
{
	for(int i=0;i<numcategories;i++)
	{
		if(strcmp(cats[i]->getname(), nm)==0)
			return(cats[i]);
	}
	cout<<"ERROR!!! in linguisticvariable::getcat(name) with name=" << nm <<"\n";
	return(NULL);
}



void linguisticvariable::clearactivations()
{
	for(int i=0;i<numcategories;i++)
		cats[i]->clearoutput();
}

void linguisticvariable::save_m(FILE *file, char* number)
{
	fprintf (file, "\r[%s]\r", number);
	fprintf (file, "Name='%s'\r", name);
	fprintf (file, "Range=[%d %d]\r", (int)cats[0]->getrangelow(), (int)cats[numcategories-1]->getrangehigh());
	fprintf (file, "NumMFs=%d\r",numcategories);
	// fprintf (file, "%d\r", numcategories);
	for (int i=0;i<numcategories;i++)
		cats[i]->save_m(file,i);
}

void linguisticvariable::save(FILE *file)
{
	fprintf (file, "%s\n", name);
	fprintf (file, "%d\n", numcategories);
	for (int i=0;i<numcategories;i++)
		cats[i]->save(file);
}

void linguisticvariable::load(FILE *file)
{
	char line[30];
	fgets (name, 30, file); name[strlen(name)-1] = '\0';
	fgets (line, 30, file); line[strlen(line)-1] = '\0';
	numcategories = atoi (line);

	for (int i=0;i<numcategories;i++)
	{
		cats[i] = new trapezoid_category;
		cats[i]->load(file);
		cats[i]->setnumber(i);
	}
}

void linguisticvariable::setinputcategoryactivations(float sensorreading)
{
	float out;
	for (int i=0;i<numcategories;i++)
	{
		cats[i]->clearoutput();
		out = cats[i]->getshare(sensorreading);
		if(out>0.0)
		{
			cats[i]->setoutput(out);
			((subat*)cats[i])->notify_rules();
		}
	}
}


void linguisticvariable::Notifica_Conclusao()
{
	// verifica se ninguém mais quer contribuir..... precisa fazer ainda!!!
	defuzzyfiedoutput = defuzzifyCOA();

}



// defuzzifyCOA - defuzzification by the CENTEROFAREA method
float linguisticvariable::defuzzifyCOA()
{
	trapezoid_category *outcat;
	float x=0.0;
	float defuzzout;

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

	outcat = getcatptr(0);
	float init=outcat->getlowval();
	outcat = &getcat(((getnumofcategories())-1));
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
		for(int j=0;j<getnumofcategories();j++)
		{
			outcat=&getcat(j);
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

