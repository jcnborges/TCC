#include<cmath>
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
#define VAL(x) (-10*x+4.5)
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
	W[SD][GEF] = MEDIA;
	W[SD][GET] = FORTE;
	
	W[SE][GEF] = FORTE;
	W[SE][GDF] = MEDIA;
	W[SE][GDT] = FORTE;
	
	W[SF][GDF] = MEDIA;
	W[SF][GDT] = FRACA;
	W[SF][GEF] = MEDIA;
	W[SF][GET] = FRACA;
	
	W[GDF][RD_Out] = FORTE;
	W[GDT][RD_Out] = FORTE;
	W[GEF][RE_Out] = FORTE;
	W[GET][RE_Out] = FORTE;
}

void inference(int se, int sf, int sd, float &rd_out, float &re_out)
{
	float gdf, gdt, gef, get;

	// normaliza distancias
	sd = (sd - MIN_DIST) / (MAX_DIST - MIN_DIST);
	sf = (sf - MIN_DIST) / (MAX_DIST - MIN_DIST);
	se = (se - MIN_DIST) / (MAX_DIST - MIN_DIST);
	
	// calcula o valor do conceito gdf
	gdf = W[SD][GDF] * 1 / (1 + exp(VAL(sd)));
	gdf += W[SE][GDF] * 1 / (1 + exp(VAL(se)));
	gdf -= W[SF][GDF] * 1 / (1 + exp(VAL(sf)));
	gdf /= W[SD][GDF] + W[SE][GDF] - W[SF][GDF];

	// calcula o valor do conceito gdt
	gdt = W[SE][GDT] * 1 / (1 + exp(VAL(se)));
	gdt += W[SF][GDT] * 1 / (1 + exp(VAL(sf)));
	gdt /= W[SE][GDT] + W[SF][GDT];
	gdt = 1 - gdt;

	// calcula o valor do conceito gef
	gef = W[SE][GEF] * 1 / (1 + exp(VAL(se)));
	gef += W[SD][GEF] * 1 / (1 + exp(VAL(sd)));
	gef -= W[SF][GEF] * 1 / (1 + exp(VAL(sf)));
	gef /= W[SE][GEF] + W[SD][GEF] - W[SE][GEF];

	// calcula o valor do conceito get
	get = W[SD][GDT] * 1 / (1 + exp(VAL(sd)));
	get += W[SF][GDT] * 1 / (1 + exp(VAL(sf)));
	get /= W[SD][GDT] + W[SF][GDT];
	get = 1 - get;

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
