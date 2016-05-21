#include "WastePile.h"

CWastePile::CWastePile()
{
}

CWastePile::~CWastePile()
{
}

void CWastePile::SetBottomCardPos( int x, int y )
{
	bottomCardX = x;
	bottomCardY = y;
	for( size_t i = 0; i < cards.size(); ++i ) {
		SetWindowPos( cards[i].get()->GetHandle(), HWND_TOP, 
			bottomCardX + (i / 13) * gap, bottomCardY, 71, 96, SWP_NOCOPYBITS );
	}
}

void CWastePile::GetTopCardPos( int & x, int & y )
{
	x = bottomCardX + ( cards.size() - 1) / 13 * gap;
	y = bottomCardY;
}

void CWastePile::GetNextCardPos( int & x, int & y )
{
	x = bottomCardX + (cards.size() / 13) * gap;
	y = bottomCardY;
}

void CWastePile::AddCard( std::unique_ptr<CCardWindow>& ptr )
{
	cards.push_back( std::move(ptr) );
	cards.back().get()->SetPile( this );
	SetBottomCardPos( bottomCardX, bottomCardY );
}

std::unique_ptr<CCardWindow> CWastePile::PopCard()
{
	return 0;
}

void CWastePile::OnButtonDown( int index )
{
}

void CWastePile::OnMouseMove( int x, int y )
{
}

void CWastePile::OnCaptureChanged( RECT& rect )
{
}


