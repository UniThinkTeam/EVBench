#pragma once
#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;


class timeController
{
	time_t startTime;
	time_t endTime;
	UINT64 startTime64;
	UINT64 endTime64;
public:
	timeController();
	void start();
	void stop();
	double getDiferenceInSecs();
	double getDiferenceInSecs64();
	string getDifferenceInString();
};