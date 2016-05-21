#include "Pile.h"

CPile::CPile()
{
	bottomCardX = 0;
	bottomCardY = 0;
	cards = std::vector<std::unique_ptr<CCardWindow> >();
	windowPtr = 0;
}

CPile::~CPile()
{
}

void CPile::SetWindow( CFieldWindow* ptr )
{
	windowPtr = ptr;
}

void CPile::GetBottomCardPos( int & x, int & y )
{
	x = bottomCardX;
	y = bottomCardY;
}

int CPile::GetSize()
{
	return cards.size();
}

void CPile::Clear()
{
	for( int i = 0; i < cards.size(); ++i ) {
		DestroyWindow( cards[i].get()->GetHandle() );
	}	
	cards.clear();
}
