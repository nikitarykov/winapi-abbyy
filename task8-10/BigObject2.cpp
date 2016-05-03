#include "BigObject2.h"

BigObject2::BigObject2()
{
}

BigObject2::~BigObject2()
{
}

void * BigObject2::operator new( size_t size )
{
	return HeapAlloc( heap, 0, size );
}

void BigObject2::operator delete( void* ptr )
{
	HeapFree( heap, 0, ptr );
}

void BigObject2::initHeapManager( HANDLE& handle )
{
	heap = handle;
}

HANDLE BigObject2::heap = 0;
