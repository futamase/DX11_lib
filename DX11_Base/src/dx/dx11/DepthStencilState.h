#pragma once

#include <stdint.h>
#include <unordered_map>

namespace tama
{
	enum class DepthFunc : uint16_t
	{
		Never			= 1,
		Less			= 2,
		Equal			= 3,
		LessEqual		= 4,
		Greater			= 5,
		NotEqual		= 6,
		GreaterEqual	= 7,
		Always			= 8
	};

	struct DepthState
	{
		constexpr DepthState(
			bool _enable = true,
			bool _writeEnable = true,
			DepthFunc _func = DepthFunc::GreaterEqual
			)
			: enable(_enable)
			, writeEnable(_writeEnable)
			, func(_func)
		{}

		using DataType = uint32_t;

#pragma warning(disable:4201)
		union
		{
			struct
			{
				bool enable;
				bool writeEnable;
				DepthFunc func;
			};

			DataType _data;
		};
#pragma warning(default:4201)

		bool operator == (const DepthState& d) const
		{
			return _data == d._data;
		}

		bool operator != (const DepthState& d) const
		{
			return _data != d._data;
		}

		enum class Predefined
		{
			None,
			TestOnly,
			WriteOnly,
			TestWrite,
			Default2D = None,
			Default3D = TestWrite,
		};

		DepthState(Predefined predefined)
		{
			static constexpr DepthState predefinedStates[4] =
			{
				{ false,	false,	DepthFunc::Always		},	// None
				{ true,		false,	DepthFunc::GreaterEqual },	// TestOnly
				{ true,		true,	DepthFunc::Always		},	// WriteOnly
				{ true,		true,	DepthFunc::GreaterEqual },	// TestWrite
			};

			*this = predefinedStates[static_cast<size_t>(predefined)];
		}

		static constexpr Predefined None		= Predefined::None;
		static constexpr Predefined TestOnly	= Predefined::TestOnly;
		static constexpr Predefined WriteOnly	= Predefined::WriteOnly;
		static constexpr Predefined TestWrite	= Predefined::TestWrite;
		static constexpr Predefined Default2D	= Predefined::Default2D;
		static constexpr Predefined Default3D	= Predefined::Default3D;
	};

	enum class StencilOp : uint8_t
	{
		Keep		= 1,
		Zero		= 2,
		Replace		= 3,
		IncrSat		= 4,
		DecrSat		= 5,
		Invert		= 6,
		Incr		= 7,
		Decr		= 8
	};

	enum class StencilFunc : uint8_t
	{
        Never			= 1,
        Less			= 2,
        Equal			= 3,
        LessEqual		= 4,
        Greater			= 5,
        NotEqual		= 6,
        GreaterEqual	= 7,
        Always			= 8
	};

	struct StencilState
	{
		constexpr StencilState(
			bool _enable = false,
			bool _stencilWriteOnly = false,
			StencilFunc _func = StencilFunc::Always,
			StencilOp _passOp = StencilOp::Keep,
			StencilOp _failOp = StencilOp::Keep,
			StencilFunc _backFunc = StencilFunc::Always,
			StencilOp _backPassOp = StencilOp::Keep,
			StencilOp _backFailOp = StencilOp::Keep)
			: enable(_enable)
			, stencilWriteOnly(_stencilWriteOnly)
			, func(_func)
			, passOp(_passOp)
			, failOp(_failOp)
			, backFunc(_backFunc)
			, backPassOp(_backPassOp)
			, backFailOp(_backFailOp) 
		{}

		constexpr StencilState(
			StencilFunc _func,
			StencilOp _passOp,
			StencilOp _failOp = StencilOp::Keep
			)
			: enable(true)
			, stencilWriteOnly(false)
			, func(_func)
			, passOp(_passOp)
			, failOp(_failOp)
			, backFunc(_func)
			, backPassOp(_passOp)
			, backFailOp(_failOp) 
		{}

		using DataType = uint32_t;

#pragma warning(disable:4201)
		union
		{
			struct
			{
				bool enable : 4;
				bool stencilWriteOnly : 4;
				StencilFunc func : 4;
				StencilOp passOp : 4;
				StencilOp failOp : 4;
				StencilFunc backFunc : 4;
				StencilOp backPassOp : 4;
				StencilOp backFailOp : 4;
			};
			
			DataType _data;
		};
#pragma warning(default:4201)

		bool operator == (const StencilState& s) const
		{
			return _data == s._data;
		}

		bool operator != (const StencilState& s) const
		{
			return _data != s._data;
		}

		enum class Predefined
		{
			None,
			Increment,
			Decrement,
			Replace,
			Default = None,
		};

		StencilState(Predefined predefined)
		{
			static const StencilState predefinedStates[4] =
			{
				{ false,	false																														},	// None
				{ true,		true, StencilFunc::Always, StencilOp::Incr,		StencilOp::Keep, StencilFunc::Always, StencilOp::Incr,		StencilOp::Keep	},	// Increment
				{ true,		true, StencilFunc::Always, StencilOp::Decr,		StencilOp::Keep, StencilFunc::Always, StencilOp::Decr,		StencilOp::Keep	},	// Decrement
				{ true,		true, StencilFunc::Always, StencilOp::Replace,	StencilOp::Keep, StencilFunc::Always, StencilOp::Replace,	StencilOp::Keep	},	// Replace
			};

			*this = predefinedStates[static_cast<size_t>(predefined)];
		}

		static constexpr Predefined None		= Predefined::None;
		static constexpr Predefined Increment	= Predefined::Increment;
		static constexpr Predefined Decrement	= Predefined::Decrement;
		static constexpr Predefined Replace		= Predefined::Replace;
		static constexpr Predefined Default		= Predefined::Default;
	};

	struct DepthStencilState
	{
		DepthStencilState(
			const DepthState& _depth = DepthState::Default2D,
			const StencilState& _stencil = StencilState::Default)
			: depth(_depth)
			, stencil(_stencil) 
		{}

		using DataType = unsigned __int64;

#pragma warning(disable:4201)
		union
		{
			struct
			{
				DepthState depth;
				StencilState stencil;
			};

			DataType _data;
		};
#pragma warning(default:4201)

		bool operator == (const DepthStencilState& d) const
		{
			return _data == d._data;
		}

		bool operator != (const DepthStencilState& d) const
		{
			return _data != d._data;
		}

		enum class Predefined
		{
			Default2D,
			Default3D,
		};

		DepthStencilState(Predefined predefined)
		{
			static const DepthStencilState predefinedStates[2] =
			{
				{ DepthState::Default2D, StencilState::Default },
				{ DepthState::Default3D, StencilState::Default },
			};

			*this = predefinedStates[static_cast<size_t>(predefined)];
		}

		static constexpr Predefined Default2D = Predefined::Default2D;
		static constexpr Predefined Default3D = Predefined::Default3D;
	};

	static_assert(sizeof(DepthState) == sizeof(DepthState::DataType), "");
	static_assert(sizeof(StencilState) == sizeof(StencilState::DataType), "");
	static_assert(sizeof(DepthStencilState) == sizeof(DepthStencilState::DataType), "");
}

template<>
struct std::hash<tama::DepthState>
{
	size_t operator()(const tama::DepthState& keyVal) const
	{
		return std::hash<tama::DepthState::DataType>()(keyVal._data);
	}
};

template<>
struct std::hash<tama::StencilState>
{
	size_t operator()(const tama::StencilState& keyVal) const
	{
		return std::hash<tama::StencilState::DataType>()(keyVal._data);
	}
};

template<>
struct std::hash<tama::DepthStencilState>
{
	size_t operator()(const tama::DepthStencilState& keyVal) const
	{
		return std::hash<tama::DepthStencilState::DataType>()(keyVal._data);
	}

};
