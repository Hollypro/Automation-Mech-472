#ifndef _PRINTCOMMUNICATION_H_
#define _PRINTCOMMUNICATION_H_

// https://www.instructables.com/id/Begging-Robot/
// Was used on info for port communication


//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "CXBOXController.h"

#include <iostream>
#include "tserial.h"
#include "tserial.cpp"
#include <bitset>
#include <string>
#include <cstring>

using namespace std;


class PComm{
private:
	int Comm;
	Tserial *Port;
public:
	PComm(int Com);

	void Send_Pos(double Pos[3]);
	void Home();
};

PComm::PComm(int Com){
	char Commu = (char)Com;
	char *Commun = &Commu;
	Comm = Com;
	Port = new Tserial();
	Port->connect(const_cast<char*>(Commun), 9600, spNONE);
}

void PComm::Send_Pos(double Pos[3]){
	char *Position;
	string PPos[3], Full;
	PPos[0] = to_string(Pos[0]);
	PPos[1] = to_string(Pos[1]);
	PPos[2] = to_string(Pos[2]);

	
	Full = 'X';
	Full += PPos[0];
	Full += 'Y';
	Full += PPos[1];
	Full += 'Z';
	Full += PPos[2];
	
	Position = new char[Full.length() + 1];

	std::copy(Full.begin(), Full.end(), Position);

	Position[Full.length()] = '\0';
	
	cout << Position <<endl;
	Port->sendArray(Position, Full.length());

	delete Position;
}

void PComm::Home(){
	char *Position;


	Position = new char[4];

	Position[0] = 'G';
	Position[1] = '2';
	Position[2] = '8';
	Position[3] = '\0';

	cout << Position << endl;
	Port->sendArray(Position, 4);

	delete Position;
}
#endif