// fuzzfier.h	V. Rao, H. Rao

// program to fuzzify data

#ifndef _CATEGORY_CLASSES
#define _CATEGORY_CLASSES
#include <iostream>
#include <stdio.h>

#define NOTSET 0.000131415

class category
{

private:



public:
	char name[30];
	float lowval,highval,midval;
        float output;

	category(){};
	void setname(char *);
	char * getname();	
	void setval(float, float, float);
	float getlowval();
	float getmidval();
	float gethighval();

	float getshare(const float&);
        void setoutput(const float&);
        void clearoutput();
        float getoutput();
       	//friend ostream& operator<<(ostream& fluxo, category tc);

	~category(){};

};

class trapezoid_category
{
private:



public:
	char name[30];
        int number;
	float lowval,highval,midvallow, midvalhigh;
        float output;
        float rangelow, rangehigh;

	trapezoid_category();
        trapezoid_category(int);
	void setname(char *);
	char * getname();
        void setnumber(int t) {number = t;};
        int getnumber() {return(number);};
        void setrange(float, float);
        float getrangelow(){return(rangelow);};
        float getrangehigh(){return(rangehigh);};
      	void setrandomval();
      	void setrighttrapezoidrandomval();
        void setlefttrapezoidrandomval();
        void setval(float , float, float, float);
        void setval(float, float, float);
        void move_lowval();
        void change_lowval(){};
	float getlowval(){return lowval;};
	float getmidvallow(){return midvallow;};
        void move_midvallow();
        void change_midvallow(){};
	float getmidval(); // Returns average between midvallow and midvalhigh
	float getmidvalhigh(){return midvalhigh;};
        void move_midvalhigh();
        void change_midvalhigh(){};
	float gethighval(){return highval;};
        void move_highval();         
        void change_highval(){};       

	float getshare(const float&);
        void setoutput(const float&);
        void clearoutput();
        float getoutput();
   	trapezoid_category(const trapezoid_category &tc);
   	trapezoid_category &operator=(const trapezoid_category &tc);
   	bool operator==(const trapezoid_category &tc) const;
   	bool operator!=(const trapezoid_category &tc) const;
//   	friend ostream& operator<<(ostream& fluxo, trapezoid_category tc);

        void save_m(FILE *file, int number);
        void save(FILE *file);
        void load(FILE *file);


	~trapezoid_category(){};

};
int randnum(int);


#endif
