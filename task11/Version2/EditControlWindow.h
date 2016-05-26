// ��������: ����� �EditControlWindow. ������������ ����� C++ ��������
// ��� ������� ���� � Edit Control.

#pragma once

#include <windows.h>

class CEditControlWindow {
public:
	CEditControlWindow();
	~CEditControlWindow();

	// ������� ��������� ����
	bool Create(HWND& handleParent);
	// ������ ��� ������ ����
	HWND GetHandle();
	// �������, ��������� ���� ����� �� ���������� ������
	void InitFont();
	// �������, ��������� ���� �����
	void DestroyFont();
	// ���������� ������ ������ � EditControl
	int GetFontSize();
	// ������������� ������ ������ � EditControl
	void SetFontSize(int size);

private:
	HWND handle; // ����� ����
		
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

