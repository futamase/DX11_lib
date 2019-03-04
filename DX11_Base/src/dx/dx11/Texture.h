#pragma once

#include "Image.h"
#include <wrl.h>
#include "..\..\base\Math.h"
#include "..\..\base\Geometry.h"
#include "DXResource.h"

namespace tama
{
	class TextureRegion;
	class Texture
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
		tama::Vec2 m_size;

		void CalcSize()
		{
			auto size = DX11::GetTextureSize(m_srv.Get());
			m_size.x = size.x;
			m_size.y = size.y;
		}
	public:
		Texture() {}

		explicit Texture(const std::wstring& path)
		{
			this->Load(path);
		}
		explicit Texture(const Image& image)
		{
			m_srv = DX11::CreateShaderResourceView(image.m_tex.Get());
			this->CalcSize();
		}

		virtual ~Texture() {}

		bool operator == (const Texture& other)
		{
			return m_srv == other.m_srv;
		}

		void Load(const std::wstring& path)
		{
			m_srv = DX11::CreateShaderResourceView(path);
			this->CalcSize();
		}

		TextureRegion Region(const Rect rect) const;

		Vec2 GetSize() const { return m_size; }

		ID3D11ShaderResourceView* GetData() const { return m_srv.Get(); }
	};


	class Sprite;	

	class TextureRegion
	{
		friend class Sprite;

		Rect m_region;
		const Texture* m_pTexture;

	public:
		TextureRegion(const Texture& tex, const Rect& rect) :
			m_pTexture(&tex), m_region(rect)
		{}

		const Rect& GetRect() const
		{
			return m_region;
		}
	};
}