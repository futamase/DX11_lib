#include "Primitive.h"
#include "../../DX/DX11/VertexData.h"
#include "../../DX/DX11/DXResource.h"

namespace Primitive
{
	struct Cube::Impl
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VB, m_IB;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	
		Impl()
		{
			m_context = DX11::GetImmediateContext();
		}
	};

	Cube::Cube()
	{
		m_impl = std::make_unique<Impl>();

		std::array<DX11::DefaultVertex, 24> vertices;

		DirectX::XMFLOAT3 pos[] = 
		{
			{ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f },

			{ -0.5f, -0.5f, 0.5f },	{ 0.5f, -0.5f, 0.5f }, { -0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f },

			{ -0.5f, -0.5f, 0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f, 0.5f, 0.5f }, { -0.5f, 0.5f, -0.5f },

			{ 0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, -0.5f },

			{ -0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { -0.5f, 0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f },

			{ -0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, 0.5f }, { -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }
		};

		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i].pos = pos[i];
		}

		uint16_t indices[] =
		{
			// ‘O
			2, 3, 0, 1,
			// Œã‚ë
			7, 6, 5, 4, 
			// ‰E
			3,7,1,5,
			// ¶
			6,2,4,0,
			// ã
			6, 2, 7, 3,
			// ‰º
			0, 4, 1, 5,
		};

		m_impl->m_VB = DX11::CreateBuffer(
			vertices.data(),
			sizeof(DX11::DefaultVertex) * vertices.size(),
			D3D11_CPU_ACCESS_WRITE,
			D3D11_BIND_VERTEX_BUFFER);

		m_impl->m_IB = DX11::CreateBuffer(&indices, sizeof(indices), 0, D3D11_BIND_INDEX_BUFFER);
	}

	Cube::~Cube() = default;

	void Cube::Draw() const
	{
//		this->SetWorldBuffer();

		auto pvb = m_impl->m_VB.Get();
		auto stride = sizeof(DX11::DefaultVertex);
		UINT offset = 0;
		m_impl->m_context->IASetVertexBuffers(0, 1, &pvb, &stride, &offset);
		m_impl->m_context->IASetIndexBuffer(m_impl->m_IB.Get(), DXGI_FORMAT_R16_UINT, 0);
		m_impl->m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		m_impl->m_context->DrawIndexed(24, 0, 0);
	}
}
