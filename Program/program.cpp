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
	bool live;
	int i, swap = 0;
	double xyz[3];
	camera nozzle, front;
	gcode printer;

	// MUST be set before setup() called
	nozzle.num = 0; //calibrate
	front.num = 1; //calibrate

	setup_camera(nozzle);
//	setup_camera(front);

	calibrate_camera(nozzle, printer);
	calibrate_position(nozzle, printer);
	system ("CLS");

	while (1)
	{
		cout << "Choose camera view\n\tlive\t\t'l'\n\timage'a.bmp'\t'i'\n";
		ch = _getch();
		if (ch == 'l')
		{
			live = 1;
			break;
		}
		if (ch == 'i')
		{
			live = 0;
			break;
		}
		cout << "'l' and 'i' are only available options\n\n";
	}
	system("CLS");
	while (1)
	{
		cout << "what would you like to do?\n\tCentroid 'c'\n\tEdge 'e'\n";
		ch = _getch();
		if (ch == 'c')
		{
			swap = 0;
			break;
		}
		if (ch == 'e')
		{
			swap = 1;
			break;
		}
		cout << "'c' and 'e' are only available options\n\n";
	}
	system("CLS");
	while(1) 
	{
	
		setup_printer(printer);

		if (live == 0) get_image(nozzle, "a.bmp"); //if no file name specified gets live feed
		if (live == 1) get_image(nozzle);

		find_centroid(nozzle); //initializes nlabels

		if (swap == 0) //find and mark centroids
		{

			for (i = 1; i <= nozzle.nlabels; i++)
			{
				

				xyz[0] = gcode_convert(nozzle.ic[i]); //x
				xyz[1] = gcode_convert(nozzle.jc[i]); //y
				xyz[2] = printer.Get_Z(); //z
				printer.Set_Position(xyz); //send gcode

				printer.Move_Down(5); //make mark
				printer.Move_Up(5); // lift up to move
			}
		}
		else //edge detect and mark edge
		{

			if (live == 0) get_image(nozzle, "a.bmp"); //if no file name specified gets live feed
			if (live == 1) get_image(nozzle);

			find_edge(nozzle);
			for (int i = 1; i <= nozzle.nlabels; i++)
			{
				trace_object(printer, nozzle, i);
			}
		}

		view_rgb_image(nozzle.rgb);
		system("CLS");

		while (1)
		{
			cout << "\nOptions:\n  'x' exit\n  'c' mark centroid\n  'e' mark edge\r";
			ch = _getch();
			if (ch == 'c')
			{
				swap = 0;
				break;
			}
			if (ch == 'e')
			{
				swap = 1;
				break;
			}
			if (ch == 'x')
			{
				swap = 3;
				break;
			}
		}
		if (swap == 3) break;
	}

//	shutdown_camera(front);
	shutdown_camera(nozzle);
	
	cout<<("\n\ndone.\n");
	_getch();

 	return 0; // no errors
}

