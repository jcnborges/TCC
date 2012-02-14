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

void init_W(float sf, float se, float sd)
{
	if (sf > LIMIAR)
	{
		W[SD][GDF] = FORTE; // w1 (+) (f2) - direita perto gira direita frente
		W[SD][GEF] = FRACA; // w2 (+) (f1) - direita longe gira esquerda frente
		W[SD][GET] = FORTE; // w3 (+) (f2) - direita perto gira esquerda tras
		
		W[SE][GEF] = FORTE; // w4 (+) (f2) - esquerda perto gira esquerda frente
		W[SE][GDF] = FRACA; // w5 (+) (f1) - esquerda longe gira direita frente
		W[SE][GDT] = FORTE; // w6 (+) (f2) - esquerda perto gira direita tras
		
		W[SF][GDF] = MEDIA; // w7 (+) (f1) - frontal longe gira direita frente 
		W[SF][GDT] = MEDIA; // w8 (+) (f2) - frontal perto gira direita tras
		W[SF][GEF] = MEDIA; // w9 (+) (f1) - frontal longe gira esquerda frente
		W[SF][GET] = MEDIA; // w10 (+) (f2) - frontal perto gira esquerda tras

		W[SD][GDT] = -FRACA; // w15 (-) (f2) - direita perto evita direita tras
		W[SE][GET] = -FRACA; // w16 (-) (f2) - esquerda perto evita esquerda tras
	}
	else
	{	
		if (se > sd)
		{
			W[SD][GDF] = FORTE; // w1 (+) (f2) - direita perto gira direita frente
			W[SD][GEF] = FRACA; // w2 (+) (f1) - direita longe gira esquerda frente
			W[SD][GET] = FORTE; // w3 (+) (f2) - direita perto gira esquerda tras
			
			W[SE][GEF] = FRACA; // w4 (+) (f2) - esquerda perto gira esquerda frente
			W[SE][GDF] = FORTE; // w5 (+) (f1) - esquerda longe gira direita frente
			W[SE][GDT] = FRACA; // w6 (+) (f2) - esquerda perto gira direita tras
			
			W[SF][GDF] = FORTE; // w7 (+) (f1) - frontal longe gira direita frente 
			W[SF][GDT] = FRACA; // w8 (+) (f2) - frontal perto gira direita tras
			W[SF][GEF] = FRACA; // w9 (+) (f1) - frontal longe gira esquerda frente
			W[SF][GET] = FORTE; // w10 (+) (f2) - frontal perto gira esquerda tras

			W[SD][GDT] = -FORTE; // w15 (-) (f2) - direita perto evita direita tras
			W[SE][GET] = -FRACA; // w16 (-) (f2) - esquerda perto evita esquerda tras
		} else
		{
			W[SD][GDF] = FRACA; // w1 (+) (f2) - direita perto gira direita frente
			W[SD][GEF] = FORTE; // w2 (+) (f1) - direita longe gira esquerda frente
			W[SD][GET] = FRACA; // w3 (+) (f2) - direita perto gira esquerda tras
			
			W[SE][GEF] = FORTE; // w4 (+) (f2) - esquerda perto gira esquerda frente
			W[SE][GDF] = FRACA; // w5 (+) (f1) - esquerda longe gira direita frente
			W[SE][GDT] = FORTE; // w6 (+) (f2) - esquerda perto gira direita tras
			
			W[SF][GDF] = FRACA; // w7 (+) (f1) - frontal longe gira direita frente 
			W[SF][GDT] = FORTE; // w8 (+) (f2) - frontal perto gira direita tras
			W[SF][GEF] = FORTE; // w9 (+) (f1) - frontal longe gira esquerda frente
			W[SF][GET] = FRACA; // w10 (+) (f2) - frontal perto gira esquerda tras

			W[SD][GDT] = -FRACA; // w15 (-) (f2) - direita perto evita direita tras
			W[SE][GET] = -FORTE; // w16 (-) (f2) - esquerda perto evita esquerda tras		
		}
	}
	W[GDF][RD_Out] = FORTE; // w11 (+) FORTE 
	W[GDT][RD_Out] = -FORTE; // w12 (-) FORTE
	W[GEF][RE_Out] = FORTE; // w13 (+) FORTE
	W[GET][RE_Out] = -FORTE; // w14 (-) FORTE

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
	init_W(sf, se, sd);

	// calcula o valor do conceito gdf
	gdf = W[SD][GDF] * (1 - SIG(sd));
	gdf += W[SF][GDF] * SIG(sf);
	gdf += W[SE][GDF] *  SIG(se);
	gdf /= W[SD][GDF] + W[SF][GDF] + W[SE][GDF];
	gdf = gdf > 0 ? gdf : 0;

	// calcula o valor do conceito gdt
	gdt = W[SE][GDT] * (1 - SIG(se));
	gdt += W[SF][GDT] * (1 - SIG(sf));
	gdt += W[SD][GDT] * (1 - SIG(sd));
	gdt /= W[SE][GDT] + W[SF][GDT] + W[SD][GDT];

	// calcula o valor do conceito gef
	gef = W[SE][GEF] * (1 - SIG(se));
	gef += W[SF][GEF] * SIG(sf);
	gef += W[SD][GEF] * SIG(sd);
	gef /= W[SE][GEF] + W[SF][GEF] + W[SD][GEF];
	gef = gef > 0 ? gef : 0;

	// calcula o valor do conceito get
	get = W[SD][GET] * (1 - SIG(sd));
	get += W[SF][GET] * (1 - SIG(sf));
	get += W[SE][GET] * (1 - SIG(se));
	get /= W[SD][GET] + W[SF][GET] + W[SE][GET];

	printf("gdf = %.5lf\ngef = %.5lf\n", gdf, gef);
	// calcula o valor do conceito rd_out
	rd_out = W[GDF][RD_Out] * gdf;
	rd_out += W[GDT][RD_Out] * gdt;
	rd_out /= FORTE;
	rd_out *= 100;

	// calcula o valor do conceito re_out
	re_out = W[GEF][RE_Out] * gef;
	re_out += W[GET][RE_Out] * get;
	re_out /= FORTE;
	re_out *= 100;
}
