#pragma once

#include "HeapManager.h"

class BigObject1
{
public:
	BigObject1();
	~BigObject1();
	void* operator new( size_t size );
	void operator delete( void* ptr );

	static void initHeapManager( CHeapManager* ptr );

private:
	int array[100000];
	static CHeapManager* heapManagerPtr;
};
