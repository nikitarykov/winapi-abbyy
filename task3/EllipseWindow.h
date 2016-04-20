// Описание: Класс СEllipseWindow. Представляет собой C++ оболочку
// над классом окна с движущимся эллипсом.

#pragma once

#include <windows.h>

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass();

	// Создать экземпляр окна
	bool Create(HWND& handleParent);
	// Показать окно
	void Show(int cmdShow);
	// Геттер для хэндла окна
	HWND getHandle();

protected:
	// Функция, вызываемая при создании неклиентской области окна (сообщение WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// Функция, вызываемая при создании клиентской области окна (сообщение WM_CREATE)
	void OnCreate();
	// Функция, вызываемая при перерисовке окна (сообщение WM_PAINT)
	void OnPaint();
	// Функция, вызываемая при срабатывании таймера (сообщение WM_TIMER)
	void OnTimer();
	// Функция, вызываемая при нажатии на окно (сообщение WM_LBUTTONDOWN)
	void OnButtonDown();
	// Функция, вызываемая при уничтожении окна (сообщение WM_DESTROY)
	void OnDestroy();

	// Перерисовка эллипса
	void drawEllipse(HDC& memoryDC, RECT& rect);
private:
	HWND handle; // хэндл окна
	UINT_PTR timer; // таймер для перерисовки
	int xSpeed; // скорость эллипса по оси x
	int ySpeed; // скорость эллипса по оси y
	int x; // координата центра эллипса по оси x
	int y; // координата центра эллипса по оси y
	int tickCount; // количество сообщений от таймера
	int paintCount; // количество перерисовок
	static const int aAxis = 100; // главная ось эллипса
	static const int bAxis = 60; // побочная ось эллипса

	// методы для перерасчета координат центра эллипса
	void calcX(const RECT& rect);
	void calcY(const RECT& rect);
	
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

