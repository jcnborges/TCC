#include <cmath>
#include <cstdio>

const double k = 23959.0516095672;
const double power = -1.34599651653945;
double a4 = 3.64036E-07, a3 = -2.44354E-04, a2 = 6.07319E-02, a1 = -6.89618;
double a0 = 3.39361E02;

int f(int x){
	if(x < 40 || x > 210) return -1;
	double r = a4 * pow(x, 4) + a3 * pow(x, 3) + a2 * x * x + a1 * x + a0;
	return int(floor(r + .5));
}

int main(void){
	printf("int distances[256] = {\n");
	for(int i = 0; i < 256; ++i){
		if(i && !(i % 16)) puts("");
		printf("%4d,", f(i));
	}
	puts("\n};");
	
	return 0;
}

