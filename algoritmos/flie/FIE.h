#ifndef FIE
#define FIE
#include "rule.h"

/*class at : public linguisticvariable
{

};
*/

/*
class ar : public rule
{
private:
   int num_conditions;
}
*/

class subat : public trapezoid_category
{
private:

public:
        int num_rules;
        linguisticvariable *lv;
        rule *rules[MAXRULES];

        subat(){num_rules=0;};
        void define_lingvar(linguisticvariable *l);
        linguisticvariable *getlv();
        void insertrule(rule *rl);
        void notify_rules();
};
#endif

