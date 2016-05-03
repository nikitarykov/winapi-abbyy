// Описание: Класс СFieldWindow. Представляет собой  класс поля
// для пасьянса "Паук"

#pragma once

#include <windows.h>
#include "CardWindow.h"
#include <vector>

class CFieldWindow
{
public:
	CFieldWindow();
	~CFieldWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass();
	// Создать экземпляр окна
	bool Create();
	// Показать окно
	void Show(int commandShow);
	
protected:
	// Функция, вызываемая при создании неклиентской области окна (сообщение WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// Функция, вызываемая при создании клиентской области окна (сообщение WM_CREATE)
	void OnCreate();
	// Функция, вызываемая при изменении размеров окна (сообщение WM_SIZE)
	void OnSize();
	// Функция, вызываемая при закрытии окна (сообщение WM_CLOSE)
	void OnClose();
	// Функция, вызываемая при уничтожении окна (сообщение WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // хэндл окна
	std::vector<CCardWindow> cards;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

};

