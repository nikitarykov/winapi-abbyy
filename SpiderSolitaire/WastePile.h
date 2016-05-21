#pragma once

#include "Pile.h"

class CWastePile : public virtual CPile
{
public:
	CWastePile();
	~CWastePile();

	void SetBottomCardPos( int x, int y );
	void GetTopCardPos( int& x, int& y );
	void GetNextCardPos( int& x, int& y );
	void AddCard( std::unique_ptr<CCardWindow>& ptr );
	std::unique_ptr<CCardWindow> PopCard();
	void OnButtonDown( int index );
	void OnMouseMove( int x, int y );
	void OnCaptureChanged( RECT& rect );

private:
	static const int gap = 12;
};