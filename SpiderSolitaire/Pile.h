#pragma once

//class CCardWindow;
class CFieldWindow;
#include <vector>
#include <memory>
#include "CardWindow.h"
//#include "FieldWindow.h"

class CPile
{
public:
	CPile();
	~CPile();

	CPile(CPile&&) = default;
	CPile& operator=(CPile&&) = default;

	CPile(const CPile&) = delete;
	CPile& operator=(const CPile&) = delete;

	virtual void SetBottomCardPos( int x, int y ) = 0;
	virtual void GetTopCardPos( int& x, int& y ) = 0;
	virtual void GetNextCardPos( int& x, int& y ) = 0;
	virtual void AddCard(std::unique_ptr<CCardWindow>& ptr) = 0;
	virtual std::unique_ptr<CCardWindow> PopCard() = 0;
	virtual void OnCaptureChanged( RECT& rect ) = 0;
	virtual void OnMouseMove( int x, int y ) = 0;
	virtual void OnButtonDown( int index ) = 0;
	virtual void SetWindow(CFieldWindow* ptr);
	void GetBottomCardPos( int& x, int& y );
	int GetSize();
	void Clear();

protected:
	int bottomCardX;
	int bottomCardY;
	CFieldWindow* windowPtr;
	std::vector<std::unique_ptr<CCardWindow> > cards;
};
