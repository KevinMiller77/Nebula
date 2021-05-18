#include "EditorCamera.h"

#include "Nebula/Core/Input.h"
#include <glfw/glfw3.h>
#include <Core/Ref.h>

namespace Nebula {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(Mat4f::perspective(fov, aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = Mat4f::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();
        printf("Editor Cam Pos: %f, %f, %f\n", m_Position.X, m_Position.Y, m_Position.Z);

		Quat orientation = GetOrientation();
		m_ViewMatrix = orientation.AsMat4f() * Mat4f::translation(m_Position);
        m_ViewMatrix = m_ViewMatrix.invertMatrix();

        LOG_DBG("Printing view matrix");
        for (int i = 0; i < 4; i++) {
            LOG_NAKED("\n\t [ ");
            for (int j = 0; j < 4; j++) {
                LOG_NAKED("%.02f, ", m_ViewMatrix.elements[j + i * 4]);
            }
            LOG_NAKED("]\n");
        }
        LOG_NAKED("\n");

		// m_ViewMatrix = m_ViewMatrix.invertMatrix();
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.6f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(float ts)
	{
		if (Input::IsKeyPressed(KeyCode::LeftAlt))
		{
			const Vec2f& mouse = Input::GetMousePos();

			Vec2f delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(MouseCode::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(MouseCode::ButtonRight))
				MouseZoom(delta.Y);
			else if (Input::IsKeyPressed(KeyCode::R))
				*this = EditorCamera(45.0f, m_ViewportWidth / m_ViewportHeight, 0.1f, 1000.0f);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(NEB_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const Vec2f& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -1.0f * GetRightDirection() * delta.X * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.Y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const Vec2f& delta)
	{
		float yawSign = GetUpDirection().Y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.X * RotationSpeed();
		m_Pitch += delta.Y * RotationSpeed();

		// LOG_INF("Yaw: %f\tPitch: %f\n", m_Yaw, m_Pitch);
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	Vec3f EditorCamera::GetUpDirection() const
	{
		return GetOrientation().Rotate({0.0f, 1.0f, 0.0f});
	}

	Vec3f EditorCamera::GetRightDirection() const
	{
		return GetOrientation().Rotate({1.0f, 0.0f, 0.0f});
	}

	Vec3f EditorCamera::GetForwardDirection() const
	{
		return GetOrientation().Rotate({0.0f, 0.0f, -1.0f});
	}

	Vec3f EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	Quat EditorCamera::GetOrientation() const
	{
		return Quat(Vec3f(-m_Pitch, -m_Yaw, 0.0f));
	}

}
