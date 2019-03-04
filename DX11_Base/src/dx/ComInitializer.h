/**
* @file ComInitializer.h
* @brief COMオブジェクトを使用する際にはこのクラスをインスタンス化する
*/
#pragma once

/**@brief COMの初期化と解放を自動化するクラス
*@remark エントリポイントに静的にインスタンス化するのが望ましい
*/
struct ComInitializer
{
	/**@brief コンストラクタ
	*@remark COMの初期化に失敗した時は例外を投げる
	*/
	ComInitializer()
	{
		if (FAILED(::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))//COINITBASE_MULTITHREADED)))
			throw std::runtime_error("CoInitializeEx");
	}
	/**@brief デストラクタ*/
	~ComInitializer()
	{

	}
	void Release()
	{
		::CoUninitialize();
	}

	ComInitializer(const ComInitializer &) = delete;
	ComInitializer(ComInitializer &&) = delete;
	ComInitializer &operator =(const ComInitializer &) = delete;
	ComInitializer &operator =(ComInitializer &&) = delete;
};
