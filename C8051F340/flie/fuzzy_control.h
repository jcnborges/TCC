// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996
// Revision 17/12/2001 - Save and Load Controller, By JAF

// Header File fuzzy_control.h



                	 // Kinds of defuzzification

#define MAXIMUM		1// Value of maximum activation of output set

#define AVERAGEOFMAX 	2// Average of activations of output sets 

#define CENTEROFAREA 	3// Center of area of activated output sets



// Class fuzzy_control : receive all rules, executes the inference engine

// and the defuzzification process

class fuzzy_control

{

protected:

   	linguisticvariable lingvarinput1,lingvarinput2,lingvarinput3;

        linguisticvariable lingvaroutput;

	int numinputvars;
        
        int numrules;

	rule *rulebase[MAXRULES];

        float activationofrule[MAXRULES];

        int kindofdefuzzification;

        float defuzzify();

        float defuzzifyMAX();

        float defuzzifyMOM();

        float defuzzifyCOA();

        void loadrulebase(FILE *file);

public:

   	fuzzy_control(){kindofdefuzzification=MAXIMUM;numrules=0;};

   	fuzzy_control(int kindofdefuzz){kindofdefuzzification=kindofdefuzz;numrules=0;};

        int getkindofdefuzzification() { return kindofdefuzzification;};

        int getnuminputvars() { return numinputvars;};

        int getnumrules() { return numrules;};

        rule *getrule(int i) { return rulebase[i];};

        void set_defuzz(int kindofdefuzz){kindofdefuzzification=kindofdefuzz;};

        int definevars(linguisticvariable &input1,linguisticvariable &output);

        int definevars(linguisticvariable &input1,linguisticvariable &input2,

                	linguisticvariable &output);

        int definevars(linguisticvariable &input1,linguisticvariable &input2,

                	linguisticvariable &input3,linguisticvariable &output);        

        int insert_rule(rule& ruletoinsert);

        int insert_rule(char *setofinplingvar1,char *setofoutlingvar);

        int insert_rule(char *setofinplingvar1,char *setofinplingvar2,

                	 char *setofoutlingvar);

        int insert_rule(char *setofinplingvar1,char *setofinplingvar2,

                	 char *setofinplingvar3,char *setofoutlingvar);

        float make_inference(float inputval1);

        float make_inference(float inputval1,float inputval2);

        float make_inference(float inputval1,float inputval2,float inputval3);

        float getnumoflingvars() { return (float)numinputvars;};

        linguisticvariable *getlingvarinput1(){return &lingvarinput1;};

        linguisticvariable *getlingvarinput2(){return &lingvarinput2;};

        linguisticvariable *getlingvarinput3(){return &lingvarinput3;};

        linguisticvariable *getlingvaroutput(){return &lingvaroutput;};

        void save_m(char *name, float error);

        void save(char *name, float error);

        void load(char *name);


};

class fuzzy_merged_control:fuzzy_control
{
private:

public:
       	linguisticvariable lingvarinput1merge1,lingvarinput2merge1,lingvarinput3merge1;
       	linguisticvariable lingvarinput1merge2,lingvarinput2merge2,lingvarinput3merge2;
        linguisticvariable lingvaroutputmerge1;
        linguisticvariable lingvaroutputmerge2;

        void merge(fuzzy_control fc1, fuzzy_control fc2);

        float make_inference(float inputval1, float percent);

        float make_inference(float inputval1,float inputval2, float percent);

        float make_inference(float inputval1,float inputval2,float inputval3, float percent);
};



