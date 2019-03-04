/**
* @file ComInitializer.h
* @brief COM�I�u�W�F�N�g���g�p����ۂɂ͂��̃N���X���C���X�^���X������
*/
#pragma once

/**@brief COM�̏������Ɖ��������������N���X
*@remark �G���g���|�C���g�ɐÓI�ɃC���X�^���X������̂��]�܂���
*/
struct ComInitializer
{
	/**@brief �R���X�g���N�^
	*@remark COM�̏������Ɏ��s�������͗�O�𓊂���
	*/
	ComInitializer()
	{
		if (FAILED(::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))//COINITBASE_MULTITHREADED)))
			throw std::runtime_error("CoInitializeEx");
	}
	/**@brief �f�X�g���N�^*/
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
