//Описание: Реализация методов класса HeapManager

#include "HeapManager.h"
#include <iostream>

CHeapManager::CHeapManager()
{
	reservedMemoryStartPtr = 0;
	pages = std::vector< int >();
	reservedBlocksBySize = std::map< int, std::set< void* > >();
	emptyCommitedBlocksBySize = std::map< int, std::set< void* > >();
	emptyCommitedBlocksByAddress = std::map< void*, int >();
	usedBlocks = std::map< void*, int >();
}

CHeapManager::~CHeapManager()
{
}

void CHeapManager::Create( int minSize, int maxSize )
{
	int reservedSize;
	void* reservedPtr;
	reserve( maxSize, reservedPtr, reservedSize );
	int commitedSize;
	void* commitedPtr;
	commit( reservedPtr, reservedSize, minSize, commitedPtr, commitedSize);
	emptyCommitedBlocksBySize[commitedSize].emplace( commitedPtr );
	emptyCommitedBlocksByAddress[commitedPtr] = commitedSize;
}

void* CHeapManager::Alloc( int size )
{
	if( size % 4 != 0 ) {
		size += 4 - ( size % 4 );
	}
	void* blockPtr;
	int blockSize;
	std::map< int, std::set< void* > >::iterator it = 
		emptyCommitedBlocksBySize.lower_bound(size);
	if( it == emptyCommitedBlocksBySize.end() ) {
		it = reservedBlocksBySize.lower_bound(size);
		if( it == reservedBlocksBySize.end() ) {
			throw std::exception( "Memory allocation error" );
		} 
		int commitSize = it->first;
		void* commitPtr = *it->second.begin();
		auto& ptrSet = reservedBlocksBySize[commitSize];
		ptrSet.erase( commitPtr );
		if( ptrSet.empty() ) {
			reservedBlocksBySize.erase( commitSize );
		}
		commit( commitPtr, commitSize, size, blockPtr, blockSize );
	} else {
		std::set< void* >::iterator blockIt = it->second.begin();
		blockPtr = *blockIt;
		blockSize = it->first;
		auto& ptrSet = emptyCommitedBlocksBySize[blockSize];
		ptrSet.erase( blockPtr );
		if( ptrSet.empty() ) {
			emptyCommitedBlocksBySize.erase( blockSize );
		}
		emptyCommitedBlocksByAddress.erase( blockPtr );
	}
	int tailSize = blockSize - size;
	void* tailPtr = static_cast< void* >( 
		static_cast< char* >( blockPtr ) + size );
	if( tailSize > 0 ) {
		emptyCommitedBlocksByAddress[tailPtr] = tailSize;
		emptyCommitedBlocksBySize[tailSize].emplace( tailPtr );
	}
	pages[getStartPageIndex( blockPtr )]++;
	pages[getEndPageIndex( blockPtr, size )]++;
	usedBlocks[blockPtr] = size;
	return blockPtr;
}

void CHeapManager::Free( void* mem )
{
	std::map< void*, int >::iterator blockIt, prevIt, nextIt;
	blockIt = usedBlocks.find( mem );
	if( blockIt == usedBlocks.end() ) {
		throw std::exception( "Error: trying to free not allocated memory" );
	}
	void* blockPtr = blockIt->first;
	int blockSize = blockIt->second;
	pages[getStartPageIndex( blockPtr )]--;
	pages[getEndPageIndex( blockPtr, blockSize )]--;
	usedBlocks.erase( blockIt );
	nextIt = emptyCommitedBlocksByAddress.upper_bound( blockPtr );
	if( nextIt != emptyCommitedBlocksByAddress.end() && nextIt->first == 
		static_cast< void* >( static_cast< char* >( blockPtr ) + blockSize ) ) 
	{
		void* nextPtr = nextIt->first;
		int nextSize = nextIt->second;
		blockSize += nextSize;
		nextIt = emptyCommitedBlocksByAddress.erase( nextIt );
		emptyCommitedBlocksBySize[nextSize].erase( nextPtr );
		if( emptyCommitedBlocksBySize[nextSize].empty() ) {
			emptyCommitedBlocksBySize.erase( nextSize );
		}
	}
	if( nextIt != emptyCommitedBlocksByAddress.begin() ) {
		prevIt = --nextIt;
		void* prevPtr = prevIt->first;
		int prevSize = prevIt->second;
		if( blockPtr == static_cast< void* >( static_cast< char* >( prevPtr ) + prevSize ) ) {
			emptyCommitedBlocksByAddress.erase( prevPtr );
			emptyCommitedBlocksBySize[prevSize].erase( prevPtr );
			if( emptyCommitedBlocksBySize[prevSize].empty() ) {
				emptyCommitedBlocksBySize.erase( prevSize );
			}
			blockSize = prevSize + blockSize;
			blockPtr = prevPtr;
		}
	}
	int startIndex = getStartPageIndex( blockPtr );
	int endIndex = getEndPageIndex( blockPtr, blockSize );
	if( pages[startIndex] > 0 ) {
		startIndex++;
	}
	if( pages[endIndex] > 0 ) {
		endIndex--;
	}
	if( endIndex >= startIndex ) {
		void* decommitPtr = static_cast< void* >(
			static_cast< char* >( reservedMemoryStartPtr ) + startIndex * pageSize );
		int decommitSize = ( endIndex - startIndex + 1 ) * pageSize;
		VirtualFree( decommitPtr, decommitSize, MEM_DECOMMIT );
		reservedBlocksBySize[decommitSize].emplace( decommitPtr );
		int leftBlockSize = 0;
		if( getStartPageIndex( blockPtr ) != startIndex ) {
			leftBlockSize = static_cast< char* >( decommitPtr ) - 
				static_cast< char* >( blockPtr );
			emptyCommitedBlocksByAddress[blockPtr] = leftBlockSize;
			emptyCommitedBlocksBySize[leftBlockSize].emplace( blockPtr );
		}
		if( getEndPageIndex( blockPtr, blockSize ) != endIndex ) {
			int rightBlockSize = blockSize - decommitSize - leftBlockSize;
			void* rightBlockPtr = static_cast< void* >(
				static_cast< char* >( decommitPtr ) + decommitSize );
			emptyCommitedBlocksByAddress[rightBlockPtr] = rightBlockSize;
			emptyCommitedBlocksBySize[rightBlockSize].emplace( rightBlockPtr );
		}
	} else {
		emptyCommitedBlocksByAddress[blockPtr] = blockSize;
		emptyCommitedBlocksBySize[blockSize].emplace( blockPtr );
	}
}

void CHeapManager::Destroy()
{
	for( auto it = usedBlocks.begin(); it != usedBlocks.end(); ++it ) {
		std::cout << it->first << " " << it->second << std::endl;
	}
	VirtualFree( reservedMemoryStartPtr, 0, MEM_RELEASE );
}

void CHeapManager::reserve( int size, void*& reservedPtr, int& reservedSize )
{
	reservedSize = size;
	if( reservedSize <= 0 ) {
		reservedSize = defaultReserveSize;
	}
	if( reservedSize % pageSize != 0 ) {
		reservedSize += pageSize - ( reservedSize % pageSize );
	}
	reservedPtr = VirtualAlloc(0, reservedSize,
			MEM_RESERVE, PAGE_READWRITE);
	if( !reservedPtr ) {
		throw std::exception( "Memory reservation error" );
	}
	reservedMemoryStartPtr = reservedPtr;
	pages.resize( reservedSize / pageSize );
}

void CHeapManager::commit( void* blockPtr, int blockSize, int size, void*& commitedPtr, int& commitedSize )
{
	commitedSize = size;
	if( commitedSize <= 0 ) {
		commitedSize = pageSize;
	}
	if( commitedSize % pageSize != 0 ) {
		commitedSize += pageSize - ( commitedSize % pageSize );
	}
	commitedPtr = VirtualAlloc( blockPtr, commitedSize,
		MEM_COMMIT, PAGE_READWRITE );
	if( !commitedPtr ) {
		throw std::exception( "Memory commitment error" );
	}
	if( blockSize - commitedSize > 0 ) {
		reservedBlocksBySize[blockSize - commitedSize].emplace(
			static_cast< void* >( static_cast< char* >( commitedPtr ) + commitedSize ) );
	}
}

int CHeapManager::getStartPageIndex( void * blockPtr )
{
	return ( static_cast< char* >( blockPtr ) - 
		static_cast< char* >( reservedMemoryStartPtr ) ) / pageSize;
}

int CHeapManager::getEndPageIndex( void * blockPtr, int blockSize )
{
	return ( static_cast< char* >( blockPtr ) + blockSize - 
		static_cast< char* >( reservedMemoryStartPtr ) - 1 ) / pageSize;
}
