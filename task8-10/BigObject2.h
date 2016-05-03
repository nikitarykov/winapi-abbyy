#pragma once

#include <windows.h>

class BigObject2
{
public:
	BigObject2();
	~BigObject2();
	void* operator new( size_t size );
	void operator delete( void* ptr );

	static void initHeapManager( HANDLE& heap );

private:
	int i;
	static HANDLE heap;
};
