#include <iostream>
#include <cmath>
#include <conio.h>
#include <Windows.h>
#include "timer.h"
#include "global_variables.h"
#include "image_transfer3.h"

using namespace std;

unsigned int num_cams = 0; //number of open cams

int setup(camera &cam)
{
	if (num_cams == 0) //turning on first cam
	{
		//ShellExecute(NULL, "open", "C:\\Users\\Curtis\\Desktop\\School\\Mech 472\\mech663_lecture12\\computer_vision\\gcode_examples_new_vision_lib\\send_G_code_example3_keyboard_control\\image_view.exe", NULL, NULL, SW_SHOWDEFAULT);
		ShellExecute(NULL, "open", "image_view.exe", NULL, NULL, SW_SHOWDEFAULT);
	}

	activate_camera(cam.num, height, width);

	cam.rgb.width = width;
	cam.rgb.height = height;
	cam.rgb.type = RGB_IMAGE;

	cam.grey.width = width;
	cam.grey.height = height;
	cam.grey.type = GREY_IMAGE;

	cam.label.width = width;
	cam.label.height = height;
	cam.label.type = LABEL_IMAGE;

	allocate_image(cam.rgb);
	allocate_image(cam.grey);
	allocate_image(cam.label);
	

	num_cams++;
	return 0;
}

int shutdown(camera &cam)
{
	//error checks
	if (num_cams == 0)
	{
		cout << "ERROR:shutdown--shutdown when no cams open\n";
		return 1;
	}

	free_image(cam.rgb);
	free_image(cam.grey);
	free_image(cam.label);

	if (num_cams == 1) //turning off last cam
	{
		deactivate_cameras();
		system("TASKKILL /F /IM image_view.exe 1>NULL");
	}

	num_cams--;
	return 0;
}