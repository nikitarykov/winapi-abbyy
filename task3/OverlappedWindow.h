// ��������: ����� �OverlappedWindow. ������������ ����� C++ ��������
// ��� ������� ���� WinAPI.

#pragma once

#include <windows.h>
#include "EllipseWindow.h"
#include <vector>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();

	// ���������������� ����� ����
	static bool RegisterClass();

	// ������� ��������� ����
	bool Create();
	// �������� ����
	void Show(int cmdShow);

protected:
	// �������, ���������� ��� �������� ������������ ������� ���� (��������� WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// �������, ���������� ��� �������� ���������� ������� ���� (��������� WM_CREATE)
	void OnCreate();
	// �������, ���������� ��� ��������� �������� ���� (��������� WM_SIZE)
	void OnSize();
	// �������, ���������� ��� ����������� ���� (��������� WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // ����� ����
	std::vector<CEllipseWindow> children; // ������ �������� ����

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

