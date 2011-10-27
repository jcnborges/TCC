#include <stdlib.h> // Random!
#include <iostream.h>
//#include "flie.h"

#include "fuzzy_obstacle.h"


fuzzy_obstacle obs1;
fuzzy_obstacle obs2;

void main()
{
 float share=0.0;
 
 obs1.set_x_category(0, 4, 8);
 obs1.set_y_category(0, 2, 4);
 share = obs1.getshare(4, 2);
 cout << "Pertinencia de (x,y) = (4,2) = " << share << endl;
 share = obs1.getshare(2, 1);
 cout << "Pertinencia de (x,y) = (2,1) = " << share << endl;
 share = obs1.getshare(3, 0);
 cout << "Pertinencia de (x,y) = (3,0) = " << share << endl;
 share = obs1.getshare(2,3);
 cout << "Pertinencia de (x,y) = (2,3) = " << share << endl;
 share = obs1.getshare(3, 2);
 cout << "Pertinencia de (x,y) = (3,2) = " << share << endl;
 

}


