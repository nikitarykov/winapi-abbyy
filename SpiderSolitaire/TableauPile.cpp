#include "TableauPile.h"
#include "FieldWindow.h"
#include <iterator>

CTableauPile::CTableauPile()
{
	dragPile = 0;
	closedCount = 0;
	emptyBox = std::unique_ptr<CCardWindow>( new CCardWindow() );
	emptyBox->Open();
	isDragging = false;
}

CTableauPile::~CTableauPile()
{
}

void CTableauPile::SetBottomCardPos( int x, int y )
{
	bottomCardX = x;
	bottomCardY = y;
	SetWindowPos( emptyBox.get()->GetHandle(), HWND_TOP, 
		bottomCardX, bottomCardY, 71, 96, SWP_NOCOPYBITS );
	for( size_t i = 0; i < closedCount; ++i ) {
		SetWindowPos( cards[i].get()->GetHandle(), HWND_TOP, 
			bottomCardX, bottomCardY + i * backGap, 71, 96, SWP_NOCOPYBITS );
	}
	for( size_t i = 0; i < cards.size() - closedCount; ++i ) {
		SetWindowPos( cards[i + closedCount].get()->GetHandle(), HWND_TOP, 
			bottomCardX, bottomCardY + closedCount * backGap + i * frontGap, 71, 96, SWP_NOCOPYBITS );
	}
}

void CTableauPile::GetTopCardPos( int & x, int & y )
{
	x = bottomCardX;
	y = bottomCardY + closedCount * backGap + (cards.size() - closedCount) * frontGap;
	if( closedCount == cards.size() ) {
		y -= backGap;
	} else {
		y -= frontGap;
	}
}

void CTableauPile::GetNextCardPos( int & x, int & y )
{
	x = bottomCardX;
	y = bottomCardY + closedCount * backGap + (cards.size() - closedCount) * frontGap;
}

void CTableauPile::AddCard( std::unique_ptr<CCardWindow>& ptr )
{
	if( !ptr.get()->IsOpen() ) {
		closedCount++;
	}
	ptr.get()->SetPileIndex( cards.size() );
	ptr.get()->SetPile( this );
	cards.push_back( std::move(ptr) );
	SetBottomCardPos( bottomCardX, bottomCardY );
}

void CTableauPile::AddPile( std::vector< std::unique_ptr<CCardWindow> >& pile) {
	for( int i = 0; i < pile.size(); ++i ) {
		pile[i].get()->SetPile( this );
		pile[i].get()->SetPileIndex( cards.size() + i );
	}
	std::move(pile.begin(), pile.end(), std::back_inserter(cards)); 
	pile.clear();
	SetBottomCardPos( bottomCardX, bottomCardY );
}

std::unique_ptr<CCardWindow> CTableauPile::PopCard()
{
	std::unique_ptr<CCardWindow> card = std::move( cards.back() );
	cards.pop_back();
	return std::move( card );
}

void CTableauPile::OnButtonDown( int index )
{
	if( !isDragging && checkForDrag( index ) ) {
		isDragging = true;
		cards[index].get()->SetDragged();
		SetCapture( cards[index].get()->GetHandle() );
		PopPile( index );
	}
}

void CTableauPile::OnMouseMove( int x, int y )
{
	dragPile->SetBottomCardPos( x, y );
}

void CTableauPile::OnCaptureChanged( RECT& rect )
{
	dragPile->SetBottomCardPos( rect.left, rect.top );
	windowPtr->DropPile( std::move(dragPile), rect );
	dragPile.release();
}

void CTableauPile::OpenTop()
{
	cards.back().get()->Open();
	closedCount--;
}

void CTableauPile::CloseTop()
{
	cards.back().get()->Close();
	closedCount++;
}

bool CTableauPile::CheckForDrop( int id )
{
	if( cards.empty() || id + 1 == cards.back().get()->GetId() ) {
		return true;
	}
	return false;
}

void CTableauPile::CheckForOpen()
{
	if( !cards.empty() && !cards.back().get()->IsOpen() ) {
		cards.back().get()->Open();
		closedCount--;
	}
	SetBottomCardPos( bottomCardX, bottomCardY );
}

bool CTableauPile::CheckForComplete()
{
	if( cards.size() < 13 ) {
		return false;
	}
	for( int i = cards.size() - 13; i < cards.size(); ++i ) {
		if( !cards[i].get()->IsOpen() ) {
			return false;
		}
	}
	for( int i = cards.size() - 13; i < cards.size() - 1; ++i ) {
		if( cards[i].get()->GetId() != cards[i + 1].get()->GetId() + 1 ) {
			return false;
		}
	}
	return true;
}

void CTableauPile::CreateEmptyBox( HWND & handle )
{
	emptyBox->Create( handle );
}

bool CTableauPile::IsOpen()
{
	if( cards.empty() ) {
		return true;
	}
	return cards.back().get()->IsOpen();
}

void CTableauPile::Clear()
{
	closedCount = 0;
	DestroyWindow( emptyBox.get()->GetHandle() );
	for( int i = 0; i < cards.size(); ++i ) {
		DestroyWindow( cards[i].get()->GetHandle() );
	}	
	cards.clear();
}

bool CTableauPile::checkForDrag( int index )
{
	if( !cards[index].get()->IsOpen() ) {
		return false;
	}
	for( int i = index + 1; i < cards.size(); ++i ) {
		if( cards[i].get()->GetId() + 1 != cards[i - 1].get()->GetId() ) {
			return false;
		}
	}
	return true;
}

void CTableauPile::PopPile( int index )
{
	for( int i = index; i < cards.size(); ++i ) {
		if( !cards[i].get()->IsOpen() ) {
			closedCount--;
		}
	}
	SetBottomCardPos( bottomCardX, bottomCardY );
	std::vector< std::unique_ptr<CCardWindow> > popCards;
	std::move( cards.begin() + index, cards.end(), std::back_inserter( popCards ) );
	cards.erase( cards.begin() + index, cards.end() );
	int nextCardPosX, nextCardPosY;
	GetNextCardPos( nextCardPosX, nextCardPosY );
	dragPile.reset( new CDragPile() );
	dragPile.get()->AddPile( popCards, nextCardPosX, nextCardPosY );
}

void CTableauPile::SendDragPileToWindow()
{
	windowPtr->SetDragPile( dragPile );
}

void CTableauPile::SetIsDragging( bool is )
{
	isDragging = is;
}
