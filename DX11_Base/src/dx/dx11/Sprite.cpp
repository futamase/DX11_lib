#include "Sprite.h"

namespace tama
{
	std::unique_ptr<DX11::SpriteBatch> Sprite::s_spriteBatch = nullptr;

	Sprite::Sprite()
	{
		if (!s_spriteBatch)
			s_spriteBatch = std::make_unique<DX11::SpriteBatch>();
	}

	void Sprite::SetTexture(const Texture& texture)
	{
		m_pTexture = &texture;
	}

	void Sprite::SetTexture(const TextureRegion& texture)
	{
		m_pTexture = texture.m_pTexture;
	}

	Sprite& Sprite::Flip(bool flag)
	{
		m_isFlipVertically = flag;
		return *this;
	}

	Sprite& Sprite::Mirror(bool flag)
	{
		m_isFlipHorizontally = flag;
		return *this;
	}

	Sprite Sprite::Flip() const
	{
		Sprite sprite(*this);
		sprite.m_isFlipVertically = true;
		return sprite;
	}

	Sprite Sprite::Mirror() const
	{
		Sprite sprite(*this);
		sprite.m_isFlipHorizontally = true;
		return sprite;
	}

	void Sprite::DrawImmediate(const ColorF& color, float layer) const
	{
		this->DrawImmediate(*m_pTexture, color, layer);
	}

	void Sprite::DrawImmediate(const Texture& tex, const ColorF& color, float layer) const
	{
		const auto size = tex.GetSize();
		const RECT rc{ 0, 0, static_cast<LONG>(size.x) , static_cast<LONG>(size.y) };

		s_spriteBatch->Begin(DX11::SpriteSortMode::Immediate);

		s_spriteBatch->Draw(
			tex.GetData(),
			m_position,
			&rc,
			color,
			m_rotation,
			m_rotationOrigin,
			m_scaling,
			this->GetEffects(),
			layer);

		s_spriteBatch->End();
	}

	void Sprite::DrawImmediate(const Texture& tex, const Vec2& pos, const Vec2& scale, const ColorF& color)
	{
		const auto size = tex.GetSize();
		const RECT rc{ 0, 0, static_cast<LONG>(size.x) , static_cast<LONG>(size.y) };

		s_spriteBatch->Begin(DX11::SpriteSortMode::Immediate);

		s_spriteBatch->Draw(
			tex.GetData(),
			pos,
			&rc,
			color,
			0.f,
			{ 0,0 },
			scale,
			DX11::SpriteEffects::None,
			0);

		s_spriteBatch->End();
	}

	void Sprite::DrawCenterImmediate() const
	{
		auto tex = *m_pTexture;
		auto color = ColorF(1,1,1,1);

		const auto size = tex.GetSize();
		const RECT rc{ 0,0, static_cast<LONG>(size.x) , static_cast<LONG>(size.y) };

		s_spriteBatch->Begin(DX11::SpriteSortMode::Immediate);

		auto pos = Vec2(m_position.x + size.x / 2.f, m_position.y + size.y / 2.f);

		s_spriteBatch->Draw(
			tex.GetData(),
			pos,
			&rc,
			color,
			m_rotation,
			{ size.x / 2.f, size.y / 2.f },
			m_scaling,
			this->GetEffects(),
			0);

		s_spriteBatch->End();
	}

	void Sprite::DrawLookAt(const Vec2& pos) const
	{
		auto tex = *m_pTexture;
		auto color = ColorF(1,1,1,1);

		const auto size = tex.GetSize();
		const RECT rc{ 0,0, static_cast<LONG>(size.x) , static_cast<LONG>(size.y) };

		s_spriteBatch->Begin(DX11::SpriteSortMode::Immediate);

		auto origin = Vec2(m_position.x + pos.x, m_position.y + pos.y);

		s_spriteBatch->Draw(
			tex.GetData(),
			origin,
			&rc,
			color,
			m_rotation,
			{ pos.x , pos.y },
			m_scaling,
			this->GetEffects(),
			0);

		s_spriteBatch->End();
	}

	void Sprite::begin()
	{
		s_spriteBatch->Begin(DX11::SpriteSortMode::Immediate);
	}
	void Sprite::end()
	{
		s_spriteBatch->End();
	}

	void Sprite::Draw() const
	{
		const auto size = m_pTexture->GetSize();
		const RECT rc{ 0, 0, static_cast<LONG>(size.x) , static_cast<LONG>(size.y) };


		s_spriteBatch->Draw(
			m_pTexture->GetData(),
			m_position,
			&rc,
			DX11::Colors::White,
			m_rotation,
			m_rotationOrigin,
			m_scaling,
			this->GetEffects(),
			0);
	}

	void Sprite::Draw(const TextureRegion& tex) const
	{
		auto rect = tex.GetRect();
		const RECT rc{ rect.x, rect.y, rect.x + rect.w, rect.y + rect.h };

		s_spriteBatch->Begin(DX11::SpriteSortMode::Immediate);

		static auto ccc = DX11::GetImmediateContext();
		//		ccc->PSSetShader(ps.Get(), nullptr, 0);

		s_spriteBatch->Draw(
			tex.m_pTexture->GetData(),
			//			m_position,
			{ 0,0 },
			&rc,
			ColorF(),
			m_rotation,
			m_rotationOrigin,
			m_scaling,
			this->GetEffects(),
			0);

		s_spriteBatch->End();
	}

	//	void Draw(DirectX::SpriteBatch* renderer, const Texture& tex) const
	//	{
	//		const auto size = tex.GetSize();
	//		const RECT rc{ 0, 0, static_cast<LONG>(size.x) , static_cast<LONG>(size.y) };
	//		renderer->Draw(tex.GetData(),
	//			m_position,
	//			&rc,
	//			DirectX::Colors::White,
	//			m_rotation,
	//			m_rotationOrigin,
	//			m_scaling,
	//			this->GetEffects());
	//	}

	//	void Draw(DirectX::SpriteBatch* renderer, const Texture& tex, const DirectX::SimpleMath::Vector4& color) const
	//	{
	//		const auto size = tex.GetSize();
	//		const RECT rc{ 0, 0, static_cast<LONG>(size.x), static_cast<LONG>(size.y) };
	//		renderer->Draw(tex.GetData(),
	//			m_position,
	//			&rc,
	//			color,
	//			m_rotation,
	//			m_rotationOrigin,
	//			m_scaling,
	//			this->GetEffects());
	//	}

	//	void Draw(DirectX::SpriteBatch* renderer, TextureRegion& tex) const
	//	{
	//		auto rect = tex.GetRect();
	//		const RECT rc{ rect.x, rect.y, rect.x + rect.w, rect.y + rect.h };
	//		renderer->Draw(tex.m_pTexture->GetData(),
	//			m_position,
	//			&rc,
	//			DirectX::Colors::White,
	//			m_rotation,
	//			m_rotationOrigin,
	//			m_scaling,
	//			this->GetEffects());
	//	}
#if 0
	struct VertexPosTex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 uv;
	};

	void SpriteBatch::CreateIndexBuffer()
	{
		std::vector<short> indices;
		indices.reserve(MaxBatchSize * IndicesPerSprite);
		for (short i = 0; i < MaxBatchSize * VerticesPerSprite; i += VerticesPerSprite)
		{
			indices.emplace_back(i);
			indices.emplace_back(i + 1);
			indices.emplace_back(i + 2);

			indices.emplace_back(i + 1);
			indices.emplace_back(i + 3);
			indices.emplace_back(i + 2);
		}

		m_indexBuffer = CreateBuffer(indices.data(), indices.size(), 0, D3D11_BIND_INDEX_BUFFER);
	}

	void SpriteBatch::CreateVertexBuffer()
	{
		m_vertexBuffer = CreateBuffer(
			nullptr,
			sizeof(VertexPosTex) * MaxBatchSize * VerticesPerSprite,
			D3D11_CPU_ACCESS_WRITE,
			D3D11_BIND_VERTEX_BUFFER);
	}

	void SpriteBatch::CreateShader()
	{
		auto path = _T("Assets/Shader/Sprite.hlsl");
		m_resource.vs = VertexShader(path, "VS");
		m_resource.ps = PixelShader(path, "PS");

		m_resource.layout = InputLayout(m_resource.vs);
	}

	void SpriteBatch::CreateDefaultState()
	{
		m_resource.bs = BlendState(BlendState::Predefined::Alphablend);
		m_resource.dss = DepthStencilState(DepthStencilState::Predefined::Default2D);
		m_resource.rs = RasterizerState(RasterizerState::Predefined::Default2D);
		m_resource.ss = SamplerState(SamplerState::Predefined::Default2D);
	}

	void SpriteBatch::Begin(const std::function<void(SpriteRenderResource* pResource)>& func)
	{
		func(&m_resource);
	}

	void SpriteBatch::End()
	{

	}
#endif
}