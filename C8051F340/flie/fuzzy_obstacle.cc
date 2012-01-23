#include "flie.h"
#include "fuzzy_obstacle.h"
#include <math.h>

fuzzy_obstacle::fuzzy_obstacle()
{
 center_x = 0.0;
 center_y = 0.0;
 conic = 0; // Not conic fuzzy obstacle (if conic==1, it's conic!)
 x_axe.setval(0.0, 0.0, 0.0, 0.0);
 y_axe.setval(0.0, 0.0, 0.0, 0.0);

}

fuzzy_obstacle::fuzzy_obstacle(float xmin, float ymin, float xmax, float ymax)
{
 center_x = xmin+((xmax-xmin)/2.0);
 center_y = ymin+((ymax-ymin)/2.0);
 conic = 0; // Not conic fuzzy obstacle (if conic==1, it's conic!)
 x_axe.setval(xmin, center_x, center_x, xmax);
 y_axe.setval(ymin, center_y, center_y, ymax);

}

fuzzy_obstacle::~fuzzy_obstacle()
{

}

void fuzzy_obstacle::set_x_category(float xmin, float xcenter, float xmax)
{
 center_x = xcenter;
 x_axe.setval(xmin, xcenter, xcenter, xmax);
}

void fuzzy_obstacle::set_x_category(float xmin, float xcenterlow, float xcenterhigh, float xmax)
{
// center_x = xcenter;
 x_axe.setval(xmin, xcenterlow, xcenterhigh, xmax);
}

void fuzzy_obstacle::set_y_category(float ymin, float ycenter, float ymax)
{
 center_y = ycenter;
 y_axe.setval(ymin, ycenter,ycenter, ymax);

}
void fuzzy_obstacle::set_y_category(float ymin, float ycenterlow, float ycenterhigh, float ymax)
{
// center_y = ycenter;
 y_axe.setval(ymin, ycenterlow,ycenterhigh, ymax);

}

void fuzzy_obstacle::set_category(float x, float y, int distance)
{
  center_x = x;
  center_y = y;
  conic = 1;
  x_axe.setval(((x-distance)>0?(x-distance):0), x, x, x+distance);
  y_axe.setval(((y-distance)>0?(y-distance):0), y, y, y+distance);
}

float fuzzy_obstacle::getshare(float x, float y)
{
 float valx=0.0; float valy=0.0;
 float result=0.0;
 
 if(!conic) // NOT Conic: piramidal or trapezoidal obstacle
 {
  valx = x_axe.getshare(x);
//  cout << "Share de X= " << valx << endl;
  valy = y_axe.getshare(y);
//  cout << "Share de Y= " << valy << endl;
  result =  tnorm(valx,valy);
//  cout << "Share do obstaculo fuzzy= " << result << endl;
  return(tnorm(valx,valy));
 }
 else // Conic Obstacle!
 {
  return (hypot( (x-center_x),(y-center_y) )); 
 } 

}

fuzzy_obstacle::fuzzy_obstacle(const fuzzy_obstacle &fo)
{
 center_x = fo.center_x; 
 center_y = fo.center_y;
 conic = fo.conic;
 x_axe = fo.x_axe;
 y_axe = fo.y_axe;
}

fuzzy_obstacle &fuzzy_obstacle::operator=(const fuzzy_obstacle &fo)
{
 center_x = fo.center_x;
 center_y = fo.center_y;
 conic = fo.conic;
 x_axe = fo.x_axe;
 y_axe = fo.y_axe;
 return *this;
}

int fuzzy_obstacle::operator==(const fuzzy_obstacle &fo)
{
// return( (x==pt.x) && (y==pt.y) );
 return( (center_x==fo.center_x) && (center_y==fo.center_y) && 
         (x_axe == fo.x_axe) && (y_axe == fo.y_axe) );
// If COnic, different tests! And I've to implement comparison between trapezoid categories!!!
}

int fuzzy_obstacle::operator!=(const fuzzy_obstacle &fo)
{
 return( (center_x!=fo.center_x) || (center_y!=fo.center_y) || 
         (x_axe != fo.x_axe) || (y_axe != fo.y_axe) );
// If COnic, different tests! And I've to implement comparison between trapezoid categories!!!
}
/* MODIFICADO
ostream& operator<<(ostream& fluxo, fuzzy_obstacle fo)
{
   cout << "center_x = " << fo.center_x << ", center_y = " << fo.center_y << endl;
   cout << "category_x = " << fo.x_axe;
   cout << "category_y = " << fo.y_axe << endl;
   return(fluxo);
}
*/

