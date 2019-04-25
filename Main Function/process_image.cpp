
#include <iostream>
#include <cmath>
#include <conio.h>

using namespace std;

#include "image_transfer2.h"
#include "vision.h"
#include "timer.h"
#include "global_variables.h"

// input rgb image B-G-R-A
extern image rgb_in, rgb_copy;

// sample time of current incoming image
extern double t_sample;

int activate_process()
// process image initialization
// executed at program startup before image acquisition takes place
{


	return 0;
}


int process_image()
// this function is called continuously from the call back function
{
	return 0;
}

int rgb_detection(image rgb_in, image &rgb_out)
// detects RGB and grey everything else out
{
	ibyte *p;
	ibyte R, G, B;
	int k;

	// process image
	p = rgb_in.pdata; // get image pointer
	for (k = 0; k < 640 * 480; k++)  // access each pixel k
	{
		//  B G R
		B = *p;
		G = *(p + 1);
		R = *(p + 2);
		
		if ((B > 100) && (R < 100) && (G < 100)) 
		{
			R = 0;
			G = 0;
			B = 255;
		}
		else if ((B < 100) && (R > 100) && (G < 100)) 
		{
			R = 255;
			G = 0;
			B = 0;
		}
		else if ((B < 100) && (R < 100) && (G > 100)) 
		{
			R = 0;
			G = 255;
			B = 0;
		}
		else 
		{
			R = 0.587 * G;
			G *= 0.587;
			B = G;
		}

		*p = B;
		*(p + 1) = G;
		*(p + 2) = R;

		// go to next pixel
		p += 3;
	}
	copy(rgb_in, rgb_out);

	return 0;
}

int deactivate_process()
// deactivation code
{
	return 0;
}



