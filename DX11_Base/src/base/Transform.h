#pragma once

#include "Math.h"

namespace tama
{
	/**@brief �Q�����p�����
	*@detail �p���O��
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

		//! ���W�A��
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

		/**@brief ���[���h���W�ϊ��s����쐬
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

	/**@brief �R�����p�����
	*@detail �p���O��
	*/
	class Transform3D
	{
	protected:
		/**@brief �R���X�g���N�^*/
		Transform3D();
		/**@brief �f�X�g���N�^*/
		virtual ~Transform3D();
		Vec3 m_scale, m_pos;
		Quaternion m_rotate;

	public:
		/**@brief ���W��ύX
		*@param[in] x x���W
		*@param[in] y y���W
		*@param[in] z z���W
		*/
		void SetPos(float x, float y, float z)
		{
			m_pos = Vec3(x, y, z);
		}

		/**@brief ���W��ύX
		*@param[in] pos ���W
		*/
		void SetPos(const Vec3& pos)
		{
			m_pos = pos;
		}

		/**@brief ���W���ړ�
		*@param[in] vecX x�������̈ړ���
		*@param[in] vecY y�������̈ړ���
		*@param[in] vecZ z�������̈ړ���
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

		/**@brief �g��k�����s��
		*@param[in] x x������
		*@param[in] y y������
		*@param[in] z z������
		*/
		void Scaling(float x, float y, float z)
		{
			m_scale = Vec3(x, y, z);
		}

		/**@brief �g��k�����s��
		*@param[in] scale �X�P�[��
		*/
		void Scaling(const Vec3& scale)
		{
			m_scale = scale;
		}

		/**@brief ���W������ɉ�](�x)
		*@param[in] x x��
		*@param[in] y y��
		*@param[in] z z��
		*/
		void Rotate(const Vec3& axis, float angle)
		{
			m_rotate += Quaternion::CreateFromAxisAngle(axis, angle);
		}

		/**@brief ���΍��W����ɉ�](���Ȃ�)
		*@param[in] yaw �����p
		*@param[in] pitch �����p
		*@param[in] roll ��]�p
		*/
		void Rotate(float yaw, float pitch, float roll)
		{
			m_rotate += Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
		}

		/**@brief �s�����ɉ�]
		*@param[in] mat ��]�s��
		*/
		void Rotate(const Matrix& mat)
		{
			m_rotate += Quaternion::CreateFromRotationMatrix(mat);
		}

		/**@brief �N�I�[�^�j�I������ɉ�]
		*@param[in] q �N�I�[�^�j�I��*/
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
