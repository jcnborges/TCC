// Project FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro - out/1996
// Header File rule.h

#define MAXRULES 50  // Maximum number of rules per rulebase for fuzzy_control


/* Class Rule - contains one rule of inference - can make inferences based on
 rules of 1, 2 or 3 predecessors and 1 sucessor. To define a rule, are necessary
 the fuzzy sets of predecessors and of the sucessor! Ex: 

 if Tempsense is Xlarge and Levelsense is Large then HeatKnob is Alot

 needs the following definition of rule:

 rule.definerule(SenseTemp.getcat("Xlarge"),SenseLevel.getcat("Large"), 
                   HeatKnob.getcat("Alot"));
*/

class rule
{
private:
	category *inputcat1, *inputcat2, *inputcat3;
	category *outputcat;

public:
	void definerule(category *input1,category *output);
	void definerule(category *input1,category *input2,category *output);
	void definerule(category *input1,category *input2,category *input3, 
                        category *output);
	float evaluaterule(float &inputval1);
        float evaluaterule(float &inputval1,float &inputval2);
        float evaluaterule(float &inputval1,float &inputval2,float &inputval3);
	category *getoutputcategory();
};
