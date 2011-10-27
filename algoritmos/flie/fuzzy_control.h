// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996
// Header File fuzzy_control.h

                	 // Kinds of defuzzification
#define MAXIMUM		1// Value of maximum activation of output set
#define AVERAGEOFMAX 	2// Average of activations of output sets 
#define CENTEROFAREA 	3// Center of area of activated output sets

// Class fuzzy_control : receive all rules, executes the inference engine
// and the defuzzification process
class fuzzy_control
{
private:
   	linguisticvariable lingvarinput1,lingvarinput2,lingvarinput3;
        linguisticvariable lingvaroutput;
	int numrules;
	rule *rulebase[MAXRULES];
        float activationofrule[MAXRULES];
        int kindofdefuzzification;
        float defuzzify();
        float defuzzifyMAX();
        float defuzzifyMOM();
        float defuzzifyCOA();
public:
   	fuzzy_control(){kindofdefuzzification=MAXIMUM;numrules=0;};
   	fuzzy_control(int kindofdefuzz){kindofdefuzzification=kindofdefuzz;numrules=0;};
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

};

