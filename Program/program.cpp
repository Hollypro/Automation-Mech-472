#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <Windows.h>
#include "timer.h"
#include "image_transfer3.h"
#include "vision.h"
#include "timer.h"
#include"global_variables.h"

using namespace std;

int main(int argc, char* argv[])
{  
	char ch;

	setup();

	cout<<("\npress x to exit\n");

	while(1) {

		acquire_image(rgb_nozzle, nozzle_camera);
//		load_rgb_image("a.bmp", rgb_nozzle); //helps for troubleshooting
		sobel(rgb_nozzle);
		view_rgb_image(rgb_nozzle);

		// press 'x' key to end program
		if (kbhit()) {
			ch = getch();
			if (ch == 'x') break;
		}
	}

	shutdown();
	
	cout<<("\n\ndone.\n");
	getch();

 	return 0; // no errors
}

