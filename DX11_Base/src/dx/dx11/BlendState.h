#pragma once

#include <stdint.h>
#include <unordered_map>

namespace tama
{
	enum class Blend : uint8_t
	{
		Zero = 1,
		One = 2,
		SrcColor = 3,
		InvSrcColor = 4,
		SrcAlpha = 5,
		InvSrcAlpha = 6,
		DestAlpha = 7,
		InvDestAlpha = 8,
		DestColor = 9,
		InvDestColor = 10,
		SrcAlphaSat = 11,
		BlendFactor = 14,
		InvBlendFactor = 15,
		Src1Color = 16,
		InvSrc1Color = 17,
		Src1Alpha = 18,
		InvSrc1Alpha = 19
	};

	enum class BlendOp : uint8_t
	{
		Add = 1,
		Subtract = 2,
		RevSubtract = 3,
		Min = 4,
		Max = 5
	};

	struct BlendState
	{
		constexpr BlendState()
			: enable(false)
			, src(Blend::SrcAlpha)
			, dest(Blend::InvSrcAlpha)
			, op(BlendOp::Add)
			, alphaToCoverageEnable(false)
			, srcAlpha(Blend::One)
			, destAlpha(Blend::Zero)
			, opAlpha(BlendOp::Add) 
		{}

		constexpr BlendState(
			bool _enable,
			Blend _src = Blend::SrcAlpha,
			Blend _dest = Blend::InvSrcAlpha,
			BlendOp _op = BlendOp::Add,
			Blend _srcAlpha = Blend::Zero,
			Blend _destAlpha = Blend::One,
			BlendOp _opAlpha = BlendOp::Add,
			bool _alphaToCoverageEnable = false)
			: enable(_enable)
			, src(_src)
			, dest(_dest)
			, op(_op)
			, alphaToCoverageEnable(_alphaToCoverageEnable)
			, srcAlpha(_srcAlpha)
			, destAlpha(_destAlpha)
			, opAlpha(_opAlpha) {}

		using DataType = uint32_t;

#pragma warning(disable:4201)
		union
		{
			struct
			{
				bool enable : 3;
				Blend src : 5;
				Blend dest : 5;
				BlendOp op : 3;
				bool alphaToCoverageEnable : 3;
				Blend srcAlpha : 5;
				Blend destAlpha : 5;
				BlendOp opAlpha : 3;
			};

			DataType _data;
		};
#pragma warning(default:4201)

		constexpr bool operator == (const BlendState& b) const
		{
			return _data == b._data;
		}

		constexpr bool operator != (const BlendState& b) const
		{
			return _data != b._data;
		}

		enum class Predefined
		{
			NonPremultiplied,
			Opaque,
			Additive,
			Alphablend,
			Subtractive,
			Multiplicative,
			Multiplicative2X,
			Default = NonPremultiplied,
		};

		BlendState(Predefined predefined)
		{
			static constexpr BlendState predefinedStates[7] =
			{
				{ true },																	// NonPremultiplied
				{ false },																	// Opaque
				{ true, Blend::SrcAlpha,	Blend::One,			BlendOp::Add },	// Additive
				{ true, Blend::SrcAlpha,	Blend::InvSrcAlpha, BlendOp::Add, Blend::InvDestAlpha, Blend::One, BlendOp::Add, false }, // AlphaBlend
				{ true, Blend::SrcAlpha,	Blend::One,			BlendOp::RevSubtract },	// Subtractive
				{ true, Blend::Zero,		Blend::SrcColor,	BlendOp::Add },	// Multiplicative
				{ true, Blend::DestColor,	Blend::SrcColor,	BlendOp::Add },	// Multiplicative2X
			};

			*this = predefinedStates[static_cast<size_t>(predefined)];
		}
	};

	static_assert(sizeof(BlendState) == sizeof(BlendState::DataType), "");
}
 
template<>
struct std::hash<tama::BlendState>
{
	size_t operator()(const tama::BlendState& keyVal) const
	{
		return std::hash<tama::BlendState::DataType>()(keyVal._data);
	}
};

