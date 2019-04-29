#include "gcode.h"
#include <iostream> //cout
#include <conio.h> //getch

gcode::gcode(){
	//G28 //Command to home all axes
	//G21 //Sets units to mm
	Position[0] = 0.0; //Default constructor brings nozzle home
	Position[1] = 0.0;
	Position[2] = 0.0;

	BedSize = 20; //Sets BedSize to 20 cm
}

gcode::gcode(double Pos[3], double Bed){
	Position[0] = Pos[0]; //This constructor brings the nozzle to a specific position.
	Position[1] = Pos[1]; //Assumes the pritner is already calibrated since no G28
	Position[2] = Pos[2];

	BedSize = Bed;
}

gcode::~gcode(){
}

double gcode::Get_X(){
	return Position[0]; //returns x
}

double gcode::Get_Y(){
	return Position[1]; //returns y
}

double gcode::Get_Z(){
	return Position[2]; //returns z
}

void gcode::Set_Position(double Pos[3]){
	Position[0] = Pos[0]; //Brings the nozzle to a specific position
	Position[1] = Pos[1];
	Position[2] = Pos[2];

	if (Position[2] < 1.5) //so that printer is not run too low accidentally
	{
		char ch;
		if (Position[2] < 0.01) return; //esentially 0;
		else
		{
			std::cout << "WARNING: setup_printer--part_height very low\n";
			std::cout << "Press c to continue anyway\n";
			ch = _getch();
		}
		if (ch != 'c') return; //exits program
	}
}
void gcode::Home(){
	//G28 //This command finds home
	Position[0] = 0.0; //Brings each coordinate to home position
	Position[1] = 0.0;
	Position[2] = 0.0;
}

void gcode::Centre(){
	//G28 //This command finds home
	Position[0] = 0.0 + BedSize / 2.0; //Brings each coordinate to bed centre position
	Position[1] = 0.0 + BedSize / 2.0;
	Position[2] = 0.0;
}

void gcode::Set_BedSize(double Bed){
	BedSize = Bed;
}

//Simply moves the nozzles in either x, y, or z by distance dist.
void gcode::Move_Up(double dist){
	Position[3] += dist;
}
void gcode::Move_Down(double dist){
	Position[3] -= dist;
}
void gcode::Move_Forward(double dist){
	Position[2] += dist;
}
void gcode::Move_Backwards(double dist){
	Position[2] -= dist;
}
void gcode::Move_Left(double dist){
	Position[1] -= dist;
}
void gcode::Move_Right(double dist){
	Position[1] += dist;
}

void gcode::Move_Direction(double Dir[3], double dist){ //Move in specific direction

	//Calculates the magnitude of the direction vector.
	double mag = sqrt(Dir[0] * Dir[0] + Dir[1] * Dir[1] + Dir[2] * Dir[2]);

	//Calculates the length of the distance vector in each coordinate
	double dist_x = Dir[0] / mag * dist;
	double dist_y = Dir[1] / mag * dist;
	double dist_z = Dir[2] / mag * dist;

	Position[0] += dist_x;
	Position[1] += dist_y;
	Position[2] += dist_z;
}