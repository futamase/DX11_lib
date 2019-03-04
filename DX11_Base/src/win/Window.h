/**
* @file Window.h
* @brief ウィンドウを管理する
*/
#pragma once

#include <cstdio>       
#include <Windows.h>
#include <tchar.h>      
#include <string>

/**@namespace Win
*/
namespace Win
{
	using tstring = std::basic_string < TCHAR >;
	
	/**@brief ウィンドウを管理する
	*/
	class Window
	{
	private:
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		tstring m_appName;
		UINT m_width, m_height;  //! 不要になるかも
	public:
		/**@brief コンストラクタ
		*@param[in] AppName タイトルバーに表示する名前
		*@param[in] hInstance インスタンスハンドル
		*@param[in] width ウィンドウの幅
		*@param[in] height ウィンドウの高さ
		*/
		Window(const tstring& AppName, HINSTANCE hInstance, UINT width, UINT height);

		/**@brief デストラクタ*/
		~Window();

		/**@brief ウィンドウハンドルを返す
		*@return ウィンドウハンドル
		*/
		HWND GetHWnd() const { return m_hWnd; }

		/**@brief ウィンドウをスクリーンの中央に移動する
		*/
		void MoveToCenter();

		/**@brief ウィンドウの幅を返す
		*@return ウィンドウの幅
		*/
		static float Width();

		/**@brief ウィンドウの高さを返す
		*@return ウィンドウの高さ
		*/
		static float Height();

		static void CursorEnable(bool enable);

		/**@brief アイコンを設定する(未実装)
		*@param fileName 画像ファイルへのパス
		*/
		void SetIcon(const tstring& fileName);

		BOOL SetTitle(const tstring& title);

		static Window* I() { return s_instance; }
	private:
		static Window* s_instance;
	};

	BOOL Resize(UINT width, UINT height);
}

