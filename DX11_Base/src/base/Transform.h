#pragma once

#include "Math.h"

namespace tama
{
	/**@brief ２次元姿勢情報
	*@detail 継承前提
	*/
	class Transform2D
	{
	protected:
		Vec2 m_position, m_scaling;
		float m_rotation;
		Vec2 m_rotationOrigin, m_scalingOrigin;

	public:
		Transform2D() :
			m_position(0, 0),
			m_rotation(0),
			m_scaling(1, 1)
		{};

		Transform2D(float centerX, float centerY)
			: Transform2D()
		{
			this->SetOrigin(centerX, centerY);
		}

		virtual ~Transform2D() = default;

		void SetPos(float x, float y)
		{
			m_position.x = x;
			m_position.y = y;
		}

		void SetPos(const Vec2& pos)
		{
			m_position = pos;
		}

		Vec2 GetPos() const
		{
			return m_position;
		}

		void Translate(float x, float y)
		{
			m_position.x += x;
			m_position.y += y;
		}

		void Translate(const Vec2& vec)
		{
			m_position += vec;
		}

		//! ラジアン
		void Rotate(float rotation)
		{
			m_rotation += rotation;
		}

		void Scale(float x, float y)
		{
			m_scaling.x = x;
			m_scaling.y = y;
		}

		void Scale(const Vec2& scale)
		{
			m_scaling = scale;
		}

		void SetOrigin(float x, float y)
		{
			m_rotationOrigin = m_scalingOrigin = Vec2(x, y);
		}

		void SetOrigin(const Vec2& origin)
		{
			m_rotationOrigin = m_scalingOrigin = origin;
		}

		/**@brief ワールド座標変換行列を作成
		*/
		static Matrix CreateWorldMatrix(const Transform2D& tr)
		{
			return DirectX::XMMatrixTranspose(
				DirectX::XMMatrixTransformation2D(
					DirectX::XMLoadFloat2(&tr.m_scalingOrigin),
					0, // scalingOrientation ?
					DirectX::XMLoadFloat2(&tr.m_scaling),
					DirectX::XMLoadFloat2(&tr.m_rotationOrigin),
					tr.m_rotation,
					DirectX::XMLoadFloat2(&tr.m_position)));
		}
	};

	/**@brief ３次元姿勢情報
	*@detail 継承前提
	*/
	class Transform3D
	{
	protected:
		/**@brief コンストラクタ*/
		Transform3D();
		/**@brief デストラクタ*/
		virtual ~Transform3D();
		Vec3 m_scale, m_pos;
		Quaternion m_rotate;

	public:
		/**@brief 座標を変更
		*@param[in] x x座標
		*@param[in] y y座標
		*@param[in] z z座標
		*/
		void SetPos(float x, float y, float z)
		{
			m_pos = Vec3(x, y, z);
		}

		/**@brief 座標を変更
		*@param[in] pos 座標
		*/
		void SetPos(const Vec3& pos)
		{
			m_pos = pos;
		}

		/**@brief 座標を移動
		*@param[in] vecX x軸方向の移動量
		*@param[in] vecY y軸方向の移動量
		*@param[in] vecZ z軸方向の移動量
		*/
		void Translate(float x, float y, float z)
		{
			m_pos.x += x;
			m_pos.y += y;
			m_pos.z += z;
		}

		void Translate(const Vec3& v)
		{
			m_pos += v;
		}

		/**@brief 拡大縮小を行う
		*@param[in] x x軸方向
		*@param[in] y y軸方向
		*@param[in] z z軸方向
		*/
		void Scaling(float x, float y, float z)
		{
			m_scale = Vec3(x, y, z);
		}

		/**@brief 拡大縮小を行う
		*@param[in] scale スケール
		*/
		void Scaling(const Vec3& scale)
		{
			m_scale = scale;
		}

		/**@brief 座標軸を基に回転(度)
		*@param[in] x x軸
		*@param[in] y y軸
		*@param[in] z z軸
		*/
		void Rotate(const Vec3& axis, float angle)
		{
			m_rotate += Quaternion::CreateFromAxisAngle(axis, angle);
		}

		/**@brief 相対座標を基に回転(しない)
		*@param[in] yaw 水平角
		*@param[in] pitch 垂直角
		*@param[in] roll 回転角
		*/
		void Rotate(float yaw, float pitch, float roll)
		{
			m_rotate += Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
		}

		/**@brief 行列を基に回転
		*@param[in] mat 回転行列
		*/
		void Rotate(const Matrix& mat)
		{
			m_rotate += Quaternion::CreateFromRotationMatrix(mat);
		}

		/**@brief クオータニオンを基に回転
		*@param[in] q クオータニオン*/
		void Rotate(const Quaternion& q)
		{
			m_rotate += q;
		}

		static Matrix CreateWorldMatrix(const Transform3D& mat)
		{
			return (Matrix::CreateScale(mat.m_scale) *
				Matrix::CreateFromQuaternion(mat.m_rotate) *
				Matrix::CreateTranslation(mat.m_pos)).Transpose();
		}
	};

}
