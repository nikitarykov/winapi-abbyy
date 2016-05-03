#pragma once

#include "HeapManager.h"

class SmallObject1
{
public:
	SmallObject1();
	~SmallObject1();
	void* operator new( size_t size );
	void operator delete( void* ptr );

	static void initHeapManager( CHeapManager* ptr );

private:
	int i;
	static CHeapManager* heapManagerPtr;
};
