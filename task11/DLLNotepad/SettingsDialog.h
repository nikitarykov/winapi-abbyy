#pragma once

// Описание: Класс SettingsDialog. Реализует функционал диалога настроек.

#pragma once

#include <windows.h>

class CSettingsDialog
{
public:
	CSettingsDialog();
	~CSettingsDialog();

	// Геттер для хэндла диалога
	HWND GetHandle();
	// Создать экземпляр диалога
	bool Create(HWND& windowHandle);
protected:
	// Функция, вызываемая при создании диалога (сообщение WM_INITDIALOG)
	void OnInitDialog(HWND& _handle);
	// Функция, вызываемая при нажатии на CheckBox "предпросмотр" 
	// в DialogBox (сообщение WM_COMMAND + IDC_CHECK1)
	void OnCheckPreview();
	// Функция, вызываемая при перемещении ползунка "шрифт" 
	// в DialogBox (сообщение WM_COMMAND + WM_HSCROLL + IDC_SLIDER1)
	void OnFontChange();
	// Функция, вызываемая при перемещении ползунка "прозрачность" 
	// в DialogBox (сообщение WM_COMMAND + WM_HSCROLL + IDC_SLIDER2)
	void OnTransparencyChange();
	// Функция, вызываемая при изменении цвета шрифта
	void OnFontColorChange();
	// Функция, вызываемая при изменении цвета фона
	void OnBackgroundColorChange();
	// Функция, вызываемая при нажатии "Ок" в Dialog
	void OnOk(LPARAM wParam);
	// Функция, вызываемая при нажатии "Cancel" в Dialog
	void OnCancel(LPARAM wParam);

private:
	HWND handle; // хэндл DialogBox
	bool isPreview; // включен ли предпросмотр в DialogBox
	int oldFontSize; // размер шрифта в Edit Control до вызова Dialog
	int fontSize; // текущее положение ползунка "Размер шрифта"
	int oldTransparency; // прозрачность окна до вызова Dialog
	int transparency; // текущее положение ползунка "Прозрачность"
	COLORREF oldFontColor; // цвет шрифта в Edit Control до вызова Dialog
	COLORREF fontColor; // текущий цвет шрифта в ChooseColor "шрифт"
	COLORREF oldBackgroundColor; // цвет фона в Edit Control до вызова Dialog
	COLORREF backgroundColor; // текущий цвет шрифта в ChooseColor "фон"
	COLORREF custColors[16]; // массив для дополнительных цветов в диалоге выбора цвета


	static INT_PTR __stdcall dialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

