// fuzzfier.cc	V. Rao, H. Rao
// program to fuzzify data

#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//#include "ga.h" // GARandomInt
#include "flie.h"

int randomnum(int,int);

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
if ((input < lowval) || (input > highval))
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
#ifdef DEBUG
// cout << "Getshare: " << output << "\t";
#endif
return output;
}


void category::setoutput(const float & out)
{
output = snorm(out,output);
}

void category::clearoutput()
{
        output = 0.0;
}

float category::getoutput()
{
return(output);
}

ostream& operator<<(ostream& fluxo, category tc)
{
   if (strcmp(tc.name, ""))  cout << "Name = " << tc.name << "\t";
   cout << "Low = " << tc.lowval << " MidVal = " << tc.midval << " High = " << tc. highval << endl;
   return(fluxo);
}

//---------------  Trapezoid_Category -------------------

trapezoid_category::trapezoid_category()
{
rangelow = rangehigh = 0.0;
}

trapezoid_category::trapezoid_category(int n)
{
rangelow = rangehigh = NOTSET;
}

trapezoid_category::trapezoid_category(const trapezoid_category &tc)
{
	strcpy(name,tc.name);
        number = tc.number;
        rangelow = tc.rangelow;
        rangehigh = tc.rangehigh;
	lowval = tc.lowval;
	highval = tc.highval;
	midvallow = tc.midvallow;
	midvalhigh = tc.midvalhigh;
        output = tc.output;

}

void trapezoid_category::setname(char *n)
{
strcpy(name,n);
}

char * trapezoid_category::getname()
{
return name;
}

void trapezoid_category::setrange(float rl, float rh)
{
  rangelow = rl;
  rangehigh = rh;
}

void trapezoid_category::setval(float l, float ml,float mh, float h)
{
if(rangelow==NOTSET)
  {
  rangelow = l;
  lowval = l;
  }
else if(rangelow<=l)
  lowval=l;
else
  cout << "Error - Out of low range!" << endl;
midvallow=ml;
midvalhigh=mh;

if(rangehigh==NOTSET)
  {
  rangehigh = h;
  highval = h;
  }
else if(rangehigh>=h)
  highval=h;
else
  cout << "Error - Out of high range!" << endl;

output=0.0;
}

void trapezoid_category::setrandomval()
{
// Sets random numbers: lowval between rangelow and rangehigh;
//                      highval between lowvaw and rangehigh;
//                      midval between lowval and highval
// Special Case: Fuzzy Set QZ(Quasi Zero), that is simetric, centered on zero!!!(JAF 13/12/2001)
     if(!strcmp(name, "QZ"))
      {
      midvallow = midvalhigh = 0.0;
      highval = randomnum(0,(int)(rangehigh));
      lowval = -highval;
      }
     else
      {
      lowval = rangelow + randomnum(0,(int)(rangehigh - rangelow));
      highval = rangehigh - randomnum(0,(int)(rangehigh - lowval));
      midvallow=midvalhigh = lowval + randomnum(0,(int)(highval - lowval));
      }
}

void trapezoid_category::setlefttrapezoidrandomval()
{
// Sets random numbers: lowval and midvallow equal rangelow;
//                      highval between lowvaw and rangehigh;
//                      midval between lowval(midvallow) and highval
     lowval = midvallow = rangelow;
     highval = rangehigh - randomnum(0,(int)(rangehigh - lowval));
     midvalhigh = lowval + randomnum(0,(int)(highval - lowval));
}

void trapezoid_category::setrighttrapezoidrandomval()
{
// Sets random numbers: highval = midvalhigh = rangehigh;
//                      lowval between lowvaw and midvalhigh(=highval);
//                      midvallow between lowval and highval
     midvalhigh = highval = rangehigh;
     lowval = rangelow + randomnum(0,(int)(highval - rangelow));
     midvallow = lowval + randomnum(0,(int)(highval - lowval));

}


void trapezoid_category::setval(float l, float m, float h)
{
setval(l,m,m,h);
}
/*
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
*/

float trapezoid_category::getmidval()
{
return ((midvallow+midvalhigh)/2.0);
}

void trapezoid_category::move_lowval()
{
  float newlowval;
  newlowval = lowval + (randomnum((int)rangelow,(int)rangehigh)*0.1);// Variation: +-10% of range of category
  if ( (newlowval>=rangelow) && (newlowval<=midvallow) )
     lowval = newlowval;
}

void trapezoid_category::move_midvallow()
{
  float newmidvallow;
  newmidvallow = midvallow + (randomnum((int)rangelow,(int)rangehigh)*0.1);// Variation range: -7.5~+7.5
  if ( (newmidvallow>=lowval) && (newmidvallow<=midvalhigh) )
     midvallow = newmidvallow;
  if(!strcmp(name, "QZ") && (midvallow>0.0))
     midvallow = 0.0;

}

void trapezoid_category::move_midvalhigh()
{
  float newmidvalhigh;
  newmidvalhigh = midvalhigh + (randomnum((int)rangelow,(int)rangehigh)*0.1);// Variation range: -7.5~+7.5
  if ( (newmidvalhigh>=midvallow) && (newmidvalhigh<=highval) )
     midvalhigh = newmidvalhigh;
}

void trapezoid_category::move_highval()
{
  float newhighval;
  newhighval = highval + (randomnum((int)rangelow,(int)rangehigh)*0.1);// Variation range: -7.5~+7.5
  if ( (newhighval>=midvalhigh) && (newhighval<=rangehigh) )
     highval = newhighval;
}


float trapezoid_category::getshare(const float & input)
{
// this member function returns the relative membership
// of an input in a trapezoid_category, with a maximum of 1.0

float output;   // This local variable assures that this function just calculates
                //the activation, but doesn't sets it to the Membership Function 02/05/2003
float midlow, highmid;

midlow=midvallow-lowval;
highmid=highval-midvalhigh;

// if outside the access range, then output=0
if ((input < lowval) || (input > highval))
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
               else
                        output = 0.0;// Equals to lowval or highval!
               }

          }

   }

return output;
}


void trapezoid_category::setoutput(const float & out)
{
        output = snorm(out,output);
}

void trapezoid_category::clearoutput()
{
        output = 0.0;
}

float trapezoid_category::getoutput()
{
return(output);
}

trapezoid_category& trapezoid_category::operator=(const trapezoid_category &tc){
	strcpy(name,tc.name);
        number = tc.number;
        rangelow = tc.rangelow;
        rangehigh = tc.rangehigh;
	lowval = tc.lowval;
	highval = tc.highval;
	midvallow = tc.midvallow;
	midvalhigh = tc.midvalhigh;
        output = tc.output;
        return *this;
}

bool trapezoid_category::operator==(const trapezoid_category &tc) const
{
  return(
  (!strcmp(name, tc.name)) && (number==tc.number) &&
  (lowval==tc.lowval) && (highval == tc.highval) &&
  (midvallow == tc.midvallow) && (midvalhigh == tc.midvalhigh) );

}

bool trapezoid_category::operator!=(const trapezoid_category &tc) const
{
  return(
  (strcmp(name, tc.name)) || (number != tc.number) ||
          (lowval!=tc.lowval) ||
          (highval != tc.highval) || (midvallow != tc.midvallow) ||
          (midvalhigh != tc.midvalhigh) );

}

ostream& operator<<(ostream& fluxo, trapezoid_category tc)
{
   if (strcmp(tc.name, ""))  cout << tc.name << "\t" << tc.number << "\t";
   cout << "Low = " << tc.lowval << " MidLow = " << tc.midvallow <<
           " MidHigh = " << tc.midvalhigh << " High = " << tc. highval << endl;
   return(fluxo);
}

void trapezoid_category::save_m(FILE *file, int number)
{
  fprintf (file, "MF%d='%s':", number+1, name);
  fprintf (file, "'trapmf',[");
  fprintf (file, "%4.1f %4.1f %4.1f %4.1f]\r", lowval, midvallow, midvalhigh, highval);
}

void trapezoid_category::save(FILE *file)
{
  fprintf (file, "%s\n", name);
  fprintf (file, "%f\n%f\n", rangelow, rangehigh);
  fprintf (file, "%f\n%f\n%f\n%f\n", lowval, midvallow, midvalhigh, highval);
}

void trapezoid_category::load(FILE *file)
{
  char line[30];

  fgets (name, 30, file); name[strlen(name)-1] = '\0';
  fgets (line, 30, file); line[strlen(line)-1] = '\0';
  rangelow = atof(line);
  fgets (line, 30, file); line[strlen(line)-1] = '\0';
  rangehigh = atof(line);
  fgets (line, 30, file); line[strlen(line)-1] = '\0';
  lowval = atof(line);
  fgets (line, 30, file); line[strlen(line)-1] = '\0';
  midvallow = atof(line);
  fgets (line, 30, file); line[strlen(line)-1] = '\0';
  midvalhigh = atof(line);
  fgets (line, 30, file); line[strlen(line)-1] = '\0';
  highval = atof(line);

//  fscanf (file, "%f\t%f\n", rangelow, rangehigh);
//  fscanf (file, "%f\t%f\t%f\t%f\n", lowval, midvallow, midvalhigh, highval);
}


int randomnum(int minval, int maxval)
{
// random number generator
// will return an integer up to maxval
//srand ((unsigned)time(NULL));
return rand() % maxval;
//return(GARandomInt(minval, maxval));
}



