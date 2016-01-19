// evbench.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "timeController.h"
#include "testProcessor.h"

using namespace std;

int main(int argc, char **argv)
{
	// test_name parameters_for_test running_parameters
	timeController tc;
	vector<string> params;
	testProcessor  tp(&tc);
	params.clear();
	// params settings
	for (int i = 1; i < argc; i++)  params.push_back(argv[i]);		
	if (params.size() > 1)
	{
		string testName = params[0];
		params.erase(params.begin());
		tp.performTest(testName, params);
		//system("pause");
	}
    return 0;
}

