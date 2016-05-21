#pragma once

#include "TableauPile.h"

class CState
{
public:
	CState();
	~CState();

	void SetPrevPile(CTableauPile* pile);
	void SetCurPile(CTableauPile* pile);
	void SetCurPileIndex(int index);
	void SetWasPrevPileClosed(bool was);
	CTableauPile* GetPrevPile();
	CTableauPile* GetCurPile();
	int GetCurPileIndex();
	bool GetWasPrevPileClosed();

private:
	CTableauPile* prevPile;
	CTableauPile* curPile;
	int curPileIndex;
	bool wasPrevClosed;
};
