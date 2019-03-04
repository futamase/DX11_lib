#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <d3d11_1.h>
#include <tchar.h>

namespace DX11
{

	/**@brief �V�F�[�_�t�@�C�����R���p�C������
	*@param[in] source �t�@�C���ւ̃p�X
	*@param[in] entryPoint �G���g���|�C���g(�֐���)
	*@param[in] target �^�[�Q�b�g(vs_4_0��)
	*@return �o�C�g�R�[�h
	*/
	std::shared_ptr<ID3DBlob> CompileShader(const TCHAR source[], const CHAR entryPoint[], const CHAR target[]);

	/**@brief �s�N�Z���V�F�[�_�𐶐�����
	*@param[in] device �f�o�C�X
	*@param[in] blob �R���p�C�������V�F�[�_�̃o�C�g�R�[�h
	*return �s�N�Z���V�F�[�_�C���^�[�t�F�[�X
	*/
	std::shared_ptr<ID3D11PixelShader> CreatePixelShader(ID3D11Device* device, ID3DBlob * blob);

	/**@brief ���_�V�F�[�_�𐶐�����
	*@param[in] device �f�o�C�X
	*@param[in] blob �R���p�C�������V�F�[�_�̃o�C�g�R�[�h
	*@return ���_�V�F�[�_�C���^�[�t�F�[�X
	*/
	std::shared_ptr<ID3D11VertexShader> CreateVertexShader(ID3D11Device* device, ID3DBlob * blob);

	/**@brief �W�I���g���V�F�[�_�𐶐�����
	*@param[in] device �f�o�C�X
	*@param[in] blob �R���p�C�������V�F�[�_�̃o�C�g�R�[�h
	*@return �W�I���g���V�F�[�_�C���^�[�t�F�[�X
	*/
	std::shared_ptr<ID3D11GeometryShader> CreateGeometryShader(ID3D11Device* device, ID3DBlob * blob);

	/**@brief ���̓��C�A�E�g�𐶐�����
	*@todo �V�F�[�_�̃o�C�g�R�[�h�����_�V�F�[�_����Ȃ̂�����
	*@param[in] device �f�o�C�X
	*@param[in] layoutDesc ���C�A�E�g�̋L�q�z��
	*@param[in] numOfLayout �z��̒���
	*@param[in] blob ���_�V�F�[�_�̃o�C�g�R�[�h
	*@return ���̓��C�A�E�g�C���^�[�t�F�[�X
	*/
	std::shared_ptr<ID3D11InputLayout> CreateInputLayout(
		ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC layoutDesc[], UINT numOfLayout, ID3DBlob * blob
	);

	/**@brief ���̓��C�A�E�g�𐶐�����
	*@param[in] device �f�o�C�X
	*@param[in] vBlob ���_�V�F�[�_�̃o�C�g�R�[�h
	*@return ���̓��C�A�E�g�C���^�[�t�F�[�X
	*/
	std::shared_ptr<ID3D11InputLayout> CreateInputLayout(ID3D11Device* device, ID3DBlob* vBlob);

}

namespace tama
{
	/**@attention �f�t�H���g�R���X�g���N�^��null��ݒ肵��������Ȏ��̂ݎg�p*/
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

	/**@attention �f�t�H���g�R���X�g���N�^��null��ݒ肵��������Ȏ��̂ݎg�p*/
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

	/**@attention �f�t�H���g�R���X�g���N�^��null��ݒ肵��������Ȏ��̂ݎg�p*/
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
