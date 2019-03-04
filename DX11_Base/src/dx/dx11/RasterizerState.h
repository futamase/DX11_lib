#pragma once

#include <stdint.h>
#include <unordered_map>

namespace tama
{
	enum class FillMode : uint8_t
	{
		Wireframe = 2,
		Solid = 3,
	};

	enum class CullMode : uint8_t
	{
		None = 1,
		Front = 2,
		Back = 3,
	};

	struct RasterizerState
	{
		constexpr RasterizerState(
			FillMode _fillMode = FillMode::Solid,
			CullMode _cullMode = CullMode::Back,
			bool _frontCounterClockwise = false,
			bool _scissorEnable = false,
			int32_t _depthBias = 0)
			: fillMode(_fillMode)
			, cullMode(_cullMode)
			, frontCounterClockwise(_frontCounterClockwise)
			, scissorEnable(_scissorEnable)
			, depthBias(_depthBias)
		{}

		using DataType = uint64_t;

#pragma warning(disable:4201)
		union
		{
			struct
			{
				/// <summary>
				/// 面の塗りつぶし方法
				/// </summary>
				FillMode fillMode : 2;

				/// <summary>
				/// カリングの基準
				/// </summary>
				CullMode cullMode : 2;

				/// <summary>
				/// 頂点が反時計回りの面を表面とするかどうかを示します。 
				/// </summary>
				bool frontCounterClockwise : 1;

				/// <summary>
				/// 頂点が反時計回りの面を表面とするかどうかを示します。 
				/// </summary>
				bool scissorEnable : 1;

				/// <summary>
				/// デプスのバイアス
				/// </summary>
				int32_t depthBias;
			};

			DataType _data;
		};
#pragma warning(default:4201)

		bool operator == (const RasterizerState& r) const
		{
			return _data == r._data;
		}

		bool operator != (const RasterizerState& r) const
		{
			return _data != r._data;
		}

		enum class Predefined
		{
			SolidCullBack,
			SolidCullFront,
			SolidCullNone,
			WireframeCullBack,
			WireframeCullFront,
			WireframeCullNone,
			Default2D = SolidCullNone,
			Default3D = SolidCullBack,
		};

		RasterizerState(Predefined predefined)
		{
			static constexpr RasterizerState predefinedStates[6] =
			{
				{ FillMode::Solid,		CullMode::Back },	// SolidCullBack
				{ FillMode::Solid,		CullMode::Front },	// SolidCullFront
				{ FillMode::Solid,		CullMode::None },	// SolidCullNone
				{ FillMode::Wireframe,	CullMode::Back },	// WireframeCullBack
				{ FillMode::Wireframe,	CullMode::Front },	// WireframeCullFront
				{ FillMode::Wireframe,	CullMode::None },	// WireframeCullNone
			};

			*this = predefinedStates[static_cast<size_t>(predefined)];
		}
	};

	static_assert(sizeof(RasterizerState) == sizeof(RasterizerState::DataType), "");
}

template<>
struct std::hash<tama::RasterizerState>
{
	size_t operator()(const tama::RasterizerState& keyVal) const
	{
		return std::hash<tama::RasterizerState::DataType>()(keyVal._data);
	}
};

