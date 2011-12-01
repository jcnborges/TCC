#include "flie.h"

flie flie;

int main(){
	flie.flie_setup();
	float a,b;
	flie.inference(20,20,20,a,b);
	printf("%f %f",a,b);
	return 0;	
}