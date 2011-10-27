// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996
// Class Methods File lingvar.cc - Implementation of class linguisticvariable
#include <string.h>
#include "flie.h"


// Contructor - Just initialize number of categories(fuzzy sets) of variable.
linguisticvariable::linguisticvariable()
{
  numcategories=0;
}

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
int linguisticvariable::includecategory(category *namecat)
{
	if(numcategories<MAXCATEGORIES)
	{
		cats[numcategories] = namecat;
		numcategories++;
		return(0);
	}
	else
		return(1);
}
// getcat(char *) - Returns the category object that have "namecat"!
category &linguisticvariable::getcat(char *namecat)
{
char *tmp;

  for(int i=0;i<numcategories;i++)
   {
   tmp=cats[i]->getname();
   if(!strcmp(namecat,tmp))
      return(*cats[i]);
   }
}

// getcat(int) - Returns the category object of position "numcat"! 
//                If numcat>numcategories, ERROR! 
category &linguisticvariable::getcat(int numcat)
{
  if(numcat<numcategories)
     return(*cats[numcat]);
  else
    cout<<"ERROR!!! in linguisticvariable::getcat(int) with numcat="<<numcat<<
    " and number of categories =" << numcategories <<"\n";
}

