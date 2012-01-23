
#include "fuzzfier.h"
#include <iostream>


class fuzzy_obstacle
{
 private:

 public:
 	float center_x;
	float center_y;
   	short int conic;
	trapezoid_category x_axe;
	trapezoid_category y_axe;
        float alpha;
		
	fuzzy_obstacle();
	fuzzy_obstacle(float xmin, float ymin, float xmax, float ymax);
	~fuzzy_obstacle();
	void set_x_category(float xmin,float xcenter,float xmax);
	void set_y_category(float ymin,float ycenter,float ymax);
	void set_x_category(float xmin,float xcenterlow, float xcenterhigh, float xmax);
	void set_y_category(float ymin,float ycenterlow, float ycenterhigh, float ymax);
	void set_category(float x,float y, int distance);
        void set_angle(float alp){alpha = alp;};

	float getshare(float x, float y);
	float getcenter_x() { return(center_x);};
	float getcenter_y() { return(center_y);};
        float getlow_x() { return(x_axe.getlowval());};
        float getlow_y() { return(y_axe.getlowval());};
        float gethigh_x() { return(x_axe.gethighval());};
        float gethigh_y() { return(y_axe.gethighval());};
        float get_alpha() { return(alpha);};
   	fuzzy_obstacle(const fuzzy_obstacle &fo);
   	fuzzy_obstacle &operator=(const fuzzy_obstacle &fo);
   	int operator==(const fuzzy_obstacle &fo);
   	int operator!=(const fuzzy_obstacle &fo);
	//ostream& operator<<(ostream& fluxo, fuzzy_obstacle fo);

};
