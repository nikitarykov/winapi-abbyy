// ��������: ����� �OverlappedWindow. ������������ ����� C++ ��������
// ��� ������� ���� WinAPI.

#pragma once

#include <windows.h>
#include "EditControlWindow.h"
#include "SettingsDialog.h"

class COverlappedWindow
{
public:
	COverlappedWindow();
	~COverlappedWindow();

	// ���������������� ����� ����
	static bool RegisterClass();
	// ������� ��������� ����
	bool Create();
	// �������� ����
	void Show(int cmdShow);
	// ������ ��� ������ ����
	HWND GetHandle();
	// ������ ��� ������ �������
	HWND GetDialogHandle();
	// ������ ��� ����� ������
	int GetFontSize();
	// ������ ��� ����� ������
	COLORREF GetFontColor();
	// ������ ��� ����� ����
	COLORREF GetBackgroundColor();
	// ������ ��� ����� ������
	void SetFontSize(int size);
	// ������ ��� ����� ������
	void SetFontColor(COLORREF color);
	// ������ ��� ����� ����
	void SetBackgroundColor(COLORREF color);
	void ResetHasDialog();

protected:
	// �������, ���������� ��� �������� ������������ ������� ���� (��������� WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// �������, ���������� ��� �������� ���������� ������� ���� (��������� WM_CREATE)
	void OnCreate();
	// �������, ���������� ��� ��������� �������� ���� (��������� WM_SIZE)
	void OnSize();
	//�������, ���������� ��� ��������� ����� � EditControl(��������� WM_CTLCOLOREDIT)
	LRESULT OnControlColorEdit(HDC hdc);
	// �������, ���������� ��� ��������� ������ � Edit Control (��������� WM_COMMAND + EN_CHANGE)
	void OnTextChange();
	// �������, ���������� ��� �������� ���� � ������
	void OnCountWords();
	// �������, ���������� ��� �������� ���� (��������� WM_CLOSE)
	void OnClose();
	// �������, ���������� ��� ����������� ���� (��������� WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // ����� ����
	CEditControlWindow editControl; // �������� Edit Control
	CSettingsDialog dialog; // Dialog Box
	COLORREF fontColor; // ���� ������ � EditControl
	COLORREF backgroundColor; // ���� ���� � EditControl
	bool isEmpty; // ������ �� Edit Control
	bool hasDialog;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	// ������� ��� ���������� ������ � ����
	void saveFile();
	// ������� ��� ����������� ������� ��������
	void showSettings();
};

