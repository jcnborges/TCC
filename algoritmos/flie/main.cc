//Arquivo Main - Setup Flie e chamar infer�ncias a partir da disponibilidade de dados

#include "flie.h"

flie flie;

int main(){
	flie.flie_setup();
	float a,b;
	flie.inference(20,20,20,a,b);
	printf("%f %f\n",a,b);
	return 0;	
}
