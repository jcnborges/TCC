#include <cmath>
#include <cstdio>

const double k = 23959.0516095672;
const double power = -1.34599651653945;

int f(int x){
	if(x < 40 || x > 220) return -1;
	double r = 4E-7 * pow(x, 4) - 0.0002 * pow(x, 3) +
			0.0607 * x * x - 6.8962 * x + 339.36;
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

