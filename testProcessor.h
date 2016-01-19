#pragma once
#include <iostream>
#include <vector>
#include "timeController.h"

using namespace std;

class testProcessor
{
private:
	// time controller for
	timeController *tc;
public:
	testProcessor(timeController *tc);
	~testProcessor();
	void performTest(string testName, vector<string> params);
};


