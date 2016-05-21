// ��������: ����� �FieldWindow. ������������ �����  ����� ����
// ��� �������� "����"

#pragma once

#define OEMRESOURCE

#include <windows.h>
#include "State.h"
#include "TableauPile.h"
#include "StockPile.h"
#include "WastePile.h"
#include "DragPile.h"
#include <vector>
#include <deque>

class CFieldWindow
{
public:
	CFieldWindow();
	~CFieldWindow();

	CFieldWindow(CFieldWindow&&) = default;
	CFieldWindow& operator=(CFieldWindow&&) = default;

	CFieldWindow(const CFieldWindow&) = delete;
	CFieldWindow& operator=(const CFieldWindow&) = delete;

	// ���������������� ����� ����
	static bool RegisterClass();
	// ������� ��������� ����
	bool Create();
	// �������� ����
	void Show(int commandShow);
	void HandOut();
	void SetDragPile( std::unique_ptr<CDragPile>& dragPile );
	void DropPile(std::unique_ptr<CDragPile>& dragPile, RECT& rect );
	void Complete();
	void InitField(bool isRestart);
	
protected:
	// �������, ���������� ��� �������� ������������ ������� ���� (��������� WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// �������, ���������� ��� �������� ���������� ������� ���� (��������� WM_CREATE)
	void OnCreate();
	void OnGetMinMaxInfo(LPARAM lParam);
	void OnCommand( WPARAM wParam );
	void OnSetCursor();
	// �������, ���������� ��� ��������� �������� ���� (��������� WM_SIZE)
	void OnSize();
	// �������, ���������� ��� ����������� ���� (��������� WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // ����� ����
	int tickCount;
	bool isMovingBack;
	std::vector<int> startPositions;
	std::vector<int> values;
	std::vector<std::unique_ptr<CTableauPile> > tableauPiles;
	std::vector<std::unique_ptr<CCardWindow> > movingCards;
	std::deque<int> tickCountForMovingCards;
	std::unique_ptr<CDragPile> dragPile;
	CTableauPile* prevPile;
	CStockPile* stockPile;
	CWastePile* wastePile;
	std::vector<std::unique_ptr<CState> > steps;
	
	void showRules();
	void stepBack();

	static void CALLBACK startHandingOutCardsProc( HWND handle, UINT message, UINT_PTR idEvent, DWORD dwTime );
	static void CALLBACK handingOutCardsProc( HWND handle, UINT message, UINT_PTR idEvent, DWORD dwTime );
	static void CALLBACK backDropProc( HWND handle, UINT message, UINT_PTR idEvent, DWORD dwTime );
	static void CALLBACK completeProc( HWND handle, UINT message, UINT_PTR idEvent, DWORD dwTime );
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

