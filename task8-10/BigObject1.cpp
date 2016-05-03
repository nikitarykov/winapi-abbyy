#include "BigObject1.h"

BigObject1::BigObject1()
{
}

BigObject1::~BigObject1()
{
}

void * BigObject1::operator new( size_t size )
{
	return heapManagerPtr->Alloc( size );
}

void BigObject1::operator delete( void* ptr )
{
	heapManagerPtr->Free( ptr );
}

void BigObject1::initHeapManager( CHeapManager* ptr )
{
	heapManagerPtr = ptr;
}

CHeapManager* BigObject1::heapManagerPtr = 0;
