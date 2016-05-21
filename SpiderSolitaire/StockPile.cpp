#pragma once

#include "StockPile.h"
#include "FieldWindow.h"

CStockPile::CStockPile()
{
}

CStockPile::~CStockPile()
{
}

void CStockPile::SetBottomCardPos( int x, int y )
{
	bottomCardX = x;
	bottomCardY = y;
	for( size_t i = 0; i < cards.size(); ++i ) {
		SetWindowPos( cards[i].get()->GetHandle(), HWND_TOP, 
			bottomCardX - gap * i, bottomCardY, 71, 96, SWP_NOCOPYBITS );
		//InvalidateRect( cards[i].get()->GetHandle(), 0, false );
	}
}

void CStockPile::GetTopCardPos( int & x, int & y )
{
	x = bottomCardX - gap * (cards.size() - 1);
	y = bottomCardY;
}

void CStockPile::GetNextCardPos( int & x, int & y )
{
	x = bottomCardX - gap * cards.size();
	y = bottomCardY;
}

void CStockPile::AddCard( std::unique_ptr<CCardWindow>& ptr )
{
	cards.push_back( std::move(ptr) );
}

std::unique_ptr<CCardWindow> CStockPile::PopCard()
{
	cards.back().get()->Show( SW_HIDE );
	DestroyWindow( cards.back().get()->GetHandle() );
	cards.back().release();
	cards.pop_back();
	SetBottomCardPos( bottomCardX, bottomCardY );
	return 0;
}

void CStockPile::OnButtonDown( int index )
{
	windowPtr->HandOut();
}

void CStockPile::OnMouseMove( int x, int y )
{
}

void CStockPile::OnCaptureChanged( RECT& rect )
{
}



