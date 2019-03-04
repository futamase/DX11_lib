#include "Camera.h"

#include "../../System/Input.h"

namespace DirectX
{
	inline XMFLOAT3 operator+ (const XMFLOAT3& f1, const XMFLOAT3& f2)
	{
		return XMFLOAT3(f1.x + f2.x, f1.y + f2.y, f1.z + f2.z);
	}

	inline XMFLOAT3 operator- (const XMFLOAT3& f1, const XMFLOAT3& f2)
	{
		return XMFLOAT3(f1.x - f2.x, f1.y - f2.y, f1.z - f2.z);
	}
}

// カメラにはそもそも平行投影と透視投影がある
// 透視：
//	簡単に言えば近くの物がより大きく、遠くの物がより小さく見える投影法で、
//	3D ゲームの世界を描画する方法として自然に感じられる方式です。
//	視野角が広いほど、より多くの物をカメラに映すことができますが、個々の物体はパースされてより小さく見える。
//  視野角は垂直方向の角度で、水平方向の範囲はアスペクト比から計算されます。
// 平行：
//	。代わりに、正投影では投影範囲を表す 

namespace DX11
{
	using namespace DirectX;

	DirectX::XMMATRIX Camera::CreatePerspectiveProjection() const
	{
		return XMMatrixTranspose(
			XMMatrixPerspectiveFovLH(
				XMConvertToRadians(m_desc.perspective.fieldOfView),
				m_desc.perspective.aspect,		
				m_desc.nearClip,
				m_desc.farClip
				));
	}

	DirectX::XMMATRIX Camera::CreateOrthographicProjection() const
	{
		return XMMatrixTranspose(
			XMMatrixOrthographicLH(
				m_desc.orthographic.viewWidth,
				m_desc.orthographic.viewHeight,
				m_desc.nearClip,
				m_desc.farClip
				));
	}

	DirectX::XMMATRIX Camera::CreateTwoDimensionProjection() const
	{
		static float a = 0;
		if (!Input::GetKey(Input::KeyCode::Right))
			a+=0.01f;
		DirectX::XMMATRIX proj(
			2.f / (m_desc.twoDimensional.windowWidth+a), 0.0f, 0.0f, 0.0f,
			0.0f, -2.f / m_desc.twoDimensional.windowHeight, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
			);
		return XMMatrixTranspose(proj);
	}


	Camera::Camera(ProjType type, const CameraDesc& desc) :
		m_buffer(nullptr, sizeof(cCameraBuffer)),
		m_desc(desc)
	{
		if (type != desc.GetType())
			throw std::exception("指定した投影タイプは何かおかしい");
	}

	void Camera::Init(const XMFLOAT3& pos, const XMFLOAT3& focus, const XMFLOAT2& screenSize)
	{
		m_pos = pos;
		m_focus = focus;

		m_desc.twoDimensional.windowWidth = screenSize.x;
		m_desc.twoDimensional.windowHeight = screenSize.y;


		m_qua.x = m_qua.z = 0;
		m_qua.y = 1;

		RockOnFlag = false;

		m_desc.perspective.aspect = screenSize.x / screenSize.y;
	}
	/*
	const ConstantBuffer::cCameraBuffer& Camera::GetBuffer(){
	m_cameraBuf.eyePos = XMFLOAT4(m_pos.x, m_pos.y, m_pos.z, 1.0f);
	return m_cameraBuf;
	}
	*/
	void Camera::SetCam(const XMFLOAT3& pos, const XMFLOAT3& focus)
	{
//		m_pos = pos;
//		m_focus = focus;
//		if ((m_pos.x == m_focus.x) && (m_pos.y == m_focus.y) && (m_pos.z == m_focus.z))//LookAtLH用の補正
//			m_pos.z += 0.01f;
//		XMStoreFloat4x4(&m_cameraBuf->matView,
//			XMMatrixTranspose(
//				XMMatrixLookAtLH(
//					XMLoadFloat3(&m_pos),	//カメラの位置
//					XMLoadFloat3(&m_focus),	//カメラが注目する座標
//					XMLoadFloat3(&m_qua))));	//カメラの上向きを設定するベクトル
	}


	void Camera::SetPos(const XMFLOAT3& pos)
	{
		m_pos = pos;
	}

	void Camera::SetFocus(const XMFLOAT3& focus)
	{
		m_focus = focus;
	}

	void Camera::SetOrtho(const XMFLOAT3& eyePos)
	{
		XMFLOAT4X4 matLightProjection, matLightView;
		XMStoreFloat4x4(&matLightProjection, XMMatrixIdentity());
		//XMStoreFloat4x4(&matLightProjection, XMMatrixOrthographicLH(30.f,22.5f, 1.f, 1000.f));
		XMStoreFloat4x4(&matLightProjection, XMMatrixOrthographicLH(1200.f, 900.f, 1.f, 10000.f));
		XMStoreFloat4x4(&matLightView, XMMatrixIdentity());
		//if (eyePos.z == 0)eyePos.z = 0.01f;
		XMStoreFloat4x4(&matLightView, XMMatrixLookAtLH(
			XMLoadFloat3(&XMFLOAT3(eyePos.x * 1.0f, eyePos.y * 1.0f, eyePos.z * 1.0f)),
			XMLoadFloat3(&XMFLOAT3(0, 0, 0)),
			XMLoadFloat3(&XMFLOAT3(0.f, 1.f, 0.f))));
//		XMStoreFloat4x4(&m_cameraBuf->matLVP, XMMatrixTranspose(XMLoadFloat4x4(&matLightView)*XMLoadFloat4x4(&matLightProjection)));
	}

	void Camera::SetOrtho(const XMFLOAT3& eyePos, const XMFLOAT3& focus)
	{
		XMFLOAT4X4 matLightProjection, matLightView;
		XMStoreFloat4x4(&matLightProjection, XMMatrixIdentity());
		XMStoreFloat4x4(&matLightProjection, XMMatrixOrthographicLH(1200.f, 900.f, 1.f, 1000.f));
		XMStoreFloat4x4(&matLightView, XMMatrixIdentity());
		//if (eyePos.z == 0)eyePos.z = 0.01f;
		XMStoreFloat4x4(&matLightView, XMMatrixLookAtLH(
			XMLoadFloat3(&XMFLOAT3(eyePos.x + focus.x, eyePos.y + focus.y, eyePos.z + focus.z)),
			XMLoadFloat3(&focus),
			XMLoadFloat3(&XMFLOAT3(0.f, 1.f, 0.f))));
//		XMStoreFloat4x4(&m_cameraBuf->matLVP, XMMatrixTranspose(XMLoadFloat4x4(&matLightView)*XMLoadFloat4x4(&matLightProjection)));
	}

	void Camera::RockOn(const XMFLOAT3& newFocus)
	{
		float rockSpd = 10.0f;
		XMFLOAT3 spd;
		m_pos = newFocus - m_focus;
		spd.x /= rockSpd; spd.y /= rockSpd; spd.z /= rockSpd;
		m_focus = m_focus + spd;
	}
	bool Camera::CheckInScreen(const XMFLOAT3& pos)
	{
		XMFLOAT3 Out;
//		XMStoreFloat3(&Out, XMVector3Transform(XMLoadFloat3(&pos), XMLoadFloat4x4(&m_cameraBuf->matView)));
//		XMStoreFloat3(&Out, XMVector3Transform(XMLoadFloat3(&Out), XMLoadFloat4x4(&m_cameraBuf->matProjection)));
		Out.x /= Out.z; Out.y /= Out.z; Out.z /= Out.z;
		if ((fabs(Out.x) <= 1.2f) && (fabs(Out.y) <= 1.2f))
			return true;
		return false;
	}

	void Camera::SetBuffer() const
	{
		static cCameraBuffer data;

		switch (m_desc.GetType())
		{
			case ProjType::Perspective:
				XMStoreFloat4x4(&data.matView,
					XMMatrixTranspose(
						XMMatrixLookAtLH(
							XMLoadFloat3(&m_pos),	
							XMLoadFloat3(&m_focus),
							XMLoadFloat3(&m_qua))));	

				XMStoreFloat4x4(&data.matProjection,
					this->CreatePerspectiveProjection());
				break;

			case ProjType::Orthographic:
				XMStoreFloat4x4(&data.matView,
					XMMatrixTranspose(
						XMMatrixLookAtLH(
							XMLoadFloat3(&m_pos),	
							XMLoadFloat3(&m_focus),	
							XMLoadFloat3(&m_qua))));	

				XMStoreFloat4x4(&data.matProjection,
					this->CreateOrthographicProjection());
				break;

			case ProjType::TwoDimensional:
				XMStoreFloat4x4(&data.matView,
					XMMatrixIdentity());

				XMStoreFloat4x4(&data.matProjection,
					this->CreateTwoDimensionProjection());
				break;
		}

		m_buffer.Set(&data, 1, ShaderStage::VS);
	}
}