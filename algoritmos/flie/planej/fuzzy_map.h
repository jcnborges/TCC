#include "../fuzzfier.h" 
#include "../fuzzy_obstacle.h"
#include "../../user/ponto.h" 
#include "ga/GAList.h" 

//#include "context.h" 
//#include "world.h"

/* defincao do modelo do ambiente - mapa */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "include.h"
//#include "mapa.h" 


#define TRUE 1   //constantes true e false
#define FALSE 0
#define DEBUG 0 //Executa Debug

class fuzzy_map
{

private:
	GAList<fuzzy_obstacle> Map;
	GAList<fuzzy_obstacle> NoBorderMap;

public:
   	void Exec_Debug(char mensag[30]);
	fuzzy_map();
	~fuzzy_map();
	void insert_obstacle(fuzzy_obstacle newobstacle);
	void reduce_map();
	void Init_Map();
	void Build_Map();
        void Print_Map();
	
};