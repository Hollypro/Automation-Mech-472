
#include <iostream>
#include <cmath>
#include <conio.h>

using namespace std;

// include this header file for basic image transfer functions
#include "image_transfer2.h"

// include this header file for computer vision functions
#include "vision.h"

#include "timer.h"

int main(int argc, char* argv[])
{  
	int camera;
	int k;
	ibyte *p; // a pointer to a single byte
	ibyte R, G, B, grey; // 0 - 255 
	// -- if you add 50 to 245 it will roll
	char ch;

	// set camera (normally 0 or 1)
	camera = 0;

	// initialize vision library
	activate_vision(camera);

	cout << "\npress a key to acquire image\n";
	getch();

	image rgb, a, b;

	rgb.width = 640;
	rgb.height = 480;
	rgb.type = RGB_IMAGE;

	a.width = 640;
	a.height = 480;
	a.type = GREY_IMAGE;

	b.width = 640;
	b.height = 480;
	b.type = GREY_IMAGE;

	// allocate dynamic memory for the image
	// one big 1D dynamic array
	allocate_image(rgb);
	allocate_image(a);
	allocate_image(b);

	cout << "\npress x key to end program";

	while(1) {
		// get a RGB image with 3 bytes per pixel
		acquire_image(rgb);
	
		// process image
		p = rgb.pdata; // get image pointer
		for(k = 0; k < 640*480; k++) { // access each pixel k

			//  B G R
			B = *p;
			G = *(p+1);
			R = *(p+2);

			
			
			if( (B > 100) && (R < 100) && (G < 100) ) { 
				R = 0;
				G = 0;
				B = 255;
			}
			else if ((B < 100) && (R > 100) && (G < 100)) {
				R = 255;
				G = 0;
				B = 0;
			}
			else if ((B < 100) && (R < 100) && (G > 100)) {
				R = 0;
				G = 255;
				B = 0;
			}
			else {
				R = 0.587 * G;
				G *= 0.587;
				B = G;
			}

			*p = B; 
			*(p+1) = G;
			*(p+2) = R;

			// go to next pixel
			p += 3;
		}

		//copy(rgb, a);
		//scale(a, b);
		//convolution(a, b, , 60);
		//scale(a, b);
		//copy(b, rgb);

		view_image(rgb); // good for debugging but it takes a bit of time
			
		// press 'x' key to end program
		if(kbhit()) {
			ch = getch();
			if(ch == 'x') break;
		}
	}

	// delete the dynamic memory
	free_image(rgb);
	free_image(a);
	free_image(b);

	// this is important for the eye 3
	deactivate_vision();

	cout << "\n\ndone.\n";
	getch();

 	return 0; // no errors
}

