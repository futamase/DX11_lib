#pragma once

#include "ConstantBuffer.h"
#include "RenderTexture.h"
#include "../../base/Math.h"

#include <memory>

namespace tama
{
	template<typename Desc>
	class Camera : public Desc
	{
	private:
		ConstantBufferBase m_buffer;

		Vec3 m_pos;
		Vec3 m_focus;
		Vec3 m_qua;
		Vec2 m_screenSize;

		bool rockOnFlag = false;

		const float PROJECTION_DEGREE = 60.f;
		float screenAspect = 0.f;

		std::unique_ptr<RenderTexture> m_rt;

	public:

	};
}
