#include "SpriteBatch.h"
//#include "CommonStates.h"
//#include "VertexTypes.h"
//#include "SharedResourcePool.h"
#include "AlignedNew.h"
#include "DXResource.h"
#include "ConstantBuffer.h"
#include <wrl.h>

#include <vector>
#include <algorithm>

#undef max
#undef min

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace
{
	// Include the precompiled shader code.
#include <PreCompiledShader/SpriteEffect_SpriteVertexShader.inc>
#include <PreCompiledShader/SpriteEffect_SpritePixelShader.inc>

// Helper looks up the D3D device corresponding to a context interface.
	inline ComPtr<ID3D11Device> GetDevice(_In_ ID3D11DeviceContext* deviceContext)
	{
		ComPtr<ID3D11Device> device;

		deviceContext->GetDevice(&device);

		return device;
	}


	// Helper converts a RECT to XMVECTOR.
	inline XMVECTOR LoadRect(_In_ RECT const* rect)
	{
		XMVECTOR v = XMLoadInt4(reinterpret_cast<uint32_t const*>(rect));

		v = XMConvertVectorIntToFloat(v, 0);

		// Convert right/bottom to width/height.
		v -= XMVectorPermute<0, 1, 4, 5>(XMVectorZero(), v);

		return v;
	}

	struct VertexPositionColorUV
	{
		VertexPositionColorUV() = default;

		VertexPositionColorUV(XMFLOAT3 const& position, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
			: position(position),
			color(color),
			textureCoordinate(textureCoordinate)
		{ }

		VertexPositionColorUV(FXMVECTOR position, FXMVECTOR color, FXMVECTOR textureCoordinate)
		{
			XMStoreFloat3(&this->position, position);
			XMStoreFloat4(&this->color, color);
			XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
		}

		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT2 textureCoordinate;

		static const int InputElementCount = 3;
		static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
	};

	const D3D11_INPUT_ELEMENT_DESC VertexPositionColorUV::InputElements[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static_assert(sizeof(VertexPositionColorUV) == 36, "Vertex struct/layout mismatch");
}

namespace DX11
{
#if 1
	__declspec(align(16))
		class SpriteBatch::Impl : public AlignedNew<SpriteBatch::Impl>
	{
	public:
		Impl(_In_ ID3D11DeviceContext* deviceContext);

		void XM_CALLCONV Begin(SpriteSortMode sortMode,
			_In_opt_ ID3D11BlendState* blendState,
			_In_opt_ ID3D11SamplerState* samplerState,
			_In_opt_ ID3D11DepthStencilState* depthStencilState,
			_In_opt_ ID3D11RasterizerState* rasterizerState,
			_In_opt_ std::function<void()> setCustomShaders,
			FXMMATRIX transformMatrix);
		void End();

		void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture,
			FXMVECTOR destination,
			_In_opt_ RECT const* sourceRectangle,
			FXMVECTOR color,
			FXMVECTOR originRotationDepth,
			int flags);

		void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture,
			FXMVECTOR destination,
			_In_opt_ RECT const* sourceRectangle,
			FXMVECTOR color,
			FXMVECTOR originRotationDepth,
			FXMVECTOR scaleRotateOrigin,
			int flags);

		__declspec(align(16))
			struct SpriteInfo : public AlignedNew<SpriteInfo>
		{
			XMFLOAT4A source;
			XMFLOAT4A destination;
			XMFLOAT4A color;
			XMFLOAT4A originRotationDepth;
			XMFLOAT4A scaleRotateOrigin;
			ID3D11ShaderResourceView* texture;
			int flags;

			// Combine values from the public SpriteEffects enum with these internal-only flags.
			static const int SourceInTexels = 4;
			static const int DestSizeInPixels = 8;

			static_assert((static_cast<int>(SpriteEffects::FlipBoth) & (SourceInTexels | DestSizeInPixels)) == 0, "Flag bits must not overlap");
		};

		DXGI_MODE_ROTATION m_Rotation;

		bool m_SetViewport;

		D3D11_VIEWPORT m_Viewport;

	private:
		void GrowSpriteQueue();
		void PrepareForRendering();
		void FlushBatch();
		void SortSprites();
		void GrowSortedSprites();

		void RenderBatch(_In_ ID3D11ShaderResourceView* texture, _In_reads_(count) SpriteInfo const* const* sprites, size_t count);

		static void XM_CALLCONV RenderSprite(_In_ SpriteInfo const* sprite,
			_Out_writes_(VerticesPerSprite) VertexPositionColorUV* vertices,
			FXMVECTOR textureSize,
			FXMVECTOR inverseTextureSize);

		static XMVECTOR GetTextureSize(_In_ ID3D11ShaderResourceView* texture);
		XMMATRIX GetViewportTransform(_In_ ID3D11DeviceContext* deviceContext, DXGI_MODE_ROTATION rotation);

		// Constants.
		static const size_t MaxBatchSize = 2048;
		static const size_t MinBatchSize = 128;
		static const size_t InitialQueueSize = 64;
		static const size_t VerticesPerSprite = 4;
		static const size_t IndicesPerSprite = 6;


		// Queue of sprites waiting to be drawn.
		std::unique_ptr<SpriteInfo[]> m_SpriteQueue;

		size_t m_SpriteQueueCount;
		size_t m_SpriteQueueArraySize;


		// To avoid needlessly copying around bulky SpriteInfo structures, we leave that
		// actual data alone and just sort this array of pointers instead. But we want contiguous
		// memory for cache efficiency, so these pointers are just shortcuts into the single
		// mSpriteQueue array, and we take care to keep them in order when sorting is disabled.
		std::vector<SpriteInfo const*> m_SortedSprites;


		// If each SpriteInfo instance held a refcount on its texture, could end up with
		// many redundant AddRef/Release calls on the same object, so instead we use
		// this separate list to hold just a single refcount each time we change texture.
		std::vector<ComPtr<ID3D11ShaderResourceView>> m_SpriteTextureReferences;


		// Mode settings from the last Begin call.
		bool m_InBeginEndPair;

		SpriteSortMode m_SortMode;
		ComPtr<ID3D11BlendState> m_BlendState;
		ComPtr<ID3D11SamplerState> m_SamplerState;
		ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
		ComPtr<ID3D11RasterizerState> m_RasterizerState;
		std::function<void()> m_SetCustomShaders;
		XMMATRIX m_TransformMatrix;


		// Only one of these helpers is allocated per D3D device, even if there are multiple SpriteBatch instances.
		struct DeviceResources
		{
			DeviceResources(_In_ ID3D11Device* device);

			ComPtr<ID3D11VertexShader> vertexShader;
			ComPtr<ID3D11PixelShader> pixelShader;
			ComPtr<ID3D11InputLayout> inputLayout;
			ComPtr<ID3D11Buffer> indexBuffer;

//			DirectX::CommonStates stateObjects;

		private:
			void CreateShaders(_In_ ID3D11Device* device);
			void CreateIndexBuffer(_In_ ID3D11Device* device);

			static std::vector<short> CreateIndexValues();
		};


		// Only one of these helpers is allocated per D3D device context, even if there are multiple SpriteBatch instances.
		struct ContextResources
		{
			ContextResources(_In_ ID3D11DeviceContext* deviceContext);

			ComPtr<ID3D11DeviceContext> deviceContext;

			ComPtr<ID3D11Buffer> vertexBuffer;

			tama::ConstantBuffer<XMFLOAT4X4> constantBuffer;

			size_t vertexBufferPosition;

			bool inImmediateMode;

		private:
			void CreateVertexBuffer();
		};

		// Per-device and per-context data.
		std::shared_ptr<DeviceResources> m_DeviceResources;
		std::shared_ptr<ContextResources> m_ContextResources;

	};

	// Constants.
	const XMMATRIX SpriteBatch::MatrixIdentity = XMMatrixIdentity();
	const XMFLOAT2 SpriteBatch::Float2Zero(0, 0);

	// Per-device constructor.
	SpriteBatch::Impl::DeviceResources::DeviceResources(_In_ ID3D11Device* device)
	{
		CreateShaders(device);
		CreateIndexBuffer(device);
	}


	// Creates the SpriteBatch shaders and input layout.
	void SpriteBatch::Impl::DeviceResources::CreateShaders(_In_ ID3D11Device* device)
	{

		if (FAILED(device->CreateVertexShader(SpriteEffect_SpriteVertexShader,
			sizeof(SpriteEffect_SpriteVertexShader),
			nullptr,
			&vertexShader)))
			throw std::runtime_error("");

		if (FAILED(device->CreatePixelShader(SpriteEffect_SpritePixelShader,
			sizeof(SpriteEffect_SpritePixelShader),
			nullptr,
			&pixelShader)))
			throw std::runtime_error("");

		if (FAILED(device->CreateInputLayout(VertexPositionColorUV::InputElements,
			VertexPositionColorUV::InputElementCount,
			SpriteEffect_SpriteVertexShader,
			sizeof(SpriteEffect_SpriteVertexShader),
			&inputLayout)))
			throw std::runtime_error("");
	}


	// Creates the SpriteBatch index buffer.
	void SpriteBatch::Impl::DeviceResources::CreateIndexBuffer(_In_ ID3D11Device* device)
	{
		D3D11_BUFFER_DESC indexBufferDesc = {};

		static_assert((MaxBatchSize * VerticesPerSprite) < USHRT_MAX, "MaxBatchSize too large for 16-bit indices");

		indexBufferDesc.ByteWidth = sizeof(short) * MaxBatchSize * IndicesPerSprite;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		auto indexValues = CreateIndexValues();

		D3D11_SUBRESOURCE_DATA indexDataDesc = {};

		indexDataDesc.pSysMem = indexValues.data();

		if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexDataDesc, &indexBuffer)))
			throw std::runtime_error("");
	}


	// Helper for populating the SpriteBatch index buffer.
	std::vector<short> SpriteBatch::Impl::DeviceResources::CreateIndexValues()
	{
		std::vector<short> indices;

		indices.reserve(MaxBatchSize * IndicesPerSprite);

		for (short i = 0; i < MaxBatchSize * VerticesPerSprite; i += VerticesPerSprite)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 2);

			indices.push_back(i + 1);
			indices.push_back(i + 3);
			indices.push_back(i + 2);
		}

		return indices;
	}


	// Per-context constructor.
	SpriteBatch::Impl::ContextResources::ContextResources(_In_ ID3D11DeviceContext* context) :
		vertexBufferPosition(0),
		inImmediateMode(false)
	{
		deviceContext = context;
		CreateVertexBuffer();
	}


	// Creates the SpriteBatch vertex buffer.
	void SpriteBatch::Impl::ContextResources::CreateVertexBuffer()
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};

		vertexBufferDesc.ByteWidth = sizeof(VertexPositionColorUV) * MaxBatchSize * VerticesPerSprite;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(GetDevice(deviceContext.Get())->CreateBuffer(&vertexBufferDesc, nullptr, &vertexBuffer)))
			throw std::runtime_error("");
	}

	// Per-SpriteBatch constructor.
	SpriteBatch::Impl::Impl(_In_ ID3D11DeviceContext* deviceContext) :
		m_Rotation(DXGI_MODE_ROTATION_IDENTITY),
		m_SetViewport(false),
		m_SpriteQueueCount(0),
		m_SpriteQueueArraySize(0),
		m_InBeginEndPair(false),
		m_SortMode(SpriteSortMode::Deferred),
		m_TransformMatrix(MatrixIdentity),
		m_DeviceResources(std::make_shared<DeviceResources>(GetDevice(deviceContext).Get())),
		m_ContextResources(std::make_shared<ContextResources>(deviceContext))
	{}

	// Begins a batch of sprite drawing operations.
	void XM_CALLCONV SpriteBatch::Impl::Begin(SpriteSortMode sortMode,
		ID3D11BlendState* blendState,
		ID3D11SamplerState* samplerState,
		ID3D11DepthStencilState* depthStencilState,
		ID3D11RasterizerState* rasterizerState,
		std::function<void()> setCustomShaders,
		FXMMATRIX transformMatrix)
	{
		if (m_InBeginEndPair)
			throw std::exception("Cannot nest Begin calls on a single SpriteBatch");

		m_SortMode = sortMode;
		m_BlendState = blendState;
		m_SamplerState = samplerState;
		m_DepthStencilState = depthStencilState;
		m_RasterizerState = rasterizerState;
		m_SetCustomShaders = setCustomShaders;
		m_TransformMatrix = transformMatrix;

		if (sortMode == SpriteSortMode::Immediate)
		{
			// If we are in immediate mode, set device state ready for drawing.
			if (m_ContextResources->inImmediateMode)
				throw std::exception("Only one SpriteBatch at a time can use SpriteSortMode_Immediate");

			PrepareForRendering();

			m_ContextResources->inImmediateMode = true;
		}

		m_InBeginEndPair = true;
	}


	// Ends a batch of sprite drawing operations.
	void SpriteBatch::Impl::End()
	{
		if (!m_InBeginEndPair)
			throw std::exception("Begin must be called before End");

		if (m_SortMode == SpriteSortMode::Immediate)
		{
			// If we are in immediate mode, sprites have already been drawn.
			m_ContextResources->inImmediateMode = false;
		}
		else
		{
			// Draw the queued sprites now.
			if (m_ContextResources->inImmediateMode)
				throw std::exception("Cannot end one SpriteBatch while another is using SpriteSortMode_Immediate");

			PrepareForRendering();
			FlushBatch();
		}

		// Break circular reference chains, in case the state lambda closed
		// over an object that holds a reference to this SpriteBatch.
		m_SetCustomShaders = nullptr;

		m_InBeginEndPair = false;
	}


	// Adds a single sprite to the queue.
	_Use_decl_annotations_
		void XM_CALLCONV SpriteBatch::Impl::Draw(ID3D11ShaderResourceView* texture,
			FXMVECTOR destination,
			RECT const* sourceRectangle,
			FXMVECTOR color,
			FXMVECTOR originRotationDepth,
			int flags)
	{
		if (!texture)
			throw std::exception("Texture cannot be null");

		if (!m_InBeginEndPair)
			throw std::exception("Begin must be called before Draw");

		// Get a pointer to the output sprite.
		if (m_SpriteQueueCount >= m_SpriteQueueArraySize)
		{
			GrowSpriteQueue();
		}

		SpriteInfo* sprite = &m_SpriteQueue[m_SpriteQueueCount];

		XMVECTOR dest = destination;

		if (sourceRectangle)
		{
			// User specified an explicit source region.
			XMVECTOR source = LoadRect(sourceRectangle);

			XMStoreFloat4A(&sprite->source, source);

			// If the destination size is relative to the source region, convert it to pixels.
			if (!(flags & SpriteInfo::DestSizeInPixels))
			{
				dest = XMVectorPermute<0, 1, 6, 7>(dest, dest * source); // dest.zw *= source.zw
			}

			flags |= SpriteInfo::SourceInTexels | SpriteInfo::DestSizeInPixels;
		}
		else
		{
			// No explicit source region, so use the entire texture.
			static const XMVECTORF32 wholeTexture = { 0, 0, 1, 1 };

			XMStoreFloat4A(&sprite->source, wholeTexture);
		}

		// Store sprite parameters.
		XMStoreFloat4A(&sprite->destination, dest);
		XMStoreFloat4A(&sprite->color, color);
		XMStoreFloat4A(&sprite->originRotationDepth, originRotationDepth);

		sprite->texture = texture;
		sprite->flags = flags;

		if (m_SortMode == SpriteSortMode::Immediate)
		{
			// If we are in immediate mode, draw this sprite straight away.
			RenderBatch(texture, &sprite, 1);
		}
		else
		{
			// Queue this sprite for later sorting and batched rendering.
			m_SpriteQueueCount++;

			// Make sure we hold a refcount on this texture until the sprite has been drawn. Only checking the
			// back of the vector means we will add duplicate references if the caller switches back and forth
			// between multiple repeated textures, but calling AddRef more times than strictly necessary hurts
			// nothing, and is faster than scanning the whole list or using a map to detect all duplicates.
			if (m_SpriteTextureReferences.empty() || texture != m_SpriteTextureReferences.back().Get())
			{
				m_SpriteTextureReferences.emplace_back(texture);
			}
		}
	}

	// Adds a single sprite to the queue.
	_Use_decl_annotations_
		void XM_CALLCONV SpriteBatch::Impl::Draw(ID3D11ShaderResourceView* texture,
			FXMVECTOR destination,
			RECT const* sourceRectangle,
			FXMVECTOR color,
			FXMVECTOR originRotationDepth,
			FXMVECTOR scaleRotateOrigin,
			int flags)
	{
		if (!texture)
			throw std::exception("Texture cannot be null");

		if (!m_InBeginEndPair)
			throw std::exception("Begin must be called before Draw");

		// Get a pointer to the output sprite.
		if (m_SpriteQueueCount >= m_SpriteQueueArraySize)
		{
			GrowSpriteQueue();
		}

		SpriteInfo* sprite = &m_SpriteQueue[m_SpriteQueueCount];

		XMVECTOR dest = destination;

		if (sourceRectangle)
		{
			// User specified an explicit source region.
			XMVECTOR source = LoadRect(sourceRectangle);

			XMStoreFloat4A(&sprite->source, source);

			// If the destination size is relative to the source region, convert it to pixels.
			if (!(flags & SpriteInfo::DestSizeInPixels))
			{
				dest = XMVectorPermute<0, 1, 6, 7>(dest, dest * source); // dest.zw *= source.zw
			}

			flags |= SpriteInfo::SourceInTexels | SpriteInfo::DestSizeInPixels;
		}
		else
		{
			// No explicit source region, so use the entire texture.
			static const XMVECTORF32 wholeTexture = { 0, 0, 1, 1 };

			XMStoreFloat4A(&sprite->source, wholeTexture);
		}

		// Store sprite parameters.
		XMStoreFloat4A(&sprite->destination, dest);
		XMStoreFloat4A(&sprite->color, color);
		XMStoreFloat4A(&sprite->originRotationDepth, originRotationDepth);
		XMStoreFloat4A(&sprite->scaleRotateOrigin, scaleRotateOrigin);

		sprite->texture = texture;
		sprite->flags = flags;

		if (m_SortMode == SpriteSortMode::Immediate)
		{
			// If we are in immediate mode, draw this sprite straight away.
			RenderBatch(texture, &sprite, 1);
		}
		else
		{
			// Queue this sprite for later sorting and batched rendering.
			m_SpriteQueueCount++;

			// Make sure we hold a refcount on this texture until the sprite has been drawn. Only checking the
			// back of the vector means we will add duplicate references if the caller switches back and forth
			// between multiple repeated textures, but calling AddRef more times than strictly necessary hurts
			// nothing, and is faster than scanning the whole list or using a map to detect all duplicates.
			if (m_SpriteTextureReferences.empty() || texture != m_SpriteTextureReferences.back().Get())
			{
				m_SpriteTextureReferences.emplace_back(texture);
			}
		}
	}

	// Dynamically expands the array used to store pending sprite information.
	void SpriteBatch::Impl::GrowSpriteQueue()
	{
		// Grow by a factor of 2.
		size_t newSize = std::max(InitialQueueSize, m_SpriteQueueArraySize * 2);

		// Allocate the new array.
		std::unique_ptr<SpriteInfo[]> newArray(new SpriteInfo[newSize]);

		// Copy over any existing sprites.
		for (size_t i = 0; i < m_SpriteQueueCount; i++)
		{
			newArray[i] = m_SpriteQueue[i];
		}

		// Replace the previous array with the new one.
		m_SpriteQueue = std::move(newArray);
		m_SpriteQueueArraySize = newSize;

		// Clear any dangling SpriteInfo pointers left over from previous rendering.
		m_SortedSprites.clear();
	}


	// Sets up D3D device state ready for drawing sprites.
	void SpriteBatch::Impl::PrepareForRendering()
	{
		auto deviceContext = m_ContextResources->deviceContext.Get();

		// Set state objects.
//		auto blendState = m_BlendState ? m_BlendState.Get() : m_DeviceResources->stateObjects.NonPremultiplied();
//		auto depthStencilState = m_DepthStencilState ? m_DepthStencilState.Get() : m_DeviceResources->stateObjects.DepthNone();
//		auto rasterizerState = m_RasterizerState ? m_RasterizerState.Get() : m_DeviceResources->stateObjects.CullCounterClockwise();
//		auto samplerState = m_SamplerState ? m_SamplerState.Get() : m_DeviceResources->stateObjects.LinearClamp();

		//		deviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
		//		deviceContext->OMSetDepthStencilState(depthStencilState, 0);
		//		deviceContext->RSSetState(rasterizerState);
//		deviceContext->PSSetSamplers(0, 1, &samplerState);

		// Set shaders.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetInputLayout(m_DeviceResources->inputLayout.Get());
		deviceContext->VSSetShader(m_DeviceResources->vertexShader.Get(), nullptr, 0);
//		deviceContext->PSSetShader(m_DeviceResources->pixelShader.Get(), nullptr, 0);

		// Set the vertex and index buffer.
		auto vertexBuffer = m_ContextResources->vertexBuffer.Get();
		UINT vertexStride = sizeof(VertexPositionColorUV);
		UINT vertexOffset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);

		deviceContext->IASetIndexBuffer(m_DeviceResources->indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		// Set the transform matrix.
		XMMATRIX transformMatrix = (m_Rotation == DXGI_MODE_ROTATION_UNSPECIFIED) ?
			m_TransformMatrix :
			(m_TransformMatrix * GetViewportTransform(deviceContext, m_Rotation));

		XMStoreFloat4x4(&m_ContextResources->constantBuffer.Get(), transformMatrix);

		m_ContextResources->constantBuffer.Set(0, tama::ShaderStage::VS);

		// If this is a deferred D3D context, reset position so the first Map call will use D3D11_MAP_WRITE_DISCARD.
		if (deviceContext->GetType() == D3D11_DEVICE_CONTEXT_DEFERRED)
		{
			m_ContextResources->vertexBufferPosition = 0;
		}

		// Hook lets the caller replace our settings with their own custom shaders.
		if (m_SetCustomShaders)
		{
			m_SetCustomShaders();
		}
	}


	// Sends queued sprites to the graphics device.
	void SpriteBatch::Impl::FlushBatch()
	{
		if (!m_SpriteQueueCount)
			return;

		SortSprites();

		// Walk through the sorted sprite list, looking for adjacent entries that share a texture.
		ID3D11ShaderResourceView* batchTexture = nullptr;
		size_t batchStart = 0;

		for (size_t pos = 0; pos < m_SpriteQueueCount; pos++)
		{
			ID3D11ShaderResourceView* texture = m_SortedSprites[pos]->texture;

			_Analysis_assume_(texture != nullptr);

			// Flush whenever the texture changes.
			if (texture != batchTexture)
			{
				if (pos > batchStart)
				{
					RenderBatch(batchTexture, &m_SortedSprites[batchStart], pos - batchStart);
				}

				batchTexture = texture;
				batchStart = pos;
			}
		}

		// Flush the final batch.
		RenderBatch(batchTexture, &m_SortedSprites[batchStart], m_SpriteQueueCount - batchStart);

		// Reset the queue.
		m_SpriteQueueCount = 0;
		m_SpriteTextureReferences.clear();

		// When sorting is disabled, we persist m_SortedSprites data from one batch to the next, to avoid
		// uneccessary work in GrowSortedSprites. But we never reuse these when sorting, because re-sorting
		// previously sorted items gives unstable ordering if some sprites have identical sort keys.
		if (m_SortMode != SpriteSortMode::Deferred)
		{
			m_SortedSprites.clear();
		}
	}

	// Sorts the array of queued sprites.
	void SpriteBatch::Impl::SortSprites()
	{
		// Fill the m_SortedSprites vector.
		if (m_SortedSprites.size() < m_SpriteQueueCount)
		{
			GrowSortedSprites();
		}

		switch (m_SortMode)
		{
			case SpriteSortMode::Texture:
				// Sort by texture.
				std::sort(m_SortedSprites.begin(), m_SortedSprites.begin() + m_SpriteQueueCount, [](SpriteInfo const* x, SpriteInfo const* y) -> bool
				{
					return x->texture < y->texture;
				});
				break;

			case SpriteSortMode::BackToFront:
				// Sort back to front.
				std::sort(m_SortedSprites.begin(), m_SortedSprites.begin() + m_SpriteQueueCount, [](SpriteInfo const* x, SpriteInfo const* y) -> bool
				{
					return x->originRotationDepth.w > y->originRotationDepth.w;
				});
				break;

			case SpriteSortMode::FrontToBack:
				// Sort front to back.
				std::sort(m_SortedSprites.begin(), m_SortedSprites.begin() + m_SpriteQueueCount, [](SpriteInfo const* x, SpriteInfo const* y) -> bool
				{
					return x->originRotationDepth.w < y->originRotationDepth.w;
				});
				break;
		}
	}


	// Populates the m_SortedSprites vector with pointers to individual elements of the m_SpriteQueue array.
	void SpriteBatch::Impl::GrowSortedSprites()
	{
		size_t previousSize = m_SortedSprites.size();

		m_SortedSprites.resize(m_SpriteQueueCount);

		for (size_t i = previousSize; i < m_SpriteQueueCount; i++)
		{
			m_SortedSprites[i] = &m_SpriteQueue[i];
		}
	}


	// Submits a batch of sprites to the GPU.
	_Use_decl_annotations_
		void SpriteBatch::Impl::RenderBatch(ID3D11ShaderResourceView* texture, SpriteInfo const* const* sprites, size_t count)
	{
		auto deviceContext = m_ContextResources->deviceContext.Get();

		// Draw using the specified texture.
		deviceContext->PSSetShaderResources(0, 1, &texture);

		XMVECTOR textureSize = GetTextureSize(texture);
		XMVECTOR inverseTextureSize = XMVectorReciprocal(textureSize);

		while (count > 0)
		{
			// How many sprites do we want to draw?
			size_t batchSize = count;

			// How many sprites does the D3D vertex buffer have room for?
			size_t remainingSpace = MaxBatchSize - m_ContextResources->vertexBufferPosition;

			if (batchSize > remainingSpace)
			{
				if (remainingSpace < MinBatchSize)
				{
					// If we are out of room, or about to submit an excessively small batch, wrap back to the start of the vertex buffer.
					m_ContextResources->vertexBufferPosition = 0;

					batchSize = std::min(count, MaxBatchSize);
				}
				else
				{
					// Take however many sprites fit in what's left of the vertex buffer.
					batchSize = remainingSpace;
				}
			}

			// Lock the vertex buffer.
			D3D11_MAP mapType = (m_ContextResources->vertexBufferPosition == 0) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;

			D3D11_MAPPED_SUBRESOURCE mappedBuffer;

			if (FAILED(deviceContext->Map(m_ContextResources->vertexBuffer.Get(), 0, mapType, 0, &mappedBuffer)))
				throw std::exception("");

			auto vertices = static_cast<VertexPositionColorUV*>(mappedBuffer.pData) + m_ContextResources->vertexBufferPosition * VerticesPerSprite;

			// Generate sprite vertex data.
			for (size_t i = 0; i < batchSize; i++)
			{
				assert(i < count);
				_Analysis_assume_(i < count);
				RenderSprite(sprites[i], vertices, textureSize, inverseTextureSize);

				vertices += VerticesPerSprite;
			}

			deviceContext->Unmap(m_ContextResources->vertexBuffer.Get(), 0);

			// Ok lads, the time has come for us draw ourselves some sprites!
			UINT startIndex = (UINT)m_ContextResources->vertexBufferPosition * IndicesPerSprite;
			UINT indexCount = (UINT)batchSize * IndicesPerSprite;

			deviceContext->DrawIndexed(indexCount, startIndex, 0);

			// Advance the buffer position.
			m_ContextResources->vertexBufferPosition += batchSize;

			sprites += batchSize;
			count -= batchSize;
		}
	}


	// Generates vertex data for drawing a single sprite.
	_Use_decl_annotations_
		void XM_CALLCONV SpriteBatch::Impl::RenderSprite(SpriteInfo const* sprite,
			VertexPositionColorUV* vertices,
			FXMVECTOR textureSize,
			FXMVECTOR inverseTextureSize)
	{
		// Load sprite parameters into SIMD registers.
		XMVECTOR source = XMLoadFloat4A(&sprite->source);
		XMVECTOR destination = XMLoadFloat4A(&sprite->destination);
		XMVECTOR color = XMLoadFloat4A(&sprite->color);
		XMVECTOR originRotationDepth = XMLoadFloat4A(&sprite->originRotationDepth);
		XMVECTOR scaleRotateOrigin = XMLoadFloat4A(&sprite->scaleRotateOrigin);

		float rotation = sprite->originRotationDepth.z;
		int flags = sprite->flags;

		// Extract the source and destination sizes into separate vectors.
		XMVECTOR sourceSize = XMVectorSwizzle<2, 3, 2, 3>(source);
		XMVECTOR destinationSize = XMVectorSwizzle<2, 3, 2, 3>(destination);

		// Scale the origin offset by source size, taking care to avoid overflow if the source region is zero.
		XMVECTOR isZeroMask = XMVectorEqual(sourceSize, XMVectorZero());
		XMVECTOR nonZeroSourceSize = XMVectorSelect(sourceSize, g_XMEpsilon, isZeroMask);

		XMVECTOR origin = XMVectorDivide(originRotationDepth, nonZeroSourceSize);

		// Convert the source region from texels to mod-1 texture coordinate format.
		if (flags & SpriteInfo::SourceInTexels)
		{
			source *= inverseTextureSize;
			sourceSize *= inverseTextureSize;
		}
		else
		{
			origin *= inverseTextureSize;
		}

		// If the destination size is relative to the source region, convert it to pixels.
		if (!(flags & SpriteInfo::DestSizeInPixels))
		{
			destinationSize *= textureSize;
		}

		// Compute a 2x2 rotation matrix.
		XMVECTOR rotationMatrix1;
		XMVECTOR rotationMatrix2;

		if (rotation != 0)
		{
			float sin, cos;

			XMScalarSinCos(&sin, &cos, rotation);

			XMVECTOR sinV = XMLoadFloat(&sin);
			XMVECTOR cosV = XMLoadFloat(&cos);

			rotationMatrix1 = XMVectorMergeXY(cosV, sinV);
			rotationMatrix2 = XMVectorMergeXY(-sinV, cosV);
		}
		else
		{
			rotationMatrix1 = g_XMIdentityR0;
			rotationMatrix2 = g_XMIdentityR1;
		}

		// The four corner vertices are computed by transforming these unit-square positions.
		static XMVECTORF32 cornerOffsets[VerticesPerSprite] =
		{
			{ 0, 0 },
			{ 1, 0 },
			{ 0, 1 },
			{ 1, 1 },
		};

		// Tricksy alert! Texture coordinates are computed from the same cornerOffsets
		// table as vertex positions, but if the sprite is mirrored, this table
		// must be indexed in a different order. This is done as follows:
		//
		//    position = cornerOffsets[i]
		//    texcoord = cornerOffsets[i ^ SpriteEffects]

		static_assert(static_cast<int>(SpriteEffects::FlipHorizontally) == 1 &&
			static_cast<int>(SpriteEffects::FlipVertically) == 2, "If you change these enum values, the mirroring implementation must be updated to match");

		int mirrorBits = flags & 3;

		// Generate the four output vertices.
		for (int i = 0; i < VerticesPerSprite; i++)
		{
			// Calculate position.
			XMVECTOR cornerOffset = (cornerOffsets[i] - origin) * destinationSize;

			// Apply 2x2 rotation matrix.
			XMVECTOR position1 = XMVectorMultiplyAdd(XMVectorSplatX(cornerOffset), rotationMatrix1, destination);
			XMVECTOR position2 = XMVectorMultiplyAdd(XMVectorSplatY(cornerOffset), rotationMatrix2, position1);

			// Set z = depth.
			XMVECTOR position = XMVectorPermute<0, 1, 7, 6>(position2, originRotationDepth);

			// Write position as a Float4, even though VertexPositionColor::position is an XMFLOAT3.
			// This is faster, and harmless as we are just clobbering the first element of the
			// following color field, which will immediately be overwritten with its correct value.
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&vertices[i].position), position);

			// Write the color.
			XMStoreFloat4(&vertices[i].color, color);

			// Compute and write the texture coordinate.
			XMVECTOR textureCoordinate = XMVectorMultiplyAdd(cornerOffsets[i ^ mirrorBits], sourceSize, source);

			XMStoreFloat2(&vertices[i].textureCoordinate, textureCoordinate);
		}
	}


	// Helper looks up the size of the specified texture.
	XMVECTOR SpriteBatch::Impl::GetTextureSize(_In_ ID3D11ShaderResourceView* texture)
	{
		// Convert resource view to underlying resource.
		ComPtr<ID3D11Resource> resource;

		texture->GetResource(&resource);

		// Cast to texture.
		ComPtr<ID3D11Texture2D> texture2D;

		if (FAILED(resource.As(&texture2D)))
		{
			throw std::exception("SpriteBatch can only draw Texture2D resources");
		}

		// Query the texture size.
		D3D11_TEXTURE2D_DESC desc;

		texture2D->GetDesc(&desc);

		// Convert to vector format.
		XMVECTOR size = XMVectorMergeXY(XMLoadInt(&desc.Width),
			XMLoadInt(&desc.Height));

		return XMConvertVectorUIntToFloat(size, 0);
	}


	// Generates a viewport transform matrix for rendering sprites using x-right y-down screen pixel coordinates.
	XMMATRIX SpriteBatch::Impl::GetViewportTransform(_In_ ID3D11DeviceContext* deviceContext, DXGI_MODE_ROTATION rotation)
	{
		// Look up the current viewport.
		if (!m_SetViewport)
		{
			UINT viewportCount = 1;

			deviceContext->RSGetViewports(&viewportCount, &m_Viewport);

			if (viewportCount != 1)
				throw std::exception("No viewport is set");
		}

		// Compute the matrix.
		float xScale = (m_Viewport.Width > 0) ? 2.0f / m_Viewport.Width : 0.0f;
		float yScale = (m_Viewport.Height > 0) ? 2.0f / m_Viewport.Height : 0.0f;

		switch (rotation)
		{
			case DXGI_MODE_ROTATION_ROTATE90:
				return XMMATRIX
				(
					0, -yScale, 0, 0,
					-xScale, 0, 0, 0,
					0, 0, 1, 0,
					1, 1, 0, 1
				);

			case DXGI_MODE_ROTATION_ROTATE270:
				return XMMATRIX
				(
					0, yScale, 0, 0,
					xScale, 0, 0, 0,
					0, 0, 1, 0,
					-1, -1, 0, 1
				);

			case DXGI_MODE_ROTATION_ROTATE180:
				return XMMATRIX
				(
					-xScale, 0, 0, 0,
					0, yScale, 0, 0,
					0, 0, 1, 0,
					1, -1, 0, 1
				);

			default:
				return XMMATRIX
				(
					xScale, 0, 0, 0,
					0, -yScale, 0, 0,
					0, 0, 1, 0,
					-1, 1, 0, 1
				);
		}
	}
#endif


	// Public constructor.
	SpriteBatch::SpriteBatch()
		: pImpl(new Impl(GetImmediateContext().Get()))
	{}


	// Move constructor.
	SpriteBatch::SpriteBatch(SpriteBatch&& moveFrom)
		: pImpl(std::move(moveFrom.pImpl))
	{}


	// Move assignment.
	SpriteBatch& SpriteBatch::operator= (SpriteBatch&& moveFrom)
	{
		pImpl = std::move(moveFrom.pImpl);
		return *this;
	}


	// Public destructor.
	SpriteBatch::~SpriteBatch()
	{}


	void XM_CALLCONV SpriteBatch::Begin(SpriteSortMode sortMode,
		ID3D11BlendState* blendState,
		ID3D11SamplerState* samplerState,
		ID3D11DepthStencilState* depthStencilState,
		ID3D11RasterizerState* rasterizerState,
		std::function<void()> setCustomShaders,
		FXMMATRIX transformMatrix)
	{
		pImpl->Begin(sortMode, blendState, samplerState, depthStencilState, rasterizerState, setCustomShaders, transformMatrix);
	}

	void SpriteBatch::End()
	{
		pImpl->End();
	}

	_Use_decl_annotations_
		void XM_CALLCONV SpriteBatch::Draw(ID3D11ShaderResourceView* texture,
			XMFLOAT2 const& position,
			RECT const* sourceRectangle,
			FXMVECTOR color,
			float rotation,
			XMFLOAT2 const& origin,
			XMFLOAT2 const& scale,
			SpriteEffects effects,
			float layerDepth) const
	{
		XMVECTOR destination = XMVectorPermute<0, 1, 4, 5>(XMLoadFloat2(&position), XMLoadFloat2(&scale)); // x, y, scale.x, scale.y

		XMVECTOR originRotationDepth = XMVectorSet(origin.x, origin.y, rotation, layerDepth);

		pImpl->Draw(texture, destination, sourceRectangle, color, originRotationDepth, static_cast<int>(effects));
	}


}
