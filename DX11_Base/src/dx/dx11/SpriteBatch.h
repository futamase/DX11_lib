#pragma once

#include <d3d11_1.h>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <functional>
#include <memory>


namespace DX11 
{
	using namespace DirectX;

    enum class SpriteSortMode
    {
        Deferred,
        Immediate,
        Texture,
        BackToFront,
        FrontToBack,
    };
    
    enum class SpriteEffects
    {
        None = 0,
        FlipHorizontally = 1,
        FlipVertically = 2,
        FlipBoth = FlipHorizontally | FlipVertically,
    };
    
    class SpriteBatch
    {
    public:
        explicit SpriteBatch();
        SpriteBatch(SpriteBatch&& moveFrom);
        SpriteBatch& operator= (SpriteBatch&& moveFrom);

        SpriteBatch(SpriteBatch const&) = delete;
        SpriteBatch& operator= (SpriteBatch const&) = delete;

        virtual ~SpriteBatch();

        // Begin/End a batch of sprite drawing operations.
        void XM_CALLCONV Begin(
			SpriteSortMode sortMode = SpriteSortMode::Deferred, 
			_In_opt_ ID3D11BlendState* blendState = nullptr, 
			_In_opt_ ID3D11SamplerState* samplerState = nullptr, 
			_In_opt_ ID3D11DepthStencilState* depthStencilState = nullptr, 
			_In_opt_ ID3D11RasterizerState* rasterizerState = nullptr,
            _In_opt_ std::function<void __cdecl()> setCustomShaders = nullptr, 
			FXMMATRIX transformMatrix = MatrixIdentity);
        
		void __cdecl End();

		void XM_CALLCONV SpriteBatch::Draw(
			_In_ ID3D11ShaderResourceView* texture,
			XMFLOAT2 const& position,
			_In_opt_ RECT const* sourceRectangle,
			FXMVECTOR color,
			float rotation,
			XMFLOAT2 const& origin,
			XMFLOAT2 const& scale,
			SpriteEffects effects,
			float layerDepth) const;

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;

        static const XMMATRIX MatrixIdentity;
        static const XMFLOAT2 Float2Zero;
    };
}
