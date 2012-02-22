#include<cmath>
#include <cstdio>
// -----------------------------------------
//
// DEFINES E ENUMS
//
// -----------------------------------------
#define FORTE 1.0f
#define MEDIA 0.5f
#define FRACA 0.125f
#define MIN_DIST 14
#define MAX_DIST 150
#define SIG(x) (1 / (1 + exp(-10 * x + 4.5)))
#define L0 0.15
#define L1 0.25
#define FRENTE 1
#define TRAS 0
enum 
{
	SE, SF,	SD, 
	GDF, GDT, GEF, GET, 
	RD_Out, RE_Out
};
// -----------------------------------------
//
// DECLARACAO DE CONSTANTES
//
// -----------------------------------------

// -----------------------------------------
//
// DECLARACAO DE VARIAVEIS GLOBAIS
//
// -----------------------------------------
float W[9][9];

void init_W(float sf)
{
	static int ESTADO = 1;
	// Transicao de estado
	if(ESTADO == FRENTE && sf < L0) ESTADO = TRAS;
	else if(ESTADO == TRAS && sf > L1) ESTADO = FRENTE;
	// Ajuste dos pesos
	if(ESTADO == FRENTE){
		W[SD][GDF] = FORTE; // w1 (+)
		W[SD][GEF] = FRACA; // w2 (+)
		W[SD][GET] = MEDIA; // w3 (+)
		
		W[SE][GEF] = FORTE; // w4 (+)
		W[SE][GDF] = FRACA; // w5 (+)
		W[SE][GDT] = MEDIA; // w6 (+)
		
		W[SF][GDF] = MEDIA; // w7 (-)
		W[SF][GDT] = FORTE; // w8 (+) 
		W[SF][GEF] = MEDIA; // w9 (-)
		W[SF][GET] = FRACA; // w10 (+)
		
		W[GDF][RD_Out] = FORTE; // w11 (+)
		W[GDT][RD_Out] = MEDIA; // w12 (-)
		W[GEF][RE_Out] = FORTE; // w13 (+)
		W[GET][RE_Out] = MEDIA; // w14 (-)
	} else if(ESTADO == TRAS){
		W[SD][GDF] = FRACA; // w1 (+)
		W[SD][GEF] = FRACA; // w2 (+)
		W[SD][GET] = FORTE; // w3 (+)
		
		W[SE][GEF] = FRACA; // w4 (+)
		W[SE][GDF] = FRACA; // w5 (+)
		W[SE][GDT] = FORTE; // w6 (+)
		
		W[SF][GDF] = FRACA; // w7 (-)
		W[SF][GDT] = MEDIA; // w8 (+) 
		W[SF][GEF] = MEDIA; // w9 (-)
		W[SF][GET] = FRACA; // w10 (+)
		
		W[GDF][RD_Out] = FRACA; // w11 (+)
		W[GDT][RD_Out] = FORTE; // w12 (-)
		W[GEF][RE_Out] = FORTE; // w13 (+)
		W[GET][RE_Out] = FRACA; // w14 (-)
	}
}

void inference(float se, float sf, float sd, float &rd_out, float &re_out)
{
	float gdf, gdt, gef, get;
	gdf = gdt = gef = get = 0.0f;
	init_W(sf);

	// normaliza distancias
	sd = (sd - MIN_DIST) / float(MAX_DIST - MIN_DIST);
	sf = (sf - MIN_DIST) / float(MAX_DIST - MIN_DIST);
	se = (se - MIN_DIST) / float(MAX_DIST - MIN_DIST);
	
	// calcula o valor do conceito gdf
	gdf = W[SD][GDF] * (1 - SIG(sd));
	gdf += W[SF][GDF] * SIG(sf);
	gdf -= W[SE][GDF] * (1 - SIG(se));
	gdf /= W[SD][GDF] + W[SF][GDF];
	gdf = gdf > 0 ? gdf : 0;

	// calcula o valor do conceito gdt
	gdt = W[SE][GDT] * (1 - SIG(se));
	gdt += W[SF][GDT] * (1 - SIG(sf));
	gdt /= W[SE][GDT] + W[SF][GDT];

	// calcula o valor do conceito gef
	gef = W[SE][GEF] * (1 - SIG(se));
	gef += W[SF][GEF] * SIG(sf);
	gef -= W[SD][GEF] * (1 - SIG(sd));
	gef /= W[SE][GEF] + W[SF][GEF];
	gef = gef > 0 ? gef : 0;

	// calcula o valor do conceito get
	get = W[SD][GET] * (1 - SIG(sd));
	get += W[SF][GET] * (1 - SIG(sf));
	get /= W[SD][GET] + W[SF][GET];

	printf("gdf = %.5lf\ngef = %.5lf\n", gdf, gef);
	// calcula o valor do conceito rd_out
	rd_out = W[GDF][RD_Out] * gdf;
	rd_out -= W[GDT][RD_Out] * gdt;
	rd_out /= FORTE;
	rd_out *= 100;

	// calcula o valor do conceito re_out
	re_out = W[GEF][RE_Out] * gef;
	re_out -= W[GET][RE_Out] * get;
	re_out /= FORTE;
	re_out *= 100;
}

