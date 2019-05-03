#ifndef _PRINTCOMMUNICATION_H_
#define _PRINTCOMMUNICATION_H_

// https://www.instructables.com/id/Begging-Robot/
// Was used on info for port communication


//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "CXBOXController.h"

#include <iostream>
#include<strstream>
#include "serial_com.h"
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include <windows.h>

using namespace std;


class PComm{
private:
	//char Port[13];
	HANDLE h1;
	const int NMAX = 64;
	char buffer[NMAX];
	
public:
	PComm(/*int Com*/);
	~PComm();

	void Send_Pos(double Pos[3]);
	void Home();
};

PComm::PComm(){
	//open_serial("\\\\.\\COM3", h1);
}

PComm::~PComm(){
}

void PComm::Send_Pos(double Pos[3]){
	//ostrstream sout(buffer, NMAX);

	//char *Position;
	//string PPos[3], Full;
	//PPos[0] = to_string(Pos[0]);
	//PPos[1] = to_string(Pos[1]);
	//PPos[2] = to_string(Pos[2]);

	//
	//Full = 'X';
	//Full += PPos[0];
	//Full += 'Y';
	//Full += PPos[1];
	//Full += 'Z';
	//Full += PPos[2];
	//
	//Position = new char[Full.length() + 1];

	//std::copy(Full.begin(), Full.end(), Position);

	//Position[Full.length()] = '\0';
	//
	//cout << Position <<endl;
	////Port->sendArray(Position, Full.length());

	//delete Position;
}

void PComm::Home(){


	//sout
}
#endif