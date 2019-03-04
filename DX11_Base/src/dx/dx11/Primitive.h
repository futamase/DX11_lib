#pragma once

#include "../../Utility/Transform.h"
#include <memory>

namespace Primitive
{
	struct Cube : public Transform3D
	{
		struct Impl;
		std::unique_ptr<Impl> m_impl;

	public:
		Cube();
		~Cube();

		void Draw() const;
	};
}
