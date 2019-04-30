#ifndef _GCODE_H_
#define _GCODE_H_

#include <math.h>

class gcode{
private:
	double Position[3]; //Position of the nozzle [x, y, z]
	double BedSize; //Size of the bed

public:
	gcode(); //Constructor
	gcode(double Pos[3], double Bed);
	~gcode(); //Deconstructor

	double Get_X(); //Return Position
	double Get_Y();
	double Get_Z();

	void Set_Position(double Pos[3]); //Go to fixed Values
	void Home();
	void Centre();
	void Set_BedSize(double Bed); //Change BedSize

	void Move_Up(double dist); //Move in pre determined directions
	void Move_Down(double dist);
	void Move_Forward(double dist);
	void Move_Backwards(double dist);
	void Move_Left(double dist);
	void Move_Right(double dist);

	void Move_Direction(double Dir[3], double dist); //Move in specific direction
};

#endif