#pragma once

#include <memory>
#include <DirectXMath.h>

#include "ShaderStage.h"

namespace tama 
{
	/**@brief むき出しの定数バッファ
	*@detail 外部にデータを保持している時に使用する
	*/
	class ConstantBufferBase
	{
		class Impl;
		std::unique_ptr<Impl> m_impl;
	public:
		/**@param[in] initialData 初期データ
		*@param[in] size データ型のサイズ(sizeof(Type))
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
		/**@brief コンストラクタ*/
		explicit ConstantBuffer()
			: m_base(nullptr, Size)
		{
			m_data = std::make_unique<Type>();
		}

		/**@brief コンストラクタ
		*@param[in] data 初期化するデータ*/
		explicit ConstantBuffer(const Type& data)
			: ConstantBuffer{}
		{
			*m_data = data;
		}

		void Set(uint32_t slot, ShaderStage bindStage) const
		{
			m_base.Set(m_data.get(), slot, bindStage);
		}

		/**@brief 定数バッファをバインド
		*@attention 内部のデータではなく外部のデータをバインドする*/
		void Set(const Type& data, uint32_t slot, ShaderStage bindStage) const
		{
			m_base.Set(&data, slot, bindStage);
		}

		/**@brief データを取得*/
		Type& Get() { return *m_data; }

		/**@brief データを取得*/
		Type& operator *() { return *m_data; }

		/**@brief 変更不可能なデータを取得*/
		const Type& operator *() const { return *m_data; }

		/**@brief データのポインタにアクセス*/
		Type* operator ->() { return m_data.get(); }

		/**@brief 変更不可能なデータのポインタにアクセス*/
		const Type* operator ->() const { return m_data.get(); }

		/**@brief デフォルトデストラクタ*/
		~ConstantBuffer() = default;
	};

}