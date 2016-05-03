// Описание: Класс CHeapManager. Представляет собой менеджер памяти,
// аналогичный куче.

#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <set>

class CHeapManager {
public:
	CHeapManager();
	~CHeapManager();


	void Create( int minSize, int maxSize );
	void* Alloc( int size );
	void Free( void* mem );
	void Destroy();

private:
	void* reservedMemoryStartPtr;
	std::vector< int > pages;
	std::map< int, std::set< void* > > reservedBlocksBySize;
	std::map< int, std::set< void* > > emptyCommitedBlocksBySize;
	std::map< void*, int > emptyCommitedBlocksByAddress;
	std::map< void*, int > usedBlocks;

	static const int defaultReserveSize = 1024 * 1024;
	static const int defaultCommitSize = 1024 * 64;
	static const int pageSize = 4096;

	void reserve( int size, void*& reservedPtr, int& reservedSize );
	void commit( void* blockPtr, int blockSize, int size, void*& commitedPtr, int& commitedSize  );
	int getStartPageIndex( void* blockPtr );
	int getEndPageIndex( void* blockPtr, int blockSize );
};