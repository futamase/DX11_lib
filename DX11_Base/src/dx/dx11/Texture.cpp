#include "Texture.h"

namespace tama
{
	TextureRegion Texture::Region(const Rect rect) const
	{
		return TextureRegion{ *this, rect };
	}
}
