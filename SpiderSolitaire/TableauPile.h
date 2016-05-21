#pragma once

#include "Pile.h"
#include "DragPile.h"

class CTableauPile : public virtual CPile
{
public:
	CTableauPile();
	~CTableauPile();

	void SetBottomCardPos( int x, int y );
	void GetTopCardPos( int& x, int& y );
	void GetNextCardPos( int & x, int & y );
	void AddCard(std::unique_ptr<CCardWindow>& ptr);
	void AddPile( std::vector< std::unique_ptr<CCardWindow> >& pile );
	std::unique_ptr<CCardWindow> PopCard();
	void OnButtonDown( int index );
	void OnMouseMove( int x, int y );
	void OnCaptureChanged( RECT& rect );
	void OpenTop();
	void CloseTop();
	bool CheckForDrop( int id );
	void CheckForOpen();
	bool CheckForComplete();
	void CreateEmptyBox( HWND& handle );
	void Clear();
	bool IsOpen();
	void PopPile( int index );
	void SendDragPileToWindow();
	void SetIsDragging( bool is );

private:
	static const int backGap = 5;
	static const int frontGap = 15;
	int closedCount;
	std::unique_ptr<CDragPile> dragPile;
	std::unique_ptr<CCardWindow> emptyBox;
	bool isDragging;

	bool checkForDrag( int index );
};
