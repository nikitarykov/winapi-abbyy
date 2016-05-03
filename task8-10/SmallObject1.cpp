#include "SmallObject1.h"

SmallObject1::SmallObject1()
{
}

SmallObject1::~SmallObject1()
{
}

void * SmallObject1::operator new( size_t size )
{
	return heapManagerPtr->Alloc( size );
}

void SmallObject1::operator delete( void* ptr )
{
	heapManagerPtr->Free( ptr );
}

void SmallObject1::initHeapManager( CHeapManager* ptr )
{
	heapManagerPtr = ptr;
}

CHeapManager* SmallObject1::heapManagerPtr = 0;
