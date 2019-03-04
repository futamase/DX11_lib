#pragma once

#include <DirectXMath.h>
#include "ConstantBuffer.h"

namespace DX11
{
	struct cCameraBuffer
	{
		DirectX::XMFLOAT4X4	matView;
		DirectX::XMFLOAT4X4	matProjection;
		DirectX::XMFLOAT4X4  matViewPort;		//����ViewPort�̋t�s��
		DirectX::XMFLOAT4X4	matLVP;
		DirectX::XMFLOAT4 eyePos;
		cCameraBuffer()
		{
			this->Init();
		}
		void Init()
		{
			DirectX::XMStoreFloat4x4(&matProjection, DirectX::XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&matView, DirectX::XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&matViewPort, DirectX::XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&matLVP, DirectX::XMMatrixIdentity());
			eyePos = DirectX::XMFLOAT4{ 0.f, 0.f, 0.f, 0.f };
		}
	};

	enum class ProjType
	{
		Perspective,	// �����ϊ�
		Orthographic,	// ���ˉe�ϊ�
		TwoDimensional,	// 2D��p�ϊ�(���̃^�C�v�̂Ƃ��̓J�����̎p�����͖��������)
	};

	struct CameraDesc
	{
		ProjType type;
		float nearClip, farClip; // �N���b�s���O����
		union {
			struct {
				float fieldOfView;	// y�������̎���(�x)
				float aspect;		// �A�X�y�N�g�� 
			};

			struct {
				float viewWidth;	// �߂����̃N���b�v���ʂ̕�
				float viewHeight;	// �����V
			};

			struct {
				float windowWidth;
				float windowHeight;
			};
		};
	public:
		CameraDesc(float nearZ, float farZ) :
			type(ProjType::TwoDimensional),
			nearClip(nearZ),
			farClip(farZ)
		{}

		CameraDesc(float nearZ, float farZ, int fov, float aspect) :
			type(ProjType::Perspective),
			nearClip(nearZ),
			farClip(farZ)
		{
			fieldOfView = static_cast<float>(fov);
			aspect = aspect;
		}
		CameraDesc(float nearZ, float farZ, float nearW, float nearH)
		{
			type = ProjType::Orthographic;
			nearClip = nearZ;
			farClip = farZ;
			viewWidth = nearW;
			viewHeight = nearH;
		}
	};

	// �J�����ɗ~�����@�\
	// �ˉe�ϊ��̐؂�ւ�
	// �ˉe�֘A�̊e�֐�
	// �����_�[�e�N�X�`�������
	class Camera
	{

	private:
		ConstantBufferBase m_buffer;

		DirectX::XMFLOAT3 m_pos;
		DirectX::XMFLOAT3 m_focus;
		DirectX::XMFLOAT3 m_qua;

		CameraDesc m_desc;

		bool RockOnFlag;

		const float ProjectionDegree = 30.f;
		float screenAspect = 0.f;

	private:
		DirectX::XMMATRIX CreatePerspectiveProjection() const;
		DirectX::XMMATRIX CreateOrthographicProjection() const;
		DirectX::XMMATRIX CreateTwoDimensionProjection() const;

	public:
		Camera(ProjType projType, const CameraDesc& desc);
		void Init(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT2& screenSize);
		void SetPos(const DirectX::XMFLOAT3& pos);
		void SetFocus(const DirectX::XMFLOAT3& focus);
		void SetCam(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& focus);
		void SetOrtho(const DirectX::XMFLOAT3& eyePos);
		void SetOrtho(const DirectX::XMFLOAT3& eyePos, const DirectX::XMFLOAT3& focus);
		void RockOn(const DirectX::XMFLOAT3& newFocus);
		bool CheckInScreen(const DirectX::XMFLOAT3& pos);
		void SetBuffer() const;
		//		const ConstantBuffer::cCameraBuffer& GetBuffer();

		const DirectX::XMFLOAT3& GetPos() const {
			return m_pos;
		};
		const DirectX::XMFLOAT3& GetFoc() const {
			return m_focus;
		};
	};

}