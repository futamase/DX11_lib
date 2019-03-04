#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <d3d11_1.h>
#include <tchar.h>

namespace DX11
{

	/**@brief シェーダファイルをコンパイルする
	*@param[in] source ファイルへのパス
	*@param[in] entryPoint エントリポイント(関数名)
	*@param[in] target ターゲット(vs_4_0等)
	*@return バイトコード
	*/
	std::shared_ptr<ID3DBlob> CompileShader(const TCHAR source[], const CHAR entryPoint[], const CHAR target[]);

	/**@brief ピクセルシェーダを生成する
	*@param[in] device デバイス
	*@param[in] blob コンパイルしたシェーダのバイトコード
	*return ピクセルシェーダインターフェース
	*/
	std::shared_ptr<ID3D11PixelShader> CreatePixelShader(ID3D11Device* device, ID3DBlob * blob);

	/**@brief 頂点シェーダを生成する
	*@param[in] device デバイス
	*@param[in] blob コンパイルしたシェーダのバイトコード
	*@return 頂点シェーダインターフェース
	*/
	std::shared_ptr<ID3D11VertexShader> CreateVertexShader(ID3D11Device* device, ID3DBlob * blob);

	/**@brief ジオメトリシェーダを生成する
	*@param[in] device デバイス
	*@param[in] blob コンパイルしたシェーダのバイトコード
	*@return ジオメトリシェーダインターフェース
	*/
	std::shared_ptr<ID3D11GeometryShader> CreateGeometryShader(ID3D11Device* device, ID3DBlob * blob);

	/**@brief 入力レイアウトを生成する
	*@todo シェーダのバイトコードが頂点シェーダ限定なのか調査
	*@param[in] device デバイス
	*@param[in] layoutDesc レイアウトの記述配列
	*@param[in] numOfLayout 配列の長さ
	*@param[in] blob 頂点シェーダのバイトコード
	*@return 入力レイアウトインターフェース
	*/
	std::shared_ptr<ID3D11InputLayout> CreateInputLayout(
		ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC layoutDesc[], UINT numOfLayout, ID3DBlob * blob
	);

	/**@brief 入力レイアウトを生成する
	*@param[in] device デバイス
	*@param[in] vBlob 頂点シェーダのバイトコード
	*@return 入力レイアウトインターフェース
	*/
	std::shared_ptr<ID3D11InputLayout> CreateInputLayout(ID3D11Device* device, ID3DBlob* vBlob);

}

namespace tama
{
	/**@attention デフォルトコンストラクタはnullを設定したい特殊な時のみ使用*/
	class VertexShader
	{
		std::shared_ptr<ID3DBlob> m_blob;
		std::shared_ptr<ID3D11VertexShader>  m_shader;
	public:
		VertexShader() = default;
		explicit VertexShader(
			const std::basic_string<TCHAR>& path, 
			const std::string& entryPoint, 
			const std::string& target = "vs_4_0");
		ID3DBlob* GetBlob() const { return m_blob.get(); }
		ID3D11VertexShader* Get() const;
	};

	/**@attention デフォルトコンストラクタはnullを設定したい特殊な時のみ使用*/
	class GeometryShader
	{
		std::shared_ptr<ID3DBlob> m_blob;
		std::shared_ptr<ID3D11GeometryShader>  m_shader;
	public:
		GeometryShader() = default;
		explicit GeometryShader(
			const std::basic_string<TCHAR>& path, 
			const std::string& entryPoint, 
			const std::string& target = "gs_4_0");
		ID3D11GeometryShader* Get() const;
	};

	/**@attention デフォルトコンストラクタはnullを設定したい特殊な時のみ使用*/
	class PixelShader
	{
		std::shared_ptr<ID3DBlob> m_blob;
		std::shared_ptr<ID3D11PixelShader>  m_shader;
	public:
		PixelShader() = default;
		explicit PixelShader(
			const std::basic_string<TCHAR>& path, 
			const std::string& entryPoint, 
			const std::string& target = "ps_4_0");
		ID3D11PixelShader* Get() const;
		void Save(const std::string& filename)
		{
			std::ofstream st{ filename };
			auto size = m_blob->GetBufferSize();
			auto buf = (BYTE*)m_blob->GetBufferPointer();

			for (unsigned int i = 0; i < size; i++)
			{
				st << buf[i] << " ";
			}
			
		}
	};

	class InputLayout
	{
		std::shared_ptr<ID3D11InputLayout> m_layout;
	public:
		explicit InputLayout(const VertexShader& vs);
		InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elementList, const VertexShader& vs);
		operator ID3D11InputLayout*() { return m_layout.get(); }
	};
}
