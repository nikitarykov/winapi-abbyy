#pragma once

// ��������: ����� SettingsDialog. ��������� ���������� ������� ��������.

#pragma once

#include <windows.h>
#include "EditControlWindow.h"

class CSettingsDialog
{
public:
	CSettingsDialog();
	~CSettingsDialog();

	// ������ ��� ������ �������
	HWND GetHandle();
	// ������� ��������� �������
	bool Create(HWND& windowHandle);
	// ������ ��� Edit Control
	void SetEditControl(CEditControlWindow& _editControl);
protected:
	// �������, ���������� ��� �������� ������� (��������� WM_INITDIALOG)
	void OnInitDialog(HWND& _handle);
	// �������, ���������� ��� ������� �� CheckBox "������������" 
	// � DialogBox (��������� WM_COMMAND + IDC_CHECK1)
	void OnCheckPreview();
	// �������, ���������� ��� ����������� �������� "�����" 
	// � DialogBox (��������� WM_COMMAND + WM_HSCROLL + IDC_SLIDER1)
	void OnFontChange();
	// �������, ���������� ��� ����������� �������� "������������" 
	// � DialogBox (��������� WM_COMMAND + WM_HSCROLL + IDC_SLIDER2)
	void OnTransparencyChange();
	// �������, ���������� ��� ��������� ����� ������
	void OnFontColorChange();
	// �������, ���������� ��� ��������� ����� ����
	void OnBackgroundColorChange();
	// �������, ���������� ��� ������� "��" � Dialog
	void OnOk();
	// �������, ���������� ��� ������� "Cancel" � Dialog
	void OnCancel();

private:
	HWND handle; // ����� DialogBox
	CEditControlWindow editControl; // Edit Control, � ������� ������������ ���������
	bool isPreview; // ������� �� ������������ � DialogBox
	int oldFontSize; // ������ ������ � Edit Control �� ������ Dialog
	int fontSize; // ������� ��������� �������� "������ ������"
	int oldTransparency; // ������������ ���� �� ������ Dialog
	int transparency; // ������� ��������� �������� "������������"
	COLORREF oldFontColor; // ���� ������ � Edit Control �� ������ Dialog
	COLORREF fontColor; // ������� ���� ������ � ChooseColor "�����"
	COLORREF oldBackgroundColor; // ���� ���� � Edit Control �� ������ Dialog
	COLORREF backgroundColor; // ������� ���� ������ � ChooseColor "���"
	COLORREF custColors[16]; // ������ ��� �������������� ������ � ������� ������ �����


	static INT_PTR __stdcall dialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

