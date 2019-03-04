#pragma once
#include "Texture.h"
#include "../../base/Math.h"

namespace tama 
{
	// レンダーターゲットを変えるときにd2dも変えないといけない
	class RenderTexture final : public Texture 
	{
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;

	public:
		RenderTexture(uint32_t width, uint32_t height);

		void SetRenderTarget(bool clear = true) const;

		~RenderTexture();
	};
}
