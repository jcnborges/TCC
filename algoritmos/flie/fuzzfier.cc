// fuzzfier.cc	V. Rao, H. Rao
// program to fuzzify data

#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "flie.h"

void category::setname(char *n)
{
strcpy(name,n);
}

char * category::getname()
{
return name;
}


void category::setval(float l, float m, float h)
{
highval=h;
midval=m;
//lowval=(l>0)?l:0;
lowval=l;
output=0.0;
}

float category::getlowval()
{
return lowval;
}

float category::getmidval()
{
return midval;
}

float category::gethighval()
{
return highval;
}       


float category::getshare(const float & input)
{
// this member function returns the relative membership
// of an input in a category, with a maximum of 1.0
float output;
float midlow, highmid;

midlow=midval-lowval;
highmid=highval-midval;
                       
// if outside the range, then output=0

cout << "input=" << input << "lowval=" << lowval << "midval=" << midval << "highval=" << highval << "\n";

if ((input <= lowval) || (input >= highval)) 
	output=0; 
	
else
	{

	if (input > midval)
	
		output=(highval-input)/highmid;
	
	
	else

		if (input==midval) 
		
			output=1.0;
	
		else

			output=(input-lowval)/midlow;
	
	}
	cout << "output=" << output << "\n";
return output;
}


void category::setoutput(const float & out)
{
output = snorm(out,output);
}

		

float category::getoutput()
{
return(output);
}


void trapezoid_category::setname(char *n)
{
strcpy(name,n);
}

char * trapezoid_category::getname()
{
return name;
}


void trapezoid_category::setval(float l, float ml,float mh, float h)
{
lowval=l>0?l:0;
midvallow=ml;
midvalhigh=mh;
highval=h;
output=0.0;
}

float trapezoid_category::getlowval()
{
return lowval;
}

float trapezoid_category::getmidvallow()
{
return midvallow;
}
float trapezoid_category::getmidvalhigh()
{
return midvalhigh;
}

float trapezoid_category::gethighval()
{
return highval;
}       


float trapezoid_category::getshare(const float & input)
{
// this member function returns the relative membership
// of an input in a trapezoid_category, with a maximum of 1.0
float output;
float midlow, highmid;

midlow=midvallow-lowval;
highmid=highval-midvalhigh;
                       
// if outside the range, then output=0
if ((input <= lowval) || (input >= highval)) 
	output=0.0; 
else
   {
      if (input > midvalhigh)
		{output=(highval-input)/highmid;}
      else
	  {
            if (input < midvallow)
           	{output=(input-lowval)/midlow;}
            else
               {
               if ((input >= midvallow) && (input <= midvalhigh) ) 
			{output=1.0;}
               }
          }
      
   }
return output;
}


void trapezoid_category::setoutput(const float & out)
{
output = snorm(out,output);
}

		

float trapezoid_category::getoutput()
{
return(output);
}

trapezoid_category::trapezoid_category(const trapezoid_category &tc)
{
	strcpy(name,tc.name);
	lowval = tc.lowval;
	highval = tc.highval;
	midvallow = tc.midvallow;
	midvalhigh = tc.midvalhigh;
        output = tc.output;

}

trapezoid_category& trapezoid_category::operator=(const trapezoid_category &tc)
{
	strcpy(name,tc.name);
	lowval = tc.lowval;
	highval = tc.highval;
	midvallow = tc.midvallow;
	midvalhigh = tc.midvalhigh;
        output = tc.output;
        return *this;
}

int trapezoid_category::operator==(const trapezoid_category &tc)
{
  return( (!strcmp(name, tc.name)) &&  (lowval==tc.lowval) && (highval == tc.highval) && 
          (midvallow == tc.midvallow) && (midvalhigh == tc.midvalhigh) );

}

int trapezoid_category::operator!=(const trapezoid_category &tc)
{
  return( (strcmp(name, tc.name)) ||  (lowval!=tc.lowval) || 
          (highval != tc.highval) || (midvallow != tc.midvallow) || 
          (midvalhigh != tc.midvalhigh) );

}

ostream& operator<<(ostream& fluxo, trapezoid_category tc)
{
//   if (strcmp(tc.name, ""))  cout << "Name = " << tc.name << endl;
   cout << "Low = " << tc.lowval << " MidLow = " << tc.midvallow << 
           " MidHigh = " << tc.midvalhigh << " High = " << tc. highval << endl;
   return(fluxo);
}


int randomnum(int maxval)
{
// random number generator
// will return an integer up to maxval

srand ((unsigned)time(NULL));
return rand() % maxval;
}



