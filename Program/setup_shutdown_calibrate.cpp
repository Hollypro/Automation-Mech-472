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
	xyz[2] = part_height + pen_z_offset + 5; //calibrate 5mm above part height

	printer.Set_Position(xyz);

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

int calibrate_camera(camera cam, gcode printer)
//allows camera to be rotated so camera axis align with printer axis
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

int calibrate_position(camera cam, gcode printer)
//used for initial determination of full_vision_height and pen offsets
{

	double xyz[3];
	bool done = FALSE;

	printer.Centre();
	cout << "Legend:\n\nMove Z\n";
	cout << "\t'o' - raise 1mm\n\t'u' - lower 1mm\n";
	cout << "\t'e' - raise 0.1mm\n\t'q' - lower 0.1mm\n";
	cout << "Move X\n";
	cout << "\t'l' - increase X by 1mm\n\t'j' - decrease X by 1mm\n";
	cout << "\t'd' - increase X by 0.1mm\n\t'a' - decrease X by 0.1mm\n";
	cout << "Move Y\n";
	cout << "\t'i' - increase Y by 1mm\n\t'k' - decrease Y by 1mm\n";
	cout << "\t'w' - increase Y by 0.1mm\n\t's' - decrease Y by 0.1mm\n";
	cout << "'x' - exit\n\n";
	
	xyz[0] = printer.Get_X();
	xyz[1] = printer.Get_Y();
	xyz[2] = printer.Get_Z();

	while (done!=TRUE)
	{
		printer.Set_Position(xyz);
		get_image(cam);
		view_rgb_image(cam.rgb);
		cout << "X = " << xyz[0] << " Y = " << xyz[1] << " Z = " << xyz[2] << "               \r"; //need spaces to ensure full line rewritten

		switch (_getch())
		{
		//Z movement
		case 'o':
			xyz[2] = xyz[2] + 1.0;
			break;
		case 'u':
			xyz[2] = xyz[2] - 1.0;
			break;
		case 'e':
			xyz[2] = xyz[2] + 0.1;
			break;
		case 'q':
			xyz[2] = xyz[2] - 0.1;
			break;
		//X movement
		case 'l':
			xyz[0] = xyz[0] + 1.0;
			break;
		case 'j':
			xyz[0] = xyz[0] - 1.0;
			break;
		case 'd':
			xyz[0] = xyz[0] + 0.1;
			break;
		case 'a':
			xyz[0] = xyz[0] - 0.1;
			break;
		//Y movement
		case 'i':
			xyz[1] = xyz[1] + 1.0;
			break;
		case 'k':
			xyz[1] = xyz[1] - 1.0;
			break;
		case 'w':
			xyz[1] = xyz[1] + 0.1;
			break;
		case 's':
			xyz[1] = xyz[1] - 0.1;
			break;
		//exit
		case 'x':
			done = TRUE;
			break;
		}
	}

	return 0;
}