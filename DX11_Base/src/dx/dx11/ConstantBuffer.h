#pragma once

#include <memory>
#include <DirectXMath.h>

#include "ShaderStage.h"

namespace tama 
{
	/**@brief �ނ��o���̒萔�o�b�t�@
	*@detail �O���Ƀf�[�^��ێ����Ă��鎞�Ɏg�p����
	*/
	class ConstantBufferBase
	{
		class Impl;
		std::unique_ptr<Impl> m_impl;
	public:
		/**@param[in] initialData �����f�[�^
		*@param[in] size �f�[�^�^�̃T�C�Y(sizeof(Type))
		*/
		ConstantBufferBase(const void* initialData, size_t size);
		void Set(const void* data, uint32_t slot, ShaderStage bindStage) const;
		~ConstantBufferBase();
	};

	template<class Type>
	class ConstantBuffer
	{
	private:
		enum { Size = sizeof(Type) };

		static_assert(Size <= 16 * 4096, "Size <= 16 * 4096");
		static_assert(Size % 16 == 0, "Size % 16 == 0");

		ConstantBufferBase m_base;
		
		std::unique_ptr<Type> m_data;

	public:
		/**@brief �R���X�g���N�^*/
		explicit ConstantBuffer()
			: m_base(nullptr, Size)
		{
			m_data = std::make_unique<Type>();
		}

		/**@brief �R���X�g���N�^
		*@param[in] data ����������f�[�^*/
		explicit ConstantBuffer(const Type& data)
			: ConstantBuffer{}
		{
			*m_data = data;
		}

		void Set(uint32_t slot, ShaderStage bindStage) const
		{
			m_base.Set(m_data.get(), slot, bindStage);
		}

		/**@brief �萔�o�b�t�@���o�C���h
		*@attention �����̃f�[�^�ł͂Ȃ��O���̃f�[�^���o�C���h����*/
		void Set(const Type& data, uint32_t slot, ShaderStage bindStage) const
		{
			m_base.Set(&data, slot, bindStage);
		}

		/**@brief �f�[�^���擾*/
		Type& Get() { return *m_data; }

		/**@brief �f�[�^���擾*/
		Type& operator *() { return *m_data; }

		/**@brief �ύX�s�\�ȃf�[�^���擾*/
		const Type& operator *() const { return *m_data; }

		/**@brief �f�[�^�̃|�C���^�ɃA�N�Z�X*/
		Type* operator ->() { return m_data.get(); }

		/**@brief �ύX�s�\�ȃf�[�^�̃|�C���^�ɃA�N�Z�X*/
		const Type* operator ->() const { return m_data.get(); }

		/**@brief �f�t�H���g�f�X�g���N�^*/
		~ConstantBuffer() = default;
	};

}