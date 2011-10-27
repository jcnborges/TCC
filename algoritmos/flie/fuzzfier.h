// fuzzfier.h	V. Rao, H. Rao

// program to fuzzify data

#ifndef _CATEGORY_CLASSES
#define _CATEGORY_CLASSES
#include <iostream.h>

class category
{

private:

	char name[30];
	float lowval,highval,midval;
        float output;

public:
	category(){};
	void setname(char *);
	char * getname();	
	void setval(float, float, float);
	float getlowval();
	float getmidval();
	float gethighval();

	float getshare(const float&);
        void setoutput(const float&);
        float getoutput();

	~category(){};   

}; 

class trapezoid_category
{
private:

	char name[30];
	float lowval,highval,midvallow, midvalhigh;
        float output;

public:
	trapezoid_category(){};
	void setname(char *);
	char * getname();	
	void setval(float, float, float, float);
	float getlowval();
	float getmidvallow();
	float getmidvalhigh();
	float gethighval();

	float getshare(const float&);
        void setoutput(const float&);
        float getoutput();
   	trapezoid_category(const trapezoid_category &tc);
   	trapezoid_category &operator=(const trapezoid_category &tc);
   	int operator==(const trapezoid_category &tc);
   	int operator!=(const trapezoid_category &tc);
   	friend ostream& operator<<(ostream& fluxo, trapezoid_category tc);


	~trapezoid_category(){};   

};

int randnum(int);


#endif
