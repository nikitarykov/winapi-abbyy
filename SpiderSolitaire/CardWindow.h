#pragma once

class CPile;
#include <windows.h>

class CCardWindow
{
public:
	CCardWindow();
	CCardWindow( int _frontId );
	~CCardWindow();

	CCardWindow(CCardWindow&&) = default;
	CCardWindow& operator=(CCardWindow&&) = default;

	CCardWindow(const CCardWindow&) = delete;
	CCardWindow& operator=(const CCardWindow&) = delete;

	// Зарегистрировать класс окна
	static bool RegisterClass();
	// Создать экземпляр окна
	bool Create(HWND& handleParent);
	// Показать окно
	void Show(int commandShow);
	// Геттер для хэндла окна
	HWND& GetHandle();
	// Открыта ли лицевая часть карты
	bool IsOpen();
	// Открыть лицевую часть карты
	void Open();
	void Close();
	void SetPile( CPile* _pile );
	CPile* GetPile();
	int GetId();
	void SetPileIndex( int index );
	int GetPileIndex();
	void SetDragged();
	
protected:
	// Функция, вызываемая при создании неклиентской области окна (сообщение WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// Функция, вызываемая при создании клиентской области окна (сообщение WM_CREATE)
	void OnCreate();
	// Функция, вызываемая при перерисовке карты (сообщение WM_PAINT)
	void OnPaint();
	void OnButtonDown( LPARAM lParam );
	void OnMouseMove( LPARAM lParam );
	void OnButtonUp( LPARAM lParam );
	void OnCaptureChanged( LPARAM lParam );
	// Функция, вызываемая при уничтожении окна (сообщение WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // хэндл окна
	bool isOpen; // открыта ли лицевая часть карты
	static const HBITMAP backBitMap;
	static const HBITMAP frontBitMaps[14];
	CPile* pile;
	int pileIndex;
	int id;
	bool isDragged;
	int mousePosX;
	int mousePosY;

	void drawCard(HDC handleDC);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};


