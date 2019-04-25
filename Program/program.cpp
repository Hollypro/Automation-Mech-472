#include <stdio.h>
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

//	ShellExecute(NULL, "open", "C:\\Users\\Curtis\\Desktop\\School\\Mech 472\\mech663_lecture12\\computer_vision\\gcode_examples_new_vision_lib\\send_G_code_example3_keyboard_control\\image_view.exe", NULL, NULL, SW_SHOWDEFAULT);
	ShellExecute(NULL, "open", "image_view.exe", NULL, NULL, SW_SHOWDEFAULT);

	setup();

	cout<<("\npress a key to acquire image\n");
	getch();

	while(1) {

		acquire_image(rgb_nozzle, nozzle_camera);
		view_rgb_image(rgb_nozzle);
		Sleep(500);

		acquire_image(rgb_front,front_camera);
		view_rgb_image(rgb_front);
		Sleep(500);

		// press 'x' key to end program
		if (kbhit()) {
			ch = getch();
			if (ch == 'x') break;
		}
	}

	shutdown();

	system("TASKKILL /F /IM image_view.exe 1>NULL");
	
	cout<<("\n\ndone.\n");
	getch();

 	return 0; // no errors
}

