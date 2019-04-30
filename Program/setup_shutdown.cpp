#include <iostream>
#include <cmath>
#include <conio.h>
#include <Windows.h>
#include "timer.h"
#include "global_variables.h"
#include "image_transfer3.h"
#include "gcode.h"


using namespace std;

unsigned int num_cams = 0; //number of open cams

int setup_printer(gcode printer)
{
	double xyz[3];

	xyz[0] = printer.Get_X(); //x
	xyz[1] = printer.Get_Y(); //y
	xyz[2] = part_height + pen_z_offset; //calibrate to part height

	printer.Set_Position(xyz);

	printer.Move_Up(1); //lift above part height for movement

	return 0;
}

int setup_camera(camera &cam)
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

int shutdown_camera(camera &cam)
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

int calibrate_camera(camera &cam, gcode printer)
{
	char ch;
	int i;
	ibyte *p;
	double xyz[3];

	printer.Centre();
	xyz[0] = printer.Get_X();
	xyz[1] = printer.Get_Y();
	xyz[2] = full_vision_height;
	printer.Set_Position(xyz);

	cout << "rotate camera until lines match gridlines\n";
	cout<<"press 'c' when done\n";

	while (1)
	{
		get_image(cam);

		p = cam.rgb.pdata + cam.rgb.width*cam.rgb.height * 2;
		for (i = 0; i < cam.rgb.width; i++) //horizontal line
		{
			*p = 255; //B
			*(p + 1) = 0; //G
			*(p + 2) = 0; //R
			p += 3;
		}

		p = cam.rgb.pdata + cam.rgb.width - 1;
		for (i = 0; i < cam.rgb.height; i++) //verticle line
		{
			*p = 255; //B
			*(p + 1) = 0; //G
			*(p + 2) = 0; //R
			p += cam.rgb.width * 3;
		}
		view_rgb_image(cam.rgb);
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'c') break;
		}
	}


	return 0;
}