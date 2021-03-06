// ��������: ����� �EllipseWindow. ������������ ����� C++ ��������
// ��� ������� ���� � ���������� ��������.

#pragma once

#include <windows.h>

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	// ���������������� ����� ����
	static bool RegisterClass();

	// ������� ��������� ����
	bool Create(HWND& handleParent);
	// �������� ����
	void Show(int cmdShow);
	// ������ ��� ������ ����
	HWND getHandle();

protected:
	// �������, ���������� ��� �������� ������������ ������� ���� (��������� WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// �������, ���������� ��� �������� ���������� ������� ���� (��������� WM_CREATE)
	void OnCreate();
	// �������, ���������� ��� ����������� ���� (��������� WM_PAINT)
	void OnPaint();
	// �������, ���������� ��� ������������ ������� (��������� WM_TIMER)
	void OnTimer();
	// �������, ���������� ��� ������� �� ���� (��������� WM_LBUTTONDOWN)
	void OnButtonDown();
	// �������, ���������� ��� ����������� ���� (��������� WM_DESTROY)
	void OnDestroy();

	// ����������� �������
	void drawEllipse(HDC& memoryDC, RECT& rect);
private:
	HWND handle; // ����� ����
	UINT_PTR timer; // ������ ��� �����������
	int xSpeed; // �������� ������� �� ��� x
	int ySpeed; // �������� ������� �� ��� y
	int x; // ���������� ������ ������� �� ��� x
	int y; // ���������� ������ ������� �� ��� y
	int tickCount; // ���������� ��������� �� �������
	int paintCount; // ���������� �����������
	static const int aAxis = 100; // ������� ��� �������
	static const int bAxis = 60; // �������� ��� �������

	// ������ ��� ����������� ��������� ������ �������
	void calcX(const RECT& rect);
	void calcY(const RECT& rect);
	
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

