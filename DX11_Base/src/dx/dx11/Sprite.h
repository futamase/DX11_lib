#pragma once

#include <vector>
#include "DX11Device.h"
#include "DXResource.h"
#include "RenderState.h"
#include "Shader.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Shader.h"
#include "../../base/Geometry.h"
#include "../../base/math.h"
#include "../../base/Transform.h"

namespace tama
{
	class Sprite : public Transform2D
	{
		const Texture* m_pTexture;
		Rect m_sourceRect; // 画像の描画する範囲(デフォルトは画像サイズ)
		bool m_isFlipVertically = false;
		bool m_isFlipHorizontally = false;

		static std::unique_ptr<DX11::SpriteBatch> s_spriteBatch;

		DX11::SpriteEffects GetEffects() const
		{
			if (m_isFlipHorizontally && m_isFlipVertically)
				return DX11::SpriteEffects::FlipBoth;
			if (m_isFlipHorizontally)
				return DX11::SpriteEffects::FlipHorizontally;
			if (m_isFlipVertically)
				return DX11::SpriteEffects::FlipVertically;
			return DX11::SpriteEffects::None;
		}
	public:
		Sprite();

		void SetTexture(const Texture& texture);

		void SetTexture(const TextureRegion& texture);

		Sprite& Flip(bool flag);

		Sprite& Mirror(bool flag);

		Sprite Flip() const;

		Sprite Mirror() const;

		void DrawImmediate(const ColorF& color = Palette::White, float layer = 0) const;
		void DrawImmediate(
			const Texture& tex,
			const ColorF& color,
			float layer) const;

		static void DrawImmediate(const Texture& tex, const Vec2& pos = Vec2(0,0), const Vec2& scale = Vec2(1,1), const ColorF& color = Palette::White);

		void DrawCenterImmediate() const;

		void DrawLookAt(const Vec2& pos) const;

		void Draw() const;

		void Draw(const TextureRegion& tex) const;


		static void begin();
		static void end();
//		void Draw(SpriteBatch* renderer, const Texture& tex) const;

//		void Draw(SpriteBatch* renderer, const Texture& tex, const DirectX::SimpleMath::Vector4& color) const;

//		void Draw(SpriteBatch* renderer, TextureRegion& tex) const;
	};
	/*
		struct SpriteRenderResource
		{
			VertexShader vs;
			PixelShader ps;
			InputLayout layout;

			RasterizerState rs;
			SamplerState ss;
			DepthStencilState dss;
			BlendState bs;
		};

		struct RenderState
		{

		};

		class SpriteBatch
		{
		private:
			static const size_t MaxBatchSize = 2048;
			static const size_t MinBatchSize = 128;
			static const size_t VerticesPerSprite = 4;
			static const size_t IndicesPerSprite = 6;

			ComPtr<ID3D11Buffer> m_vertexBuffer, m_indexBuffer;
			ID3D11DeviceContext* m_context;

			SpriteRenderResource m_resource;

		public:
			SpriteBatch(ID3D11DeviceContext* context)
			{
				m_context = context;
				this->CreateIndexBuffer();
				this->CreateVertexBuffer();
				this->CreateShader();
				this->CreateDefaultState();
			}

			void Begin(const std::function<void(SpriteRenderResource* pResource)>& func = [](SpriteRenderResource* pResource) {});

			void End();

		private:
			void CreateIndexBuffer();
			void CreateVertexBuffer();
			void CreateShader();
			void CreateDefaultState();
		};

	*/
}