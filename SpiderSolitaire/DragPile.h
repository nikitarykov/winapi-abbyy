#pragma once

#include "Pile.h"

class CDragPile : public virtual CPile
{
public:
	CDragPile();
	~CDragPile();

	void SetBottomCardPos( int x, int y );
	void GetTopCardPos( int& x, int& y );
	void GetNextCardPos( int & x, int & y );
	void AddCard(std::unique_ptr<CCardWindow>& ptr);
	std::unique_ptr<CCardWindow> PopCard();
	void OnCaptureChanged( RECT& rect );
	void OnMouseMove( int x, int y );
	void OnButtonDown( int index );
	CPile* GetPrevPile();
	int GetId();
	void AddPile( std::vector< std::unique_ptr<CCardWindow> >& pile, int x, int y );
	void PopPile(std::vector< std::unique_ptr<CCardWindow> >& target);

private:
	static const int gap = 15;
};
