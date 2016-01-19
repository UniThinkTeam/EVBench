#include "stdafx.h"
#include "testProcessor.h"
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>
#include <stdlib.h>
#include <condition_variable>

mutex m1, m2;
condition_variable cv;
bool ready;

testProcessor::testProcessor(timeController *tc)
{
	this->tc = tc;
	tc->stop();
}

testProcessor::~testProcessor()
{
}

void write(string s)
{
	m2.lock();
	cout << s.c_str() << endl;
	m2.unlock();
}


// cpu testing
bool testPrime(int number)
{
	for (int i = 3; i < number; i++)
		if (number % i == 0) return false;
	return true;
}

void testPrimes(int index, int maxNumber, int *remainingCount)
{
	// wait until all ready
	mutex m;
	unique_lock<std::mutex> lck(m);
	while (!ready) cv.wait(lck);
    // test starting
	while (true)
	{		
		m1.lock();
		if (*remainingCount > 0)
		{
			(*remainingCount)--;
			//cout << *remainingCount << endl;
		}
		m1.unlock();
		if (*remainingCount <= 0) break;
		for (int i = 0; i < maxNumber; i++)
			testPrime(i);
	}
}

// memory testing
void copyArray(char *inputArray, char *outputArray, int size)
{
	//for (int i = 0; i < size; i++)
	//	outputArray[i] = inputArray[i];
	memcpy(inputArray, outputArray, size);
}

void testMem(int index, double arraySize, double wholeSize)
{
	srand(time(NULL));
	double transferedBytes = 0;
	char *inputArray = new char[arraySize];
	char *outputArray = new char[arraySize];
	char *buffArray = NULL;
	// input array initialization
	for (int i = 0; i < arraySize; i++)
		inputArray[i] = rand() % 1000;
	// wait until all ready
	mutex m;
	unique_lock<std::mutex> lck(m);
	while (!ready) cv.wait(lck);
	// copying phase
	while (transferedBytes < wholeSize)
	{
		copyArray(inputArray, outputArray, arraySize);
		// array modify			
		transferedBytes += arraySize;
		// pointer redirection
		//buffArray = inputArray;
		//inputArray = outputArray;
		//outputArray = buffArray;
	}
}

long long valueToBytes(string value)
{
	long multiply = 1;	
	if (value[value.length() - 1] == 'K')
		multiply *= 1024;
	if (value[value.length() - 1] == 'M')
		multiply *= 1024*1024;
	if (value[value.length() - 1] == 'G')
		multiply *= 1024 * 1024*1024;
	if (multiply > 1)
		value = value.substr(0, value.length() - 1);
	long long val = atof(value.c_str());
	return val * multiply;       
}


void testProcessor::performTest(string testName, vector<string> params)
{

	vector<thread*> tasks;
	tasks.clear();
	if (testName == "cpu")
	{
		if (params.size() != 3)
		{
			cout << "Using: cpu threadCount maxNumber repetionCount " << endl;
			return;
		}
		
		int threadCount = atoi(params[0].c_str());
		double maxNumber = atof(params[1].c_str());
		int repetitionCount = atoi(params[2].c_str());
		ready = false;
		for (unsigned int i = 0; i < threadCount; i++)
		{
			thread *t = new thread(testPrimes, i, maxNumber, &repetitionCount);
			tasks.push_back(t);
		}
		// synchronization 
		ready = true;
		cv.notify_all();
		// starting of all threads
		cout << "Testing has been started (thread count: " << threadCount << ")" <<  endl;
		tc->start();
		for (int i = 0; i < tasks.size(); i++) tasks[i]->join();
		tc->stop();
		cout << "Testing has been stopped." << endl;
		// statistics printing
		cout << "Test duration " << tc->getDiferenceInSecs() << "s, that is " << tc->getDifferenceInString().c_str() << endl;
		cout << "Exact difference : " << tc->getDiferenceInSecs64() << "s " << endl;
	}
	if (testName == "mem")
	{
		if (params.size() != 3)
		{
			cout << "Using: mem threadCount bufferSize wholeSize " << endl;
			return;
		}
		int threadCount = atoi(params[0].c_str());
		double bufferSize = valueToBytes(params[1].c_str());
		double wholeSize  = valueToBytes(params[2].c_str());
		ready = false;
		for (unsigned int i = 0; i < threadCount; i++)
		{
			thread *t = new thread(testMem, i, bufferSize, wholeSize);
			tasks.push_back(t);
		}
		Sleep(1500);
		// synchronization 
		ready = true;
		cv.notify_all();
		// starting of all threads
		tc->start();
		cout << "Testing has been started (thread count: " << threadCount << ")" << endl;		
		for (int i = 0; i < tasks.size(); i++) tasks[i]->join();
		tc->stop();
		cout << "Testing has been stopped." << endl;
		long long wholeTransferredCount = threadCount*wholeSize;
		cout << "Total transferredCount: " << wholeTransferredCount << endl;
		cout << "Total testing time: " << tc->getDiferenceInSecs64() << endl;
		// in bytes
		double transferRate = (double)wholeTransferredCount / tc->getDiferenceInSecs64();
		// in megabytes
		transferRate /= 1024*1024;
		cout << "Measured transfer rate: " << transferRate << " MB/s" << endl;
	}
}