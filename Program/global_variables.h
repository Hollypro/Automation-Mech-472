//Global Variables used between .cpp files
#pragma once //include guard
#include "image_transfer3.h"

const int nozzle_camera = 0; //will need to calibrate
const int front_camera = 1; //will need to calibrate
const int height = 480; //image height
const int width = 640; //image width;
image rgb_nozzle, rgb_front, grey_nozzle, grey_front, label_nozzle, label_front;

