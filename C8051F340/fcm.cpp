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
	W[SD][GDF] = FORTE;
	W[SD][GEF] = FRACA;
	W[SD][GET] = FRACA;
	
	W[SE][GEF] = FORTE;
	W[SE][GDF] = FRACA;
	W[SE][GDT] = FRACA;
	
	W[SF][GDF] = FORTE;
	W[SF][GDT] = FRACA;
	W[SF][GEF] = FORTE;
	W[SF][GET] = FRACA;
	
	W[GDF][RD_Out] = FORTE;
	W[GDT][RD_Out] = MEDIA;
	W[GEF][RE_Out] = FORTE;
	W[GET][RE_Out] = MEDIA;
}

void inference(float se, float sf, float sd, float &rd_out, float &re_out)
{
	float gdf, gdt, gef, get;
	gdf = gdt = gef = get = 0.0f;

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
	/*
	gdt = W[SE][GDT] * (1 - 1 / (1 + exp(VAL(se))));
	gdt += W[SF][GDT] * (1 - 1 / (1 + exp(VAL(sf))));
	gdt /= W[SE][GDT] + W[SF][GDT];
	*/
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
	/*
	get = W[SD][GDT] * (1 - 1 / (1 + exp(VAL(sd))));
	get += W[SF][GDT] * (1 - 1 / (1 + exp(VAL(sf))));
	get /= W[SD][GDT] + W[SF][GDT];
	*/
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
