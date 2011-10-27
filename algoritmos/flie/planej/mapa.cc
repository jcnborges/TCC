/* defincao do modelo do ambiente - mapa */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "include.h"
#include "world.h"
#include "mapa.h" 

#define TRUE 1   //constantes true e false
#define FALSE 0
#define DEBUG 0 //Executa Debug


//struct lista_pontos *atual;
//struct lista_pontos *ap_lista;

short int Ambiente [1001][1001];
char mensag[30]; 
char a[3];
 

void Executa_Debug(char mensag[30])
{
  if (DEBUG == 1){
    printf("%s\n",mensag);
    fflush(stdout);
  }
}

void Init_Map ()
{
  int i,j;
  Executa_Debug("Entrou no Init Map");
  for (i = 0; i < 1001; i++){
    for (j = 0; j < 1001; j++)
      Ambiente[j][i] = 0;/*zera coluna,linha do ambiente*/ 
  }
  // ap_lista = NULL;
  Executa_Debug("Saiu do Init Map"); 
}


struct ponto Reta (struct ponto P1, struct ponto P2, int fim, vetor *V)
  /* usa o algoritmo da reta de Bresenham, retorna o ponto final ou ponto de colisao*/
  /* tambem e utilizada pelo mapeamento para o caso de tijolos com angulo, a variavel fim indica para qual finalidade esta se tracando a reta -> se 0 entao e para mapeamento; se 1 e para retornar o ponto encontrado*/
{
  struct ponto P;
  int x, y, delta_x, delta_y, s1, s2, temp, erro = 0, k = 0;
  boolean interchange;
  
  Executa_Debug("Entrou no algoritmo da reta");
  x = P1.X;
  y = P1.Y;
  printf("\nponto P1 %d %d ", x, y);
  printf("\nponto P2 %d %d ", P2.X, P2.Y);
  fflush(stdout);
  delta_x = abs (P2.X - x);
  delta_y = abs (P2.Y - y);
  if ((P2.X - x) < 0) 
    s1 = -1;
  if ((P2.X - x) == 0) 
    s1 = 0;
  if ((P2.X - x) > 0) 
    s1 = 1;
  if ((P2.Y - y) < 0)
    s2 = -1;
  if ((P2.Y - y) == 0)
    s2 = 0;
  if ((P2.Y - y) > 0)
    s2 = 1;
  if (delta_y > delta_x){
    temp = delta_x;
    delta_x = delta_y;
    delta_y = temp;
    interchange = 1;
  }
  else 
    interchange = 0;
  erro = 2 * (delta_y - delta_x);
  for (int i = 1; i <= delta_x+1; i++){
    //printf("\nfim = %d", fim);
    //printf ("\nAmbiente [%d][%d] = %d",x,y,Ambiente[y][x]);
    fflush(stdout);
    if (fim == 0){ //traca a reta
      Ambiente[y][x] = 1;
      V[k].X = x;
      V[k].Y = y;
//      scanf("%c", &a[1]);
      k++;
    }
    else{
      int j = Ambiente [y][x]; //verifica o ponto no ambiente
      if (j == 1){
	printf("\nObstaculo encontrado pela reta %d  %d", x, y);
	fflush(stdout);
	P.X = x;
	P.Y = y;
	return (P); /* retorna o ponto onde bateu*/
      }
    }
    while (erro > 0){
      fflush(stdout);
      if (interchange == 1)
	x = x + s1;
      else
	y = y + s2;
      erro = (erro - (2 * delta_x));
    }
    if (interchange == 1)
      y = y + s2;
    else
      x = x + s1;
    erro = (erro + (2 * delta_y));
  }
  P.X = x;
  P.Y = y;
  return (P); /* retorna o ponto final*/
  Executa_Debug ("Passou pelo algoritmo da Reta");
}


void Preenche (vetor *V1, vetor *V2)
{
  /* Preenche todos os pontos ate encontrar a borda - baseado do algoritmo de preenchimento por saturacao - usado para preenchimento de tijolos com angulo*/
  int i,j;
  vetor v;
  
  Executa_Debug(" Entrou preenche");
  for (i = 0; i < 50; i++){
    Reta(V1[i],V2[i], 0,&v);
    printf("Preenchendo de %d, %d ate %d, %d\n", V1[i].Y, V1[i].X, V2[i].Y, V2[i].X);
    fflush(stdout);
  }
  Executa_Debug("Saiu Preenche");
  scanf("%c", &a[1]);
}


void Mapeamento() 
  /* constroi o mapa do ambiente, buscando na lista de objetos os tijolos do mundo*/
{ 
  FILE *file;
  short int  i,j,k,l;
  struct World *world; 
  struct Robot *robot;
  struct Object *Obj;
  int pontos [4][2];
  struct ponto P,P1,P2,P3,P4;
  vetor V1[55], V2[55];
  
  Executa_Debug("Entrou no Mapeamento"); 
  world = context->World;
  robot = context->Robot;
  Obj = world->Objects;
  while (Obj != NULL)
    {
      if (Obj->Type == LAMP)
	{ /*se eh lampada descarta*/
	  Executa_Debug("Eh lampada!"); 
//          printf("LAMPADA em X = %d ,Y = %d \n", Obj->X, Obj->Y); 
//          printf("ROBO em X = %f, Y = %f \n", robot->X, robot->Y);
//          getchar();
	  Obj=Obj->Next;
	}
      else
	if (((Obj->Alpha*180/M_PI) == 0) || ((Obj->Alpha*180/M_PI) == 180))
	  {//tijolo horizontal
	    for ( k = Obj->X -25; k <= Obj->X +25; k++ ){
	      for (l = Obj->Y -10; l <= Obj->Y +10; l++)
		Ambiente[l][k] = 1; 
	    }
	    Executa_Debug("tijolo horizontal");
	   // scanf ("%c",&a);
	  }
	else 
	  if (((Obj->Alpha*180/M_PI) == 90) || ((Obj->Alpha*180/M_PI) == 270))
	    {//tijolo vertical
	      for ( k = ((Obj->X) -10); k <= ((Obj->X) +10); k++ )
		for ( l = ((Obj->Y) -25); l <= ((Obj->Y) +25); l++ )
		  Ambiente[l][k]= 1;
	      Executa_Debug("tijolo vertical");
	     // scanf ("%c",&a);
	    }
	  else 
	    {
	      //tijolo enclinado
	      /*calcula os pontos no ambiente - tras o tijolo para a posicao 0,0 do ambiente calcula os pontos do tijolo baseado no seno e no cosseno e mapeia para a posicao no ambiente*/
	      Executa_Debug("tijolo com angulo");
	      pontos[0][0] = 25;
	      pontos[0][1] = 10; /*ponto x1*/
	      pontos[1][0] = 25;
	      pontos[1][1] = -10;/*ponto x2*/
	      pontos[2][0] = -25;
	      pontos[2][1] = -10;/*ponto x3*/
	      pontos[3][0] = -25;
	      pontos[3][1] = 10; /*ponto x4*/
	      //Obj->Alpha = 0; // testeeee
	      for (l = 0; l < 4; l++){
		double x_linha = (pontos[l][0] * cos(Obj->Alpha*180/M_PI) - pontos[l][1] * sin(Obj->Alpha*180/M_PI));
		double y_linha = (pontos[l][0] * sin(Obj->Alpha*180/M_PI) + pontos[l][1] * cos(Obj->Alpha*180/M_PI));
		printf("xlinha %lf, ylinha %lf", x_linha, y_linha);
		fflush(stdout);
		scanf("%c",&a[1]);
		pontos[l][0] = (int)(Obj->X + x_linha);
		pontos[l][1] = (int)(Obj->Y + y_linha);	
	      }	
	      Executa_Debug("Passou calculo dos pontos");;
	      /*faz o preenchimento da matriz de pontos*/
	      P1.X = pontos[0][0]; //joga os valores para variaveis do tipo ponto
	      P1.Y = pontos[0][1];
	      P2.X = pontos[1][0];
	      P2.Y = pontos[1][1];
	      P3.X = pontos[2][0];
	      P3.Y = pontos[2][1];
	      P4.X = pontos[3][0];
	      P4.Y = pontos[3][1];
	      printf("Pontos P1 = %d,%d\n P2 = %d,%d\n P3 = %d,%d\n,P4 = %d,%d", P1.X,P1.Y,P2.X,P2.Y,P3.X,P3.Y,P4.X,P4.Y);
	      fflush(stdout);
	      scanf("%c", &a[1]); 
	      Reta(P3,P2,0,V1); //traca retas entre os pontos 
	      Reta(P4,P1,0,V2);
	      Executa_Debug("Saiu - vai para a preenche");
	      Preenche(V1,V2); //preenche todo o espaco com 1's.
	    }
      // printf("Obj recebeu o proximo tijolo, %d %d\n", Obj->X, Obj->Y);
      Obj = Obj->Next;
    }
  Executa_Debug("Saiu do Mapeamento");
/* file = fopen("meumundo.world","w");
// Teste para ver se a matriz esta correta
  for (i = 0;i < 1001; i++){
   for(j = 0;j < 1001; j++){
//      if (((j % 4) == 0) && ((i % 4) == 0))
	fprintf(file,"%d", Ambiente[i][j]);
        }
    
//   if ((i%4)==0) 
   fprintf(file,"fimcol\n");
  }
  Executa_Debug("Escreveu no arquivo");
  fclose(file);*///Teste não mais necessário - JAF 09/08/2000
}
