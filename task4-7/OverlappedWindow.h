// Описание: Класс СOverlappedWindow. Представляет собой C++ оболочку
// над классом окна WinAPI.

#pragma once

#include <windows.h>
#include "EditControlWindow.h"
#include "SettingsDialog.h"

class COverlappedWindow
{
public:
	COverlappedWindow();
	~COverlappedWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass();
	// Создать экземпляр окна
	bool Create();
	// Показать окно
	void Show(int cmdShow);
	// Геттер для хэндла окна
	HWND GetHandle();
	// Геттер для хэндла диалога
	HWND GetDialogHandle();
	// Геттер для цвета шрифта
	COLORREF GetFontColor();
	// Геттер для цвета фона
	COLORREF GetBackgroundColor();
	// Сеттер для цвета шрифта
	void SetFontColor(COLORREF color);
	// Сеттер для цвета фона
	void SetBackgroundColor(COLORREF color);

protected:
	// Функция, вызываемая при создании неклиентской области окна (сообщение WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// Функция, вызываемая при создании клиентской области окна (сообщение WM_CREATE)
	void OnCreate();
	// Функция, вызываемая при изменении размеров окна (сообщение WM_SIZE)
	void OnSize();
	//Функция, вызываемая при изменении цвета в EditControl(сообщение WM_CTLCOLOREDIT)
	LRESULT OnControlColorEdit(HDC hdc);
	// Функция, вызываемая при изменении текста в Edit Control (сообщение WM_COMMAND + EN_CHANGE)
	void OnTextChange();
	// Функция, вызываемая при закрытии окна (сообщение WM_CLOSE)
	void OnClose();
	// Функция, вызываемая при уничтожении окна (сообщение WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // хэндл окна
	CEditControlWindow editControl; // дочерний Edit Control
	CSettingsDialog dialog; // Dialog Box
	COLORREF fontColor; // цвет шрифта в EditControl
	COLORREF backgroundColor; // цвет фона в EditControl
	bool isEmpty; // пустой ли Edit Control

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	// Функция для сохранения текста в файл
	void saveFile();
	// Функция для отображения диалога настроек
	void showSettings();
};

