#ifndef _DX_INPUT_H
#define _DX_INPUT_H

//#define SAFE_DELETE(p) {if(p) {delete (p); (p) = NULL;}}


#define	NC_DX_PROP_MIN	-1000
#define	NC_DX_PROP_MAX	1000

#define	PS2_Z	0 // △
#define	PS2_X	1 // ○
#define	PS2_C	2 // ×
#define	PS2_V	3 // □1
#define PS2_L2	4
#define PS2_R2	5
#define PS2_L1	6
#define PS2_R1	7
#define PS2_START	8
#define PS2_SELECT	9

#include <cstdint>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

enum class KeyCode : uint16_t
{
	A = DIK_A,
	B = DIK_B,
	C = DIK_C,
	D = DIK_D,
	E = DIK_E,
	F = DIK_F,
	G = DIK_G,
	H = DIK_H,
	I = DIK_I,
	J = DIK_J,
	K = DIK_K,
	L = DIK_L,
	M = DIK_M,
	N = DIK_N,
	O = DIK_O,
	P = DIK_P,
	Q = DIK_Q,
	R = DIK_R,
	S = DIK_S,
	T = DIK_T,
	U = DIK_U,
	V = DIK_V,
	W = DIK_W,
	X = DIK_X,
	Y = DIK_Y,
	Z = DIK_Z,

	Slash = DIK_SLASH,
	Peliod = DIK_PERIOD,
	Comma = DIK_COMMA,
	BackSlash = DIK_BACKSLASH,
	LBlacket = DIK_LBRACKET,
	RBlacket = DIK_RBRACKET,
	Colon = DIK_COLON,
	SemiColon = DIK_SEMICOLON,
	AtMark = DIK_AT,

	Num0 = DIK_0,
	Num1 = DIK_1,
	Num2 = DIK_2,
	Num3 = DIK_3,
	Num4 = DIK_4,
	Num5 = DIK_5,
	Num6 = DIK_6,
	Num7 = DIK_7,
	Num8 = DIK_8,
	Num9 = DIK_9,
	Pad0 = DIK_0,
	Pad1 = DIK_1,
	Pad2 = DIK_2,
	Pad3 = DIK_3,
	Pad4 = DIK_4,
	Pad5 = DIK_5,
	Pad6 = DIK_6,
	Pad7 = DIK_7,
	Pad8 = DIK_8,
	Pad9 = DIK_9,
	F1 = DIK_F1,
	F2 = DIK_F2,
	F3 = DIK_F3,
	F4 = DIK_F4,
	F5 = DIK_F5,
	F6 = DIK_F6,
	F7 = DIK_F7,
	F8 = DIK_F8,
	F9 = DIK_F9,
	F10 = DIK_F10,
	F11 = DIK_F11,
	F12 = DIK_F12,
	F13 = DIK_F13,
	F14 = DIK_F14,
	Esc = DIK_ESCAPE,
	LShift = DIK_LSHIFT,
	RShift = DIK_RSHIFT,
	BackSpace = DIK_BACKSPACE,
	Delete = DIK_DELETE,
	Return = DIK_RETURN,
	LCtr = DIK_LCONTROL,
	RCtr = DIK_RCONTROL,
	LAlt = DIK_LALT,
	RAlt = DIK_LALT,
	Left = DIK_LEFT,
	Right = DIK_RIGHT,
	Up = DIK_UP,
	Down = DIK_DOWN,
};

class DXInput
{
public:
	HRESULT		Setup(HINSTANCE hInst, HWND hWnd);
	void		Cleanup();
	void		Update();

	// キーボード
	bool		GetIsJustPressedKeyboard(KeyCode code);
	bool		GetIsJustPulledKeyboard(KeyCode code);
	bool		GetIsPressedKeyboard(KeyCode code);
	bool		GetIsFreeKeyboard(KeyCode code);

	// マウス
	bool		GetIsJustPressedMouse(long i);
	bool		GetIsJustPulledMouse(long i);
	bool		GetIsPressedMouse(long i);
	bool		GetIsFreeMouse(long i);

	// ジョイパッド
	bool		GetIsJustPressedJoypad(long iPadID, long i);
	bool		GetIsJustPulledJoypad(long iPadID, long i);
	bool		GetIsPressedJoypad(long iPadID, long i);
	bool		GetIsFreeJoypad(long iPadID, long i);

	// 左スティック
	bool		GetIsStickUp(long iPadID, long Sens = -300);
	bool		GetIsStickDown(long iPadID, long Sens = 300);
	bool		GetIsStickLeft(long iPadID, long Sens = -300);
	bool		GetIsStickRight(long iPadID, long Sens = 300);

	float		GetHorizontal(long iPadID);
	float		GetVertical(long iPadID);

	// ジョイパッド関連-----------------
	DIJOYSTATE2 *	GetJoypadState(long i)	{ return &m_JoypadState[i][m_iStateIndex]; }
	DIJOYSTATE2 *	GetPreJoypadState(long i){ return &m_JoypadState[i][1 - m_iStateIndex]; }

	long		GetStateIndex()	const{ return m_iStateIndex; }
	long		GetDetectJoypadCount(){ return m_iDetectJoypadCount; }
	// ----------------------------------

private:
	DXInput();
	~DXInput();
	DXInput(const DXInput&) = delete;
	DXInput& operator= (const DXInput&) = delete;

public:
	static DXInput* GetInstance()
	{
		static DXInput instance;
		return &instance; 
	}

private:
	HRESULT		SetupKeyboard(HWND hWnd);
	HRESULT		SetupJoypad(HWND hWnd);
	HRESULT		SetupMouse(HWND hWnd);


	void		UpdateKeyboardState();
	void		UpdateMouseState();
	void		UpdateJoypadState();

	// ジョイパッド関連----------
	static BOOL CALLBACK	GetJoyPadCallback(DIDEVICEINSTANCE *pDidIns, void *pThis);
	static BOOL CALLBACK	EnumObjectCallback(DIDEVICEOBJECTINSTANCE *pDidIns, void *pThis);
	// --------------------------

	enum{ JOYPAD_MAX = 4 };
	LPDIRECTINPUT8			m_pInput;			//!<	デバイスへのポインタ
	LPDIRECTINPUTDEVICE8	m_pKeyDevice;		//!<	キーボードデバイス
	LPDIRECTINPUTDEVICE8	m_pMouseDevice;		//!<	マウスデバイス
	LPDIRECTINPUTDEVICE8	m_pJoyPadDevice[JOYPAD_MAX];	//!<	ジョイパッドデバイス

	long					m_iDetectJoypadCount;	//!<	検出したジョイパッドの数
	long					m_iSettingJoypad;		//!<	設定しているジョイパッド

	long					m_iStateIndex;			//!<	状態のインデックス

	DIMOUSESTATE2			m_MouseState[2];		//!<	マウスの状態
	DIJOYSTATE2				m_JoypadState[JOYPAD_MAX][2];//!<	ジョイパッドの状態
	unsigned char			m_KeyboardState[2][256];	 //!<	キーボードの状態
};

#endif
