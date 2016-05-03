// ��������: ����� �FieldWindow. ������������ �����  ����� ����
// ��� �������� "����"

#pragma once

#include <windows.h>
#include "CardWindow.h"
#include <vector>

class CFieldWindow
{
public:
	CFieldWindow();
	~CFieldWindow();

	// ���������������� ����� ����
	static bool RegisterClass();
	// ������� ��������� ����
	bool Create();
	// �������� ����
	void Show(int commandShow);
	
protected:
	// �������, ���������� ��� �������� ������������ ������� ���� (��������� WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// �������, ���������� ��� �������� ���������� ������� ���� (��������� WM_CREATE)
	void OnCreate();
	// �������, ���������� ��� ��������� �������� ���� (��������� WM_SIZE)
	void OnSize();
	// �������, ���������� ��� �������� ���� (��������� WM_CLOSE)
	void OnClose();
	// �������, ���������� ��� ����������� ���� (��������� WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // ����� ����
	std::vector<CCardWindow> cards;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

};

