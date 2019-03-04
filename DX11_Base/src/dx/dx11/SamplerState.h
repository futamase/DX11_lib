#pragma once

#include <stdint.h>
#include <unordered_map>
#include "../../base/Math.h"

namespace tama
{
	enum class TextureFilter : uint8_t
	{
		MinMagMipPoint				= 0,
		MinMagPointMipLinear		= 0x1,
		MinPointMagLinearMipPoint	= 0x4,
		MinPointMagMipLinear		= 0x5,
		MinLinearMagMipPoint		= 0x10,
		MinLinearMagPointMipLinear	= 0x11,
		MinMagLinearMipPoint		= 0x14,
		MinMagMipLinear				= 0x15,
		Anisotropic					= 0x55,   
	};

	enum class TextureAddressMode : uint8_t
	{
        Wrap		= 1,
        Mirror		= 2,
        Clamp		= 3,
		Border		= 4,
        MirrorOnce	= 5
	};

	struct SamplerState
	{
		constexpr SamplerState(
			TextureAddressMode _addressU = TextureAddressMode::Clamp,
			TextureAddressMode _addressV = TextureAddressMode::Clamp,
			TextureAddressMode _addressW = TextureAddressMode::Clamp,
			TextureFilter _filter = TextureFilter::MinMagMipLinear,
			uint8_t _maxAnisotropy = 16,
			const Color& _borderColor = Color(0,0))
			: filter(_filter)
			, addressU(_addressU)
			, addressV(_addressV)
			, addressW(_addressW)
			, maxAnisotropy(_maxAnisotropy)
			, borderColor(_borderColor) 
		{}

		constexpr SamplerState(
			TextureAddressMode _address,
			TextureFilter _filter,
			uint8_t _maxAnisotropy = 16,
			const tama::Color& _borderColor = tama::Color(0, 0))
			: filter(_filter)
			, addressU(_address)
			, addressV(_address)
			, addressW(_address)
			, maxAnisotropy(_maxAnisotropy)
			, borderColor(_borderColor) 
		{}

		SamplerState(const SamplerState&) = default;
		SamplerState& operator= (const SamplerState&) = default;

		using DataType = uint64_t;

#pragma warning(disable:4201)
		union
		{
			struct
			{
				TextureAddressMode addressU : 4;
				TextureAddressMode addressV : 4;
				TextureAddressMode addressW : 8;
				TextureFilter filter : 8;
				uint8_t maxAnisotropy : 8;
				Color borderColor;
			};

			DataType _data;
		};
#pragma warning(default:4201)

		static constexpr uint32_t MaxSamplerCount = 8;

		bool operator == (const SamplerState& s) const
		{
			return _data == s._data;
		}

		bool operator != (const SamplerState& s) const
		{
			return _data != s._data;
		}

		enum class Predefined
		{
			WrapPoint,
			WrapLinear,
			WrapAniso,
			MirrorPoint,
			MirrorLinear,
			MirrorAniso,
			ClampPoint,
			ClampLinear,
			ClampAniso,
			Default2D = WrapLinear,
			Default3D = WrapAniso,
		};

		SamplerState(Predefined predefined)
		{
			static const SamplerState predefinedStates[9] =
			{
				{ TextureAddressMode::Wrap,		TextureFilter::MinMagMipPoint	},	// WrapPoint
				{ TextureAddressMode::Wrap,		TextureFilter::MinMagMipLinear	},	// WrapLinear
				{ TextureAddressMode::Wrap,		TextureFilter::Anisotropic		},	// WrapAniso
				{ TextureAddressMode::Mirror,	TextureFilter::MinMagMipPoint	},	// MirrorPoint
				{ TextureAddressMode::Mirror,	TextureFilter::MinMagMipLinear	},	// MirrorLinear
				{ TextureAddressMode::Mirror,	TextureFilter::Anisotropic		},	// MirrorAniso
				{ TextureAddressMode::Clamp,	TextureFilter::MinMagMipPoint	},	// ClampPoint
				{ TextureAddressMode::Clamp,	TextureFilter::MinMagMipLinear	},	// ClampLinear
				{ TextureAddressMode::Clamp,	TextureFilter::Anisotropic		},	// ClampAniso
			};

			*this = predefinedStates[static_cast<size_t>(predefined)];
		}

		static constexpr Predefined WrapPoint = Predefined::WrapPoint;
		static constexpr Predefined WrapLinear = Predefined::WrapLinear;
		static constexpr Predefined WrapAniso = Predefined::WrapAniso;
		static constexpr Predefined MirrorPoint = Predefined::MirrorPoint;
		static constexpr Predefined MirrorLinear = Predefined::MirrorLinear;
		static constexpr Predefined MirrorAniso = Predefined::MirrorAniso;
		static constexpr Predefined ClampPoint = Predefined::ClampPoint;
		static constexpr Predefined ClampLinear = Predefined::ClampLinear;
		static constexpr Predefined ClampAniso = Predefined::ClampAniso;
		static constexpr Predefined Default2D = Predefined::Default2D;
		static constexpr Predefined Default3D = Predefined::Default3D;
	};

	static_assert(sizeof(SamplerState) == sizeof(SamplerState::DataType), "");
}

template<>
struct std::hash<tama::SamplerState>
{
	size_t operator()(const tama::SamplerState& keyVal) const
	{
		return std::hash<tama::SamplerState::DataType>()(keyVal._data);
	}
};
