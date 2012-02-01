#include "flie.h"
#include "FIE.h"

void subat::define_lingvar(linguisticvariable *l)
{
	lv = l;
}

linguisticvariable *subat::getlv()
{
	return(lv);
}
void subat::insertrule(rule *rl)
{
	if(num_rules<MAXRULES)
	{
		rules[num_rules] = rl;
		num_rules++;
	}
	else
		cout <<"Erro na inserção de regras em subat\n";
}
// Método que notifica as regras das quais esta subat é premissa!!!
void subat::notify_rules()
{
	float out;
	out =  this->getoutput();
	if(out>0.0)
		for(int i=0;i<num_rules;i++)
			rules[i]->Notifica(this);
}
