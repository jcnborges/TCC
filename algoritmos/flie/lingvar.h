// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996
// Header File lingvar.h 

#define MAXCATEGORIES 11 // Maximum number of fuzzy sets that define a ling. variable

// Class linguisticvariable - contains all fuzzy sets that define one variable
class linguisticvariable
{
private:
	char name[30];
	int numcategories;
	category *cats[MAXCATEGORIES];

public:
	linguisticvariable();
	void setname(char*);
        char *getname();
	int includecategory(category*);
        category& getcat(char*);
        category& getcat(int);
        int getnumofcategories(){return(numcategories);};

};
