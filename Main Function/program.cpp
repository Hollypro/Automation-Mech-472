
#include <iostream>
#include <cmath>
#include <conio.h>
#include "timer.h"
#include "image_transfer2.h" //custom functions and basic image transfer functions
#include "vision.h" //computer vision functions

#include "global_variables.h" //global variables declared

using namespace std;

int main(int argc, char* argv[])
{  
	char ch;
	
	setup();
	
	while(1) {
		// get a RGB image with 3 bytes per pixel
		acquire_image(rgb_nozzle);
	
		rgb_detection(rgb_nozzle, rgb_nozzle); //must calibrate detection range in function

		view_image(rgb_nozzle); // good for debugging but it takes a bit of time
			
		// press 'x' key to end program
		if(kbhit()) {
			ch = getch();
			if(ch == 'x') break;
		}
	}

	shutdown();

	cout << "\n\ndone.\n";
	getch();

 	return 0; // no errors
}

