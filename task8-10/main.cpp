#include "HeapManager.h"
#include "SmallObject1.h"
#include "BigObject1.h"
#include "SmallObject2.h"
#include "BigObject2.h"
#include <iostream>
#include <chrono>
#include <conio.h>
#include <psapi.h>

class SteadyTimer {
public:
	SteadyTimer() 
	{
		Reset();
	}

	void Reset() 
	{
		start = std::chrono::steady_clock::now();
	}

	double SecondsElapsed() const 
	{
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast< std::chrono::duration<double> >(now - start);
		return elapsed.count();
	}

private:
	std::chrono::steady_clock::time_point start;
};

void heapManagerTimeTest()
{
	CHeapManager heapManager = CHeapManager();
	heapManager.Create(1024*16, 64*1024*1024);
	SmallObject1::initHeapManager( &heapManager );
	BigObject1::initHeapManager( &heapManager );
	SmallObject1* smallObjectPtrs[100000];
	SteadyTimer timer;
	for( size_t i = 0; i < 50000; ++i ) {
		smallObjectPtrs[i] = new SmallObject1;
	}
	BigObject1* bigObjectPtr = new BigObject1;
	for( size_t i = 50000; i < 100000; ++i ) {
		smallObjectPtrs[i] = new SmallObject1;
	}
	for( size_t i = 50000; i < 100000; ++i ) {
		delete smallObjectPtrs[i];
	}
	for( size_t i = 0; i < 50000; ++i ) {
		delete smallObjectPtrs[i];
	}
	delete bigObjectPtr;
	double heapManagerTime = timer.SecondsElapsed();
	std::cout << "HeapManager needed " << heapManagerTime << " seconds." << std::endl;
	HANDLE processHandle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX info;
	info.cb = sizeof(info);
	GetProcessMemoryInfo(processHandle, reinterpret_cast< PROCESS_MEMORY_COUNTERS* >( &info ), info.cb);
	std::cout << "HeapManager WorkingSetSize: " << info.WorkingSetSize << " bytes." << std::endl;
	heapManager.Destroy();
}

void defaultHeapTimeTest()
{
	HANDLE heap;
	heap = HeapCreate( 0, 1024*16, 64*1024*1024);
	SmallObject2::initHeapManager( heap );
	BigObject2::initHeapManager( heap );
	SmallObject2* smallObjectPtrs[100000];
	SteadyTimer timer;
	for( size_t i = 0; i < 50000; ++i ) {
		smallObjectPtrs[i] = new SmallObject2;
	}
	BigObject2* bigObjectPtr = new BigObject2;
	for( size_t i = 50000; i < 100000; ++i ) {
		smallObjectPtrs[i] = new SmallObject2;
	}
	for( size_t i = 50000; i < 100000; ++i ) {
		delete smallObjectPtrs[i];
	}
	for( size_t i = 0; i < 50000; ++i ) {
		delete smallObjectPtrs[i];
	}
	delete bigObjectPtr;
	double heapManagerTime = timer.SecondsElapsed();
	std::cout << "Default heap needed " << heapManagerTime << " seconds." << std::endl;
	HANDLE processHandle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX info;
	info.cb = sizeof(info);
	GetProcessMemoryInfo(processHandle, reinterpret_cast< PROCESS_MEMORY_COUNTERS* >( &info ), info.cb);
	std::cout << "Default heap WorkingSetSize: " << info.WorkingSetSize << " bytes." << std::endl;
	HeapDestroy( heap );
}

int main()
{
	defaultHeapTimeTest();
	heapManagerTimeTest();
	getch();
	return 0;
}
