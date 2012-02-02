//Arquivo Main - Setup Flie e chamar inferências a partir da disponibilidade de dados

#include "flie.h"

flie flie;

int main(){
	flie.flie_setup();
	float a,b;
	flie.inference(125,125,125,a,b);
	printf("%f %f\n",a,b);
	return 0;	
}
