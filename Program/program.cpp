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
#include "global_variables.h"
#include "gcode.h"

using namespace std;

int main(int argc, char* argv[])
{  
	char ch = 'a';
	int i, swap = 0;
	double xyz[3];
	camera nozzle, front;
	gcode printer;

	nozzle.num = 0; //calibrate
	front.num = 1; //calibrate

	setup(nozzle);
//	setup(front);

	cout<<("\npress x to exit\n");

	while(1) 
	{
		get_image(nozzle, "a.bmp"); //if no file name specified gets live feed
//		get_image(nozzle);


		if (swap == 0) // mark centroids
		{
			find_centroid(nozzle);
			
			xyz[0] = printer.Get_X(); //x
			xyz[1] = printer.Get_Y(); //y
			xyz[2] = part_height; //calibrate to part height

			if (xyz[2] == 0.5 || xyz[2] == 1) //reminder incase we forget to change into gcode
			{								// so that printer is not run too low
				cout << "ERROR: program--part_height not converted to gcode\n";
				cout << "Press c to continue anyway\n";
				ch = getch();
				if (ch != 'c') return 1; //exits program
			}
			printer.Set_Position(xyz);

			printer.Move_Up(1); //lift above part height for movement

			for (i = 1; i <= nozzle.nlabels; i++)
			{
				

				xyz[0] = gcode_convert(nozzle.ic[i]); //x
				xyz[1] = gcode_convert(nozzle.jc[i]); //y
				xyz[2] = printer.Get_Z(); //z
				printer.Set_Position(xyz); //send gcode

				printer.Move_Down(1); //make mark
				printer.Move_Up(1); // lift up to move
			}
		}
		else
		{
			find_edge(nozzle);
			for (int i = 1; i <= nozzle.nlabels; i++)
			{
				trace_object(printer, nozzle, i);
			}
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

//	shutdown(front);
	shutdown(nozzle);
	
	cout<<("\n\ndone.\n");
	getch();

 	return 0; // no errors
}

