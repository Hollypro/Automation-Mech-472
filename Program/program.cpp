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
	char ch = 'a';
	int swap = 0;
	camera nozzle, front;

	nozzle.num = 1; //calibrate
	front.num = 0; //calibrate

	setup(nozzle);
	setup(front);

	cout<<("\npress x to exit\n");

	while(1) 
	{
//		acquire_image(nozzle.rgb, nozzle_camera);
		load_rgb_image("a.bmp", nozzle.rgb); //helps for troubleshooting

		if (swap == 0)
		{
			copy(nozzle.rgb, nozzle.grey); //need grey for find_centroid
			find_centroid(nozzle.grey, nozzle.label, nozzle.ic, nozzle.jc);
			copy(nozzle.grey, nozzle.rgb); //need rgb for view
		}
		else
		{
			find_edge(nozzle.rgb);
		}
		view_rgb_image(nozzle.rgb);

		// press 'x' key to end program
		if (kbhit()) 
		{
			swap++;
			if (swap > 1)swap = 0;
			ch = getch();
			if (ch == 'x') break;
		}
	}

	shutdown(front);
	shutdown(nozzle);
	
	cout<<("\n\ndone.\n");
	getch();

 	return 0; // no errors
}

