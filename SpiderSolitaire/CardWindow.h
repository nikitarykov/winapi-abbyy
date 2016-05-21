#pragma once

class CPile;
#include <windows.h>

class CCardWindow
{
public:
	CCardWindow();
	CCardWindow( int _frontId );
	~CCardWindow();

	CCardWindow(CCardWindow&&) = default;
	CCardWindow& operator=(CCardWindow&&) = default;

	CCardWindow(const CCardWindow&) = delete;
	CCardWindow& operator=(const CCardWindow&) = delete;

	// ���������������� ����� ����
	static bool RegisterClass();
	// ������� ��������� ����
	bool Create(HWND& handleParent);
	// �������� ����
	void Show(int commandShow);
	// ������ ��� ������ ����
	HWND& GetHandle();
	// ������� �� ������� ����� �����
	bool IsOpen();
	// ������� ������� ����� �����
	void Open();
	void Close();
	void SetPile( CPile* _pile );
	CPile* GetPile();
	int GetId();
	void SetPileIndex( int index );
	int GetPileIndex();
	void SetDragged();
	
protected:
	// �������, ���������� ��� �������� ������������ ������� ���� (��������� WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// �������, ���������� ��� �������� ���������� ������� ���� (��������� WM_CREATE)
	void OnCreate();
	// �������, ���������� ��� ����������� ����� (��������� WM_PAINT)
	void OnPaint();
	void OnButtonDown( LPARAM lParam );
	void OnMouseMove( LPARAM lParam );
	void OnButtonUp( LPARAM lParam );
	void OnCaptureChanged( LPARAM lParam );
	// �������, ���������� ��� ����������� ���� (��������� WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // ����� ����
	bool isOpen; // ������� �� ������� ����� �����
	static const HBITMAP backBitMap;
	static const HBITMAP frontBitMaps[14];
	CPile* pile;
	int pileIndex;
	int id;
	bool isDragged;
	int mousePosX;
	int mousePosY;

	void drawCard(HDC handleDC);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};


