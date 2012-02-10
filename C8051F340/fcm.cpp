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
#define LIMIAR 0.1
#define SIG(x) (1 / (1 + exp(-10 * x + 4.5)))
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

void init_W()
{
	W[SD][GDF] = FORTE; // w1 (+) FORTE
	W[SD][GEF] = FORTE; // w2 (+) FRACA
	W[SD][GET] = FORTE; // w3 (+) MEDIA
	
	W[SE][GEF] = FORTE; // w4 (+) FORTE
	W[SE][GDF] = FORTE; // w5 (+) FRACA
	W[SE][GDT] = FORTE; // w6 (+) MEDIA
	
	W[SF][GDF] = MEDIA; // w7 (+) FORTE
	W[SF][GDT] = MEDIA; // w8 (+) FRACA 
	W[SF][GEF] = MEDIA; // w9 (+) FORTE
	W[SF][GET] = MEDIA; // w10 (+) FRACA

	W[GDF][RD_Out] = FORTE; // w11 (+) FORTE
	W[GDT][RD_Out] = FORTE; // w12 (-) MEDIA
	W[GEF][RE_Out] = FORTE; // w13 (+) FORTE
	W[GET][RE_Out] = FORTE; // w14 (-) MEDIA

	W[GDT][SD] = FRACA; // w15 (-) FRACA
	W[GET][SE] = FRACA; // w16 (-) FRACA
}

void init_W(float sf)
{
	if (sf > LIMIAR)
	{
		W[SD][GDF] = FORTE; // w1 (+) FORTE
		W[SD][GEF] = FORTE; // w2 (+) FRACA
		W[SD][GET] = FORTE; // w3 (+) MEDIA
		
		W[SE][GEF] = FORTE; // w4 (+) FORTE
		W[SE][GDF] = FORTE; // w5 (+) FRACA
		W[SE][GDT] = FORTE; // w6 (+) MEDIA
		
		W[SF][GDF] = MEDIA; // w7 (-) MEDIA
		W[SF][GDT] = MEDIA; // w8 (+) FRACA 
		W[SF][GEF] = MEDIA; // w9 (-) MEDIA
		W[SF][GET] = MEDIA; // w10 (+) FRACA

		W[GDF][RD_Out] = FORTE; // w11 (+) FORTE
		W[GDT][RD_Out] = FORTE; // w12 (-) MEDIA
		W[GEF][RE_Out] = FORTE; // w13 (+) FORTE
		W[GET][RE_Out] = FORTE; // w14 (-) MEDIA

		W[GDT][SD] = FRACA; // w15 (-) FRACA
		W[GET][SE] = FRACA; // w16 (-) FRACA
	}
	else
	{		
		W[SD][GDF] = FRACA; // w1 (+) FRACO
		W[SD][GEF] = FORTE; // w2 (+) FORTE
		W[SD][GET] = FRACA; // w3 (+) FRACA
		
		W[SE][GEF] = FORTE; // w4 (+) FORTE
		W[SE][GDF] = FRACA; // w5 (+) FRACA
		W[SE][GDT] = FORTE; // w6 (+) MEDIA
		
		W[SF][GDF] = FRACA; // w7 (+) FRACA
		W[SF][GDT] = FORTE; // w8 (+) FORTE 
		W[SF][GEF] = MEDIA; // w9 (+) FORTE
		W[SF][GET] = FRACA; // w10 (+) FRACA

		W[GDF][RD_Out] = FORTE; // w11 (+) FORTE
		W[GDT][RD_Out] = FORTE; // w12 (-) MEDIA
		W[GEF][RE_Out] = FORTE; // w13 (+) FORTE
		W[GET][RE_Out] = FORTE; // w14 (-) MEDIA

		W[GDT][SD] = FRACA; // w15 (-) FRACA
		W[GET][SE] = FORTE; // w16 (-) FORTE
	}
}

void inference(float se, float sf, float sd, float &rd_out, float &re_out)
{
	float gdf, gdt, gef, get;
	gdf = gdt = gef = get = 0.0f;

	// normaliza distancias
	sd = (sd - MIN_DIST) / float(MAX_DIST - MIN_DIST);
	sf = (sf - MIN_DIST) / float(MAX_DIST - MIN_DIST);
	se = (se - MIN_DIST) / float(MAX_DIST - MIN_DIST);

	// inicializa os pesos de acordo com a distancia sd
	init_W(sf);

	// calcula o valor do conceito gdf
	gdf = W[SD][GDF] * (1 - SIG(sd));
	gdf += W[SF][GDF] * SIG(sf);
	gdf -= W[SE][GDF] * (1 - SIG(se));
	gdf /= W[SD][GDF] + W[SF][GDF];
	gdf = gdf > 0 ? gdf : 0;

	// calcula o valor do conceito gdt
	gdt = W[SE][GDT] * (1 - SIG(se));
	gdt += W[SF][GDT] * (1 - SIG(sf));
	gdt -= W[SD][GDT] * (1 - SIG(sd));
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
	get -= W[SE][GET] * (1 - SIG(se));
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
