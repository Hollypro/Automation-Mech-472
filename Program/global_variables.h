//Global Variables declaration (defined in global_variables.cpp)
#pragma once //include guard
#ifndef GLOBAL1
#define GLOBAL1
#include "image_transfer3.h"

extern const int nozzle_camera; //will need to calibrate
extern const int front_camera; //will need to calibrate
extern const int height; //image height
extern const int width; //image width;
extern double ic_nozzle[1000], jc_nozzle[1000]; //centroid positions from nozzle
extern image rgb_nozzle, rgb_front, grey_nozzle, grey_front, label_nozzle, label_front;

#endif