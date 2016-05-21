#pragma once

#include <vector>
#include "CardWindow.h"

class CCardPile
{
public:
	CCardPile();
	~CCardPile();

	void Redraw();
	void AddCard( CCardWindow& card );
	void AddPile( CCardPile& pile );
	void PopPile( CCardPile& pile );

private:
	std::vector<CCardWindow> cards;
};