#include "SmallObject2.h"

SmallObject2::SmallObject2()
{
}

SmallObject2::~SmallObject2()
{
}

void * SmallObject2::operator new( size_t size )
{
	return HeapAlloc( heap, 0, size );
}

void SmallObject2::operator delete( void* ptr )
{
	HeapFree( heap, 0, ptr );
}

void SmallObject2::initHeapManager( HANDLE& handle )
{
	heap = handle;
}

HANDLE SmallObject2::heap = 0;
