#pragma once

#include <windows.h>

class SmallObject2
{
public:
	SmallObject2();
	~SmallObject2();
	void* operator new( size_t size );
	void operator delete( void* ptr );

	static void initHeapManager( HANDLE& heap );

private:
	int i;
	static HANDLE heap;
};
