#pragma once

#include <windows.h>

class CCardWindow
{
public:
	CCardWindow();
	~CCardWindow();

	// ���������������� ����� ����
	static bool RegisterClass();
	// ������� ��������� ����
	bool Create(HWND& handleParent);
	// �������� ����
	void Show(int commandShow);
	// ������ ��� ������ ����
	HWND GetHandle();
	
protected:
	// �������, ���������� ��� �������� ������������ ������� ���� (��������� WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// �������, ���������� ��� �������� ���������� ������� ���� (��������� WM_CREATE)
	void OnCreate();
	// �������, ���������� ��� ����������� ���� ����� (��������� WM_ERASEBKGND)
	void OnDrawBackground();
	// �������, ���������� ��� ����������� ���� (��������� WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // ����� ����

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};


