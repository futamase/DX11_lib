/**
* @file Window.h
* @brief �E�B���h�E���Ǘ�����
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
	
	/**@brief �E�B���h�E���Ǘ�����
	*/
	class Window
	{
	private:
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		tstring m_appName;
		UINT m_width, m_height;  //! �s�v�ɂȂ邩��
	public:
		/**@brief �R���X�g���N�^
		*@param[in] AppName �^�C�g���o�[�ɕ\�����閼�O
		*@param[in] hInstance �C���X�^���X�n���h��
		*@param[in] width �E�B���h�E�̕�
		*@param[in] height �E�B���h�E�̍���
		*/
		Window(const tstring& AppName, HINSTANCE hInstance, UINT width, UINT height);

		/**@brief �f�X�g���N�^*/
		~Window();

		/**@brief �E�B���h�E�n���h����Ԃ�
		*@return �E�B���h�E�n���h��
		*/
		HWND GetHWnd() const { return m_hWnd; }

		/**@brief �E�B���h�E���X�N���[���̒����Ɉړ�����
		*/
		void MoveToCenter();

		/**@brief �E�B���h�E�̕���Ԃ�
		*@return �E�B���h�E�̕�
		*/
		static float Width();

		/**@brief �E�B���h�E�̍�����Ԃ�
		*@return �E�B���h�E�̍���
		*/
		static float Height();

		static void CursorEnable(bool enable);

		/**@brief �A�C�R����ݒ肷��(������)
		*@param fileName �摜�t�@�C���ւ̃p�X
		*/
		void SetIcon(const tstring& fileName);

		BOOL SetTitle(const tstring& title);

		static Window* I() { return s_instance; }
	private:
		static Window* s_instance;
	};

	BOOL Resize(UINT width, UINT height);
}

