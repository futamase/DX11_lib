#include "Shader.h"
#include "DX11device.h"

#include "../../base/Debug.h"

#include <stdexcept>
#include <unordered_map>
#include <d3dcompiler.h>

#include <functional>

#pragma comment(lib, "d3dcompiler.lib")

namespace {
	template<typename Type> 
	std::shared_ptr<Type> CreateShader(
		ID3D11Device* device, BYTE* shader, size_t size,
		HRESULT(__stdcall ID3D11Device::*func)(const void *, SIZE_T, ID3D11ClassLinkage* , Type**)
		) {
		Type* shaderObject = nullptr;
		auto hr = (device->*func)(shader, size, nullptr, &shaderObject);
		if (FAILED(hr)) throw std::runtime_error("シェーダーの生成に失敗");
		return std::shared_ptr<Type>(shaderObject, [](IUnknown*p) { p->Release(); });
	}
}

namespace DX11
{
	std::shared_ptr<ID3DBlob> CompileShader(const TCHAR source[], const CHAR entryPoint[], const CHAR target[])
	{
		// 行列を列優先で設定し、古い形式の記述を許可しないようにする
		UINT flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;

		// 最適化レベルを設定する
#if defined(DEBUG) || defined(_DEBUG)
		flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
		flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

		ID3DBlob* blob = nullptr;
		auto hr = ::D3DCompileFromFile(source, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, target, flag1, 0, &blob, nullptr);
		if (FAILED(hr)) throw std::runtime_error("シェーダーファイルのコンパイルに失敗しました.");
		return std::shared_ptr<ID3DBlob>(blob, [](IUnknown*p) {p->Release(); });
	}

	std::shared_ptr<ID3D11PixelShader> CreatePixelShader(ID3D11Device* device, ID3DBlob * blob)
	{
		return CreateShader(device, (BYTE*)blob->GetBufferPointer(), blob->GetBufferSize(), &ID3D11Device::CreatePixelShader);
	}
	std::shared_ptr<ID3D11VertexShader> CreateVertexShader(ID3D11Device* device, ID3DBlob * blob)
	{
		return CreateShader(device, (BYTE*)blob->GetBufferPointer(), blob->GetBufferSize(), &ID3D11Device::CreateVertexShader);
	}
	std::shared_ptr<ID3D11GeometryShader> CreateGeometryShader(ID3D11Device* device, ID3DBlob * blob) {
		return CreateShader(device, (BYTE*)blob->GetBufferPointer(), blob->GetBufferSize(), &ID3D11Device::CreateGeometryShader);
	}

	std::shared_ptr<ID3D11InputLayout> CreateInputLayout(
		ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC layoutDesc[], UINT numOfLayout, ID3DBlob * blob
	) {
		ID3D11InputLayout* layout;
		auto hr = device->CreateInputLayout(
			layoutDesc, numOfLayout, (BYTE*)blob->GetBufferPointer(), blob->GetBufferSize(), &layout
		);
		if (FAILED(hr)) throw std::runtime_error("入力レイアウトの生成に失敗");
		return std::shared_ptr<ID3D11InputLayout>(layout, [](IUnknown*p) {p->Release(); });
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputElementDesc(ID3DBlob* blob);
	std::shared_ptr<ID3D11InputLayout> CreateInputLayout(ID3D11Device* device, ID3DBlob* vBlob)
	{
		ID3D11InputLayout* layout;

		auto elementList = GetInputElementDesc(vBlob);

		auto hr = device->CreateInputLayout(
			elementList.data(), elementList.size(),
			vBlob->GetBufferPointer(), vBlob->GetBufferSize(),
			&layout);
		if (FAILED(hr)) throw std::runtime_error("入力レイアウトの生成に失敗");

		return std::shared_ptr<ID3D11InputLayout>(layout, [](IUnknown*p) {p->Release(); });
	}

	//----------------------------------------ShaderReflection---------------------------------------------------------
	//とりあえずFLOATのみに対応
	DXGI_FORMAT GetDxgiFormat(D3D10_REGISTER_COMPONENT_TYPE type, BYTE mask)
	{
		if (mask == 0x0F) { //4成分
			switch (type) {
				case D3D10_REGISTER_COMPONENT_FLOAT32:
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
				case D3D10_REGISTER_COMPONENT_SINT32:
					return DXGI_FORMAT_R32G32B32A32_SINT;
				case D3D10_REGISTER_COMPONENT_UINT32:
					return DXGI_FORMAT_R32G32B32A32_UINT;
				default:
					return DXGI_FORMAT_UNKNOWN;
			}
		}

		if (mask == 0x07) { //3成分
			switch (type) {
				case D3D10_REGISTER_COMPONENT_FLOAT32:
					return DXGI_FORMAT_R32G32B32_FLOAT;
				case D3D10_REGISTER_COMPONENT_SINT32:
					return DXGI_FORMAT_R32G32B32_SINT;
				case D3D10_REGISTER_COMPONENT_UINT32:
					return DXGI_FORMAT_R32G32B32_UINT;
				default:
					return DXGI_FORMAT_UNKNOWN;
			}
		}

		if (mask == 0x3) { //2成分
			switch (type) {
				case D3D10_REGISTER_COMPONENT_FLOAT32:
					return DXGI_FORMAT_R32G32_FLOAT;
				case D3D10_REGISTER_COMPONENT_SINT32:
					return DXGI_FORMAT_R32G32_SINT;
				case D3D10_REGISTER_COMPONENT_UINT32:
					return DXGI_FORMAT_R32G32_UINT;
				default:
					return DXGI_FORMAT_UNKNOWN;
			}
		}

		if (mask == 0x1) { //1成分
			switch (type) {
				case D3D10_REGISTER_COMPONENT_FLOAT32:
					return DXGI_FORMAT_R32_FLOAT;
				case D3D10_REGISTER_COMPONENT_SINT32:
					return DXGI_FORMAT_R32_SINT;
				case D3D10_REGISTER_COMPONENT_UINT32:
					return DXGI_FORMAT_R32_UINT;
				default:
					return DXGI_FORMAT_UNKNOWN;
			}
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	std::unordered_map<ID3DBlob*, std::shared_ptr<ID3D11ShaderReflection>> shaderReflectionList;

	//! シェーダの記述を取得する(floatのみ)
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputElementDesc(ID3DBlob* blob)
	{
		ID3D11ShaderReflection* reflection;
		if (FAILED(D3DReflect(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			IID_ID3D11ShaderReflection,
			reinterpret_cast<void**>(&reflection)
		)))
			throw std::runtime_error("D3DReflect");

		shaderReflectionList.emplace(blob, std::shared_ptr<ID3D11ShaderReflection>(reflection, [](IUnknown* p) {p->Release(); }));

		D3D11_SHADER_DESC desc;
		if (FAILED(reflection->GetDesc(&desc)))
			throw std::runtime_error("ID3D11ShaderReflection::GetDesc");

		std::vector<D3D11_INPUT_ELEMENT_DESC> elementList;
		for (UINT i = 0; i < desc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			if (FAILED(reflection->GetInputParameterDesc(i, &paramDesc)))
				throw std::runtime_error("ID3D11ShaderReflection::GetInputParameterDesc");

			auto format = GetDxgiFormat(paramDesc.ComponentType, paramDesc.Mask);
			D3D11_INPUT_ELEMENT_DESC element = {
				paramDesc.SemanticName,
				paramDesc.SemanticIndex,
				format,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			};
			elementList.push_back(element);
		}

		return elementList;
	}


}

namespace tama
{

	VertexShader::VertexShader(
		const std::basic_string<TCHAR>& path,
		const std::string& entryPoint,
		const std::string& target)
	{
		m_blob = DX11::CompileShader(path.data(), entryPoint.data(), target.data());

		m_shader = DX11::CreateVertexShader(DX11::Device::Get(), m_blob.get());
	}

	ID3D11VertexShader* VertexShader::Get() const 
	{ 
		if (!m_shader)
			tama::Debug::Log(L"VertexShader is null. it was not set.");
		
		return m_shader.get(); 
	}

	GeometryShader::GeometryShader(
		const std::basic_string<TCHAR>& path,
		const std::string& entryPoint,
		const std::string& target)
	{
		m_blob = DX11::CompileShader(path.data(), entryPoint.data(), target.data());

		m_shader = DX11::CreateGeometryShader(DX11::Device::Get(), m_blob.get());
	}

	ID3D11GeometryShader* GeometryShader::Get() const
	{
		if (!m_shader)
			tama::Debug::Log(L"GeometryShader is null. it was not set.");
		
		return m_shader.get(); 
	}

	PixelShader::PixelShader(
		const std::basic_string<TCHAR>& path,
		const std::string& entryPoint,
		const std::string& target)
	{
		m_blob = DX11::CompileShader(path.data(), entryPoint.data(), target.data());

		m_shader = DX11::CreatePixelShader(DX11::Device::Get(), m_blob.get());
	}

	ID3D11PixelShader* PixelShader::Get() const 
	{ 
		if (!m_shader)
			tama::Debug::Log(L"PixelShader is null. it was not set.");
		
		return m_shader.get(); 
	}

	InputLayout::InputLayout(const VertexShader& vs)
	{
		m_layout = DX11::CreateInputLayout(DX11::Device::Get(), vs.GetBlob());
	}

	InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elementList, const VertexShader& vs)
	{
		m_layout = DX11::CreateInputLayout(DX11::Device::Get(), elementList.data(), elementList.size(), vs.GetBlob());
	}
}