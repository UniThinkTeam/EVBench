#include "stdafx.h"
#include <string>
#include <sstream>
#include "timeController.h"


UINT64 getTime()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);  // converts to file time format
	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;
	return ui.QuadPart;
}

timeController::timeController()
{
	
}

void timeController::start()
{
	time(&startTime);
	startTime64 = getTime();
}

	
void timeController::stop()
{
	time(&endTime);
	endTime64 = getTime();
}
	
double timeController::getDiferenceInSecs()
{
	return difftime(endTime, startTime);
}

double timeController::getDiferenceInSecs64()
{
	return double(endTime64 - startTime64)/10000000;
}

string timeController::getDifferenceInString()
{	
	time_t seconds(getDiferenceInSecs()); 
	tm *p = gmtime(&seconds); 
	ostringstream oss;
	string h, m, s;
	// hours
	oss.str("");
	oss << p->tm_hour;
	h = oss.str();
	if (h.length() < 2) h = "0" + h;
	// mins
	oss.str("");
	oss << p->tm_min;
	m = oss.str();
	if (m.length() < 2) m = "0" + m;
	// secs
	oss.str("");
	oss << p->tm_sec;
	s = oss.str();
	if (s.length() < 2) s = "0" + s;
	return (h+":"+m+":"+s);
}