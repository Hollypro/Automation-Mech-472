#include <iostream>
#include <cmath>
#include <conio.h>
//#include "image_transfer3.h"
//#include "vision.h"
#include "timer.h"
#include "global_variables.h"

using namespace std;

int setup()
{
	activate_camera(nozzle_camera, height, width);
	activate_camera(front_camera, height, width);

	rgb_nozzle.width = width;
	rgb_nozzle.height = height;
	rgb_nozzle.type = RGB_IMAGE;

	rgb_front.width = width;
	rgb_front.height = height;
	rgb_front.type = RGB_IMAGE;

	grey_nozzle.width = width;
	grey_nozzle.height = height;
	grey_nozzle.type = GREY_IMAGE;

	grey_front.width = width;
	grey_front.height = height;
	grey_front.type = GREY_IMAGE;

	label_nozzle.width = width;
	label_nozzle.height = height;
	label_nozzle.type = LABEL_IMAGE;

	label_front.width = width;
	label_front.height = height;
	label_front.type = LABEL_IMAGE;

	allocate_image(rgb_nozzle);
	allocate_image(rgb_front);
	allocate_image(grey_nozzle);
	allocate_image(grey_front);
	allocate_image(label_nozzle);
	allocate_image(label_front);



	return 0;
}

int shutdown()
{
	free_image(rgb_nozzle);
	free_image(rgb_front);
	free_image(grey_nozzle);
	free_image(grey_front);
	free_image(label_nozzle);
	free_image(label_front);

	deactivate_cameras(); //TODO: check if need twice for each activate_vision
	return 0;
}