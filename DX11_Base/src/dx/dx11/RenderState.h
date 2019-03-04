#pragma once

#include "BlendState.h"
#include "SamplerState.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"

#include <string>


namespace Graphics
{
	void SetBlendState(const tama::BlendState& state);
	void SetSamplerState(const tama::SamplerState& state, uint32_t slot);
	void SetRasterizerState(const tama::RasterizerState& state);
	void SetDepthStencilState(const tama::DepthStencilState& state);
}
