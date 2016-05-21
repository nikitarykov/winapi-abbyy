#include "TableauPile.h"
#include "FieldWindow.h"
#include <iterator>

CDragPile::CDragPile()
{
}

CDragPile::~CDragPile()
{
}

void CDragPile::SetBottomCardPos( int x, int y)
{
	bottomCardX = x;
	bottomCardY = y;
	for( size_t i = 0; i < cards.size(); ++i ) {
		SetWindowPos( cards[i].get()->GetHandle(), HWND_TOP, 
			bottomCardX, bottomCardY + i * gap, 71, 96, SWP_NOZORDER | SWP_NOCOPYBITS );
	}
}

void CDragPile::GetTopCardPos( int & x, int & y )
{
}

void CDragPile::GetNextCardPos( int & x, int & y )
{
}

void CDragPile::AddCard( std::unique_ptr<CCardWindow>& ptr )
{
}

std::unique_ptr<CCardWindow> CDragPile::PopCard()
{
	return std::unique_ptr<CCardWindow>();
}

void CDragPile::AddPile( std::vector< std::unique_ptr<CCardWindow> >& pile, int x, int y ) 
{
	std::move(pile.begin(), pile.end(), std::back_inserter(cards)); 
	pile.clear();
	SetBottomCardPos( x, y );
}

void CDragPile::OnButtonDown( int index )
{
}

CPile* CDragPile::GetPrevPile()
{
	return cards[0].get()->GetPile();
}

int CDragPile::GetId()
{
	return cards[0].get()->GetId();
}

void CDragPile::OnMouseMove( int x, int y )
{
}

void CDragPile::OnCaptureChanged( RECT& rect )
{
}

std::unique_ptr<CCardWindow> PopCard()
{
	return 0;
}

void CDragPile::PopPile( 
	std::vector< std::unique_ptr<CCardWindow> >& target )
{
	std::move( cards.begin(), cards.end(), std::back_inserter( target ) );
	cards.clear();
}
