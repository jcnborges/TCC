/* defincao do modelo do ambiente - mapa */

#ifndef MAPA_H
#define MAPA_H

struct ponto {
  int X;
  int Y;
};

struct lista_pontos {
  struct ponto P;
  struct lista_pontos *prox;
  struct lista_pontos *ant;
};

typedef struct ponto vetor;

void                   Init_Map (void);
void                   Mapeamento ();/* constroi o mapa do ambiente */
struct ponto           Reta (struct ponto P1, struct ponto P2, int fim, struct ponto V); /*traca uma reta (algoritmo de Bresenham)*/
void                   Preenche (struct ponto V1, struct ponto V2);
extern struct Object  *FindObject(struct World *w,short int x,short int y);
#endif









