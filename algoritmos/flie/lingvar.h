// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996

// Header File lingvar.h 

#define MAXCATEGORIES 11 // Maximum number of fuzzy sets that define a ling. variable


// Class linguisticvariable - contains all fuzzy sets that define one variable

class linguisticvariable : public TTipoPremissa

{

private:



public:
	char name[30];

	int numcategories;

	trapezoid_category *cats[MAXCATEGORIES];

                float defuzzyfiedoutput;


	linguisticvariable();

        linguisticvariable& operator=(const linguisticvariable& l);

	void setname(char*);

        char *getname();

	int includecategory(trapezoid_category*);
        
        int setcatptr(int,trapezoid_category*);
        
        trapezoid_category& getcat(char*);

        trapezoid_category& getcat(int);

        trapezoid_category* getcatptr(int);
         
        int getnumofcategories(){return(numcategories);};
        
        void clearactivations();

                void setinputcategoryactivations(float sensorreading);

        void save_m(FILE *file, char* number);

        void save(FILE *file);

        void load(FILE *file);

        trapezoid_category *getcatptr(char *nm);

                void Notifica_Conclusao();

                float defuzzifyCOA();

                float getdefuzzyfiedoutput(){return(defuzzyfiedoutput);};


};

