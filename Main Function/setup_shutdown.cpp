#include <iostream>
#include <cmath>
#include <conio.h>

using namespace std;

#include "image_transfer2.h"
#include "vision.h"
#include "timer.h"
#include "global_variables.h"

int setup()
{
	activate_vision(nozzle_camera);
	activate_vision(front_camera);

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

	deactivate_vision(); //TODO: check if need twice for each activate_vision
	return 0;
}