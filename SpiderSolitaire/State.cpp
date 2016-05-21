#include "State.h"

CState::CState()
{
	prevPile = 0;
	curPile = 0;
	curPileIndex = 0;
	wasPrevClosed = false;
}

CState::~CState()
{
}

void CState::SetPrevPile( CTableauPile * pile )
{
	prevPile = pile;
}

void CState::SetCurPile( CTableauPile * pile )
{
	curPile = pile;
}

void CState::SetCurPileIndex( int index )
{
	curPileIndex = index;
}

void CState::SetWasPrevPileClosed( bool was )
{
	wasPrevClosed = was;
}

CTableauPile * CState::GetPrevPile()
{
	return prevPile;
}

CTableauPile * CState::GetCurPile()
{
	return curPile;
}

int CState::GetCurPileIndex()
{
	return curPileIndex;
}

bool CState::GetWasPrevPileClosed()
{
	return wasPrevClosed;
}