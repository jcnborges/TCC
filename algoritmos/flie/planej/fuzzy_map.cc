#include "fuzzy_map.h" 

#include <sys/types.h> 
#include <sys/stat.h> 
 
#include "header.h" 
#include "sim.h" 
#include "world.h" 
#include "graphics.h" 

#include "user.h" // Definition of Ambiente!!
//#include "robot.h" 

#define BORDA 15
#define ALMOST 9

#ifdef FUZZYMAP
float Ambiente[1001][1001];
#else
short int Ambiente[1001][1001];
#endif


extern struct Context *context;

//short int Ambiente [1001][1001];

/*int main()
{
fuzzy_map M;

fuzzy_obstacle *fo;

   M.Init_Map();
   fo = new fuzzy_obstacle(0,0,10,10);   
   cout << "Obstaculo: " << *fo << endl;
   M.insert_obstacle(*fo);
   cout << "Mapa contem: " << endl;
   M.Print_Map();
   fo = new fuzzy_obstacle(10,10,20,20);   
   M.insert_obstacle(*fo);
   cout << "Mapa contem: " << endl;
   M.Print_Map();
//    delete fo;

}

*/ 

void fuzzy_map::Exec_Debug(char mensag[30])
{
  if (DEBUG == 1){
    printf("%s\n",mensag);
    fflush(stdout);
  }
}

fuzzy_map::fuzzy_map()
{
}

fuzzy_map::~fuzzy_map()
{
 while(Map.head()) Map.destroy(); // destroy the lists
 while(NoBorderMap.head()) NoBorderMap.destroy();
}

void fuzzy_map::Init_Map ()
{
  int i,j;
  Exec_Debug("Entrou no Init Map");
  
   for (i = 0; i < 1001; i++){
    for (j = 0; j < 1001; j++)
      Ambiente[j][i]  = 0.0;//zera coluna,linha do ambiente
     printf(".");
   }
  
  // ap_lista = NULL;

  while(Map.head()) Map.destroy(); // destroy any pre-existing list


  Exec_Debug("Saiu do Init Map"); 
}

void fuzzy_map::Build_Map()
// constroi o mapa do ambiente, buscando na lista de objetos os tijolos do mundo
{ 

  FILE *file;
  short int  i,j,k,l;
  struct World *world; 
  struct Robot *robot;
  struct Object *Obj;

  int initX, initY, endX, endY; //used by enlarged bricks(By JAF 24/09/2000)

  fuzzy_obstacle *newobstacle;
  
  Exec_Debug("Entrou no Mapeamento"); 
  world = context->World;
  robot = context->Robot;
  Obj = world->Objects;
  while (Obj != NULL)
    {
      if (Obj->Type == LAMP)
	{ //se eh lampada descarta
	  Exec_Debug("Eh lampada!"); 
	  Obj=Obj->Next;
	}
      else
	if (((Obj->Alpha*180/M_PI) == 0) || ((Obj->Alpha*180/M_PI) == 180))
	  {	//tijolo horizontal
	    	Exec_Debug("tijolo horizontal");
             initX = Obj->X -25 - BORDA;
	     if(initX<0) initX = 0;
             endX = Obj->X + 25 + BORDA;
	     if(endX>1000) endX = 1000;
             initY = Obj->Y -10 - BORDA;
	     if(initY<0) initY = 0;
             endY = Obj->Y + 10 + BORDA;
	     if(endY>1000) endY = 1000;

     	     newobstacle = new fuzzy_obstacle(initX, initY, endX, endY);
             insert_obstacle(*newobstacle);
          
	     for ( k = initX; k <= endX; k++ )
	      for (l = initY; l <= endY; l++)
		 { Ambiente[l][k]+= newobstacle->getshare(k,l); 
                //  printf("Ambiente [ %d][ %d] = %f\n", l ,k, Ambiente[l][k]);}
              }
          
	    
//          getchar();
	  }
	else 
	  if (((Obj->Alpha*180/M_PI) == 90) || ((Obj->Alpha*180/M_PI) == 270))
	    {   //tijolo vertical
		Exec_Debug("tijolo vertical");
		initX = Obj->X -10 - BORDA;
	        if(initX<0) initX = 0;
                endX = Obj->X + 10 + BORDA;
	        if(endX>1000) endX = 1000;
                initY = Obj->Y -25 - BORDA;
	        if(initY<0) initY = 0;
                endY = Obj->Y + 25 + BORDA;
	        if(endY>1000) endY = 1000;
		  //Ambiente[l][k]= 1;
                newobstacle = new fuzzy_obstacle(initX, initY, endX, endY);
                insert_obstacle(*newobstacle);
	       for ( k = initX; k <= endX; k++ )
		 for ( l = initY; l <= endY; l++ )
		 {
		  Ambiente[l][k]+= newobstacle->getshare(k,l);
		  //printf("Ambiente [ %d][ %d] = %f\n", l ,k, Ambiente[l][k]);
		 }
	    

	     // scanf ("%c",&a);
	    }
	  else 
	    {
	      //tijolo enclinado
	      //calcula os pontos no ambiente - tras o tijolo para a posicao 0,0 do ambiente calcula os pontos do tijolo baseado no seno e no cosseno e mapeia para a posicao no ambiente
	      Exec_Debug("tijolo com angulo");
	    
	      fflush(stdout);
	    }
      // printf("Obj recebeu o proximo tijolo, %d %d\n", Obj->X, Obj->Y);
      Obj = Obj->Next;
    }
  Exec_Debug("Saiu do Mapeamento");
/* file = fopen("meumundofuzzy.world","w");
// Teste para ver se a matriz esta correta
  for (i = 0;i < 1001; i++){
   for(j = 0;j < 1001; j++){
//      if (((j % 4) == 0) && ((i % 4) == 0))
	fprintf(file,"%.2f ", Ambiente[i][j]);
        }
    
//   if ((i%4)==0) 
   fprintf(file,"fimcol\n");
  }
  Exec_Debug("Escreveu no arquivo");
  fclose(file); */ //Teste não mais necessário - JAF 09/08/2000
}

void fuzzy_map::reduce_map()
{

int DEBUGREDUCE=1;
fuzzy_obstacle *iterate=NULL;
fuzzy_obstacle *first=NULL;
fuzzy_obstacle *second=NULL;
fuzzy_obstacle *newobs=NULL;
fuzzy_obstacle *any=NULL;

FILE *file;
	
float NoBorderEnvironment[1001][1001];
int changed;
  int xlow, xcenter, xhigh, ylow,ycenter, yhigh;

int numfirst=0;
int numsecond=0;

int i,n;
int j,k,l;	
int flagfusion=1;

while(NoBorderMap.head()) NoBorderMap.destroy();

	iterate = Map.head();//NoBorderMap.head();
	n=Map.size();//NoBorderMap.size();
	for(i=0;i<n;i++)
	 {
/*		
	  if((iterate->getcenter_x() == 17.5) || (iterate->getcenter_x() == 982.0))
		{
		cout <<"Descartando tijolo " << i << " da borda horizontal!" << endl;
		}
	  else if((iterate->getcenter_y() == 19.5) || (iterate->getcenter_y() == 981.0))
		{
		cout << "Descartando tijolo " << i << " da borda vertical!" << endl;
		}
	  else
		{
*/
		  cout << "Obstaculo iterado " << i << " eh " << *iterate << endl;
		  if(first==NULL) { first = iterate;numfirst = i;cout << "Eh o primeiro"<< endl;}
		  else {second = iterate; numsecond = i;cout << "Eh o segundo!"<< endl;flagfusion=0;}
                  if(DEBUGREDUCE) getchar();
//		}

	 if ( (first!=NULL)&(second!=NULL) )
          if ((fabs(first->getcenter_y()-second->getcenter_y())<ALMOST) && (fabs(first->getlow_y()-second->getlow_y())<ALMOST) && (fabs(first->gethigh_y()-second->gethigh_y())<ALMOST ) )
           {
            if ( (first->gethigh_x() > second->getlow_x()) && (first->getcenter_x() < second->getcenter_x()) )
              {
               flagfusion=1;
               newobs=new fuzzy_obstacle(first->getlow_x(),first->getlow_y(),second->gethigh_x(),second->gethigh_y());
               cout << "1 Substituindo primeiro e segundo por : " << *newobs << endl;
               second = NULL;
               first = newobs;
               if(DEBUGREDUCE) getchar();
               for ( k=(int)newobs->getlow_x(); k <= (int)newobs->gethigh_x(); k++ )
		 for ( l = (int)newobs->getlow_y(); l <= (int)newobs->gethigh_y(); l++ )
		 {
		  Ambiente[l][k] = newobs->getshare(k,l);
		//printf("Ambiente [ %d][ %d] = %f\n", l ,k, Ambiente[l][k]);
		 }
              }
            else 
             if ((second->gethigh_x() > first->getlow_x()) && (first->getcenter_x() > second->getcenter_x()) )
              {
               flagfusion=1;
               newobs=new fuzzy_obstacle(second->getlow_x(),second->getlow_y(),first->gethigh_x(),first->gethigh_y());
               cout << "2 Substituindo primeiro e segundo por : " << *newobs << endl;
               second = NULL;
               first = newobs;              
               if(DEBUGREDUCE) getchar();
               for ( k=(int)newobs->getlow_x(); k <= (int)newobs->gethigh_x(); k++ )
		 for ( l = (int)newobs->getlow_y(); l <= (int)newobs->gethigh_y(); l++ )
		 {
		  Ambiente[l][k] = newobs->getshare(k,l);
		//printf("Ambiente [ %d][ %d] = %f\n", l ,k, Ambiente[l][k]);
		 }
               }
	    }
           else
            if ( (fabs(first->getcenter_x()-second->getcenter_x())<ALMOST) && (fabs(first->getlow_x()-second->getlow_x())<ALMOST) 
             && (fabs(first->gethigh_x()-second->gethigh_x())<ALMOST) ) 
             {
              if ( (first->gethigh_y() > second->getlow_y()) && (first->getcenter_y() <second->getcenter_y()) )
              {
               flagfusion=1;
               newobs=new fuzzy_obstacle(first->getlow_x(),first->getlow_y(),second->gethigh_x(),second->gethigh_y());
               cout << "3 Substituindo primeiro e segundo por : " << *newobs << endl;
               second = NULL;
               first = newobs;
               if(DEBUGREDUCE) getchar();
               for ( k = (int)newobs->getlow_x(); k <=  (int)newobs->gethigh_x(); k++ )
		 for ( l = (int)newobs->getlow_y(); l <= (int)newobs->gethigh_y(); l++ )
		 {
		  Ambiente[l][k] = newobs->getshare(k,l);
		//printf("Ambiente [ %d][ %d] = %f\n", l ,k, Ambiente[l][k]);
		 }

              }
            else 
             if ( (second->gethigh_y() > first->getlow_y()) && (first->getcenter_y() > second->getcenter_y()) )//high_y < low_y
              {
               flagfusion=1;
               newobs=new fuzzy_obstacle(second->getlow_x(),second->getlow_y(),first->gethigh_x(),first->gethigh_y());
               cout << "4 Substituindo primeiro e segundo por : " << *newobs << endl;
               second = NULL;
               first = newobs;
               if(DEBUGREDUCE) getchar();
               for (k= (int)newobs->getlow_x(); k <=  (int)newobs->gethigh_x(); k++ )
		 for ( l =  (int)newobs->getlow_y(); l <=  (int)newobs->gethigh_y(); l++ )
		 {
		  Ambiente[l][k] = newobs->getshare(k,l);
		//printf("Ambiente [ %d][ %d] = %f\n", l ,k, Ambiente[l][k]);
		 }

               }
             }
             
             
             if(flagfusion==0)
             {
              flagfusion = 0;
	      cout << " Obstaculos nao puderam ser fundidos !! Pegando proximo!!! " << endl;
	      if (newobs!=NULL)
		{cout << " Inserindo obstaculo : " << *newobs << " no novo mapa!!!" << endl;
		 }
	      else 
		{ 
		  newobs = new fuzzy_obstacle(*first);
		}

	  if((newobs->getcenter_x() == 17.5) || (newobs->getcenter_x() == 982.0))
		{
		cout <<"Descartando tijolo " << i << " da borda horizontal!" << endl;
		}
	  else if((newobs->getcenter_y() == 19.5) || (newobs->getcenter_y() == 981.0))
		{
		cout << "Descartando tijolo " << i << " da borda vertical!" << endl;
		}
	  else
		{
		  NoBorderMap.insert(*newobs);
                  cout << "inserido!!!" << endl;
                }
              first = second;
              second = NULL;
             }
	
          iterate = Map.next();//NoBorderMap.next();
		
	  }
//Inserindo ultimo obstaculo!!!
	      if (newobs!=NULL)
		{cout << " Inserindo obstaculo : " << *newobs << " no novo mapa!!!" << endl;
	        NoBorderMap.insert(*newobs);
		 }
	      else 
		{ 
		  newobs = new fuzzy_obstacle(*first);
		  NoBorderMap.insert(*newobs);
		}
               cout << "inserido!!!" << endl;

 file = fopen("meumundofuzzy.world","w");
// Teste para ver se a matriz esta correta
  for (i = 0;i < 1001; i++){
   for(j = 0;j < 1001; j++){
      if (((j % 4) == 0) && ((i % 4) == 0))
	fprintf(file,"%.2f ", Ambiente[i][j]);
        }
    
  if ((i%4)==0) 
   fprintf(file,"\n");
  }
  Exec_Debug("Escreveu no arquivo");
  fclose(file); 

	getchar();

 for (i = 0;i < 1001; i++)
   for(j = 0;j < 1001; j++)
	NoBorderEnvironment[i][j] = 0.0;

 iterate = NoBorderMap.head();
 n=NoBorderMap.size();
 for(i=0;i<n;i++)
  {
     for ( k = (int)iterate->getlow_x(); k <=  (int)iterate->gethigh_x(); k++ )
	 for ( l = (int)iterate->getlow_y(); l <= (int)iterate->gethigh_y(); l++ )
		 {
		  NoBorderEnvironment[l][k] = iterate->getshare(k,l);
		 }
	     iterate = NoBorderMap.next();
 	
   }



 iterate = NoBorderMap.head();
 n=NoBorderMap.size();

 cout << " Imprimindo Mapa antes do ajuste das bordas: Mapa com "  << n << " obstaculos!" <<endl;
 if(iterate != NULL) cout << "Obstaculo Fuzzy 0 = "<< *iterate << endl;
for(i=1;i<n;i++)
   {
    iterate = NoBorderMap.next();
    cout << "Obstaculo Fuzzy " << i << " = " << *iterate << endl;
   }

 // Procedure to adjust fuzzy obstacles relative to borders with other obstacles!!!13/10/2000 By JAF
 iterate = NoBorderMap.head();
 
 changed = 0;
 
 for(i=0;i<n;i++)
  {
        
     xlow = (int)iterate->getlow_x(); xcenter = (int)iterate->getcenter_x();xhigh = (int)iterate->gethigh_x();
     ylow = (int)iterate->getlow_y(); ycenter = (int)iterate->getcenter_y();yhigh = (int)iterate->gethigh_y();
     if(xlow==0) xlow=2;
     if(ylow==0) ylow=2;
     if(xhigh==1000) xhigh=998;
     if(yhigh==1000) yhigh=998;// Adjust to test only inside valid areas of data structure NoBorderEnvironment!!

     if( (Ambiente[ycenter][xlow-1]>0.0)&&(Ambiente[ycenter][xhigh+1]>0.0) )
       {
         cout << "Alterando obstaculo " << *iterate << " para: ";
          iterate->set_x_category(xlow, xlow+1,xhigh-1,xhigh);// Fuzzy obstacle degenerates to nonfuzzy!
          changed=1;
         cout <<  " para: "<< *iterate ;
          if(DEBUGREDUCE) getchar();
       }
     else
     if(Ambiente[ycenter][xlow-1]>0.0)
       {
         cout << "Alterando obstaculo " << *iterate << " para: ";
          iterate->set_x_category(xlow, xlow+1,xlow+1,xhigh);// Only one side touches other obstacle! Becames ramp!
          changed=1;
         cout <<  " para: "<< *iterate ;
         if(DEBUGREDUCE) getchar();
       }
     else
     if(Ambiente[ycenter][xhigh+1]>0.0)
       {
         cout << "Alterando obstaculo " << *iterate << " para: ";
          iterate->set_x_category(xlow, xhigh-1,xhigh-1,xhigh);// The other side touches obstacle! Becames ramp!
          changed=1;
         cout <<  " para: "<< *iterate ;
          if(DEBUGREDUCE) getchar();
       }
     if(changed==1)
       {
         changed=0;
         for (k= xlow; k <=  xhigh; k++ )
           for ( l =  ylow; l <=  yhigh; l++ )
             {
               NoBorderEnvironment[l][k] = iterate->getshare(k,l);
	//	  printf("Ambiente [ %d][ %d] = %f\n", l ,k, NoBorderEnvironment[l][k]);
              //    getchar();
             }
       }// End of if for X_category

     if( (Ambiente[ylow-1][xcenter]>0.0)&&(Ambiente[yhigh+1][xcenter]>0.0) )
       {
         cout << "Alterando obstaculo " << *iterate << " para: ";
          iterate->set_y_category(ylow, ylow+1,yhigh-1,yhigh);// Fuzzy obstacle degenerates to nonfuzzy!
          changed=1;
         cout <<  " para: "<< *iterate ;
          if(DEBUGREDUCE) getchar();
       }
     else
     if(Ambiente[ylow-1][xcenter]>0.0)
       {
         cout << "Alterando obstaculo " << *iterate << " para: ";
          iterate->set_y_category(ylow, ylow+1,ylow+1,yhigh);// Only one side touches other obstacle! Becames ramp!
          changed=1;
         cout <<  " para: "<< *iterate ;
          if(DEBUGREDUCE) getchar();
       }
     else
     if(Ambiente[yhigh+1][xcenter]>0.0)
       {
         cout << "Alterando obstaculo " << *iterate << " para: ";
          iterate->set_y_category(ylow, yhigh-1,yhigh-1,yhigh);// The other side touches obstacle! Becames ramp!
          changed=1;
         cout <<  " para: "<< *iterate ;
          if(DEBUGREDUCE) getchar();
       }
     if(changed==1)
       {
         changed=0;
         for (k= xlow; k <=  xhigh; k++ )
           for ( l =  ylow; l <=  yhigh; l++ )
             {
               NoBorderEnvironment[l][k] = iterate->getshare(k,l);
	//	  printf("Ambiente [ %d][ %d] = %f\n", l ,k, NoBorderEnvironment[l][k]);
              //    getchar();
             }
       }// End of if for Y_category


     iterate = NoBorderMap.next();
 	
   }

 file = fopen("novomundo.world","w");
  for (i = 0;i < 1001; i++){
   for(j = 0;j < 1001; j++){
      if (((j % 4) == 0) && ((i % 4) == 0))
	fprintf(file,"%.2f ", NoBorderEnvironment[i][j]);
        }
    
  if ((i%4)==0) 
   fprintf(file,"\n");
  }
  fclose(file); 	
// delete(&Map);
while(Map.head()) Map.destroy();
Map = NoBorderMap;
/*cout << "Imprimindo mapa depois do tratamento das bordas dos obstaculos" << endl;
getchar();
Print_Map();
getchar();
*/

}

void fuzzy_map::insert_obstacle(fuzzy_obstacle newobstacle)
{
	Map.insert(newobstacle);

}

int GAList<fuzzy_obstacle>::write(ostream & os) const
{
  fuzzy_obstacle *cur, *head;

  GAListIter<fuzzy_obstacle> tmpiter(*this);
  if((head=tmpiter.head()) != NULL) os << *head << " ";
  for(cur=tmpiter.next(); cur && cur != head; cur=tmpiter.next())
    os << *cur << " ";
  return os.fail() ? 1 : 0;
}

void fuzzy_map::Print_Map()
{
int i;
int tam;
fuzzy_obstacle *tmp;
tam = Map.size();
cout << " Mapa com "  << tam << " obstaculos!" <<endl;
tmp = Map.head();
if(tmp != NULL) cout << "Obstaculo Fuzzy 0 = "<< *tmp << endl;
for(i=1;i<tam;i++)
   {
    tmp = Map.next();
    cout << "Obstaculo Fuzzy " << i << " = " << *tmp << endl;
   }
  // cout << Map << endl;
}

/*               any = Map.warp(numfirst);
               
               cout << "Matando " << *any << endl;
               Map.destroy();
               any = Map.current();
               cout << "Matando " << *any << endl;
               Map.destroy();
               cout << "Inserindo no lugar " << *newobs << endl;
               Map.insert(*newobs);
               Map.prev();
               first = newobs;
               second = NULL;
*/

