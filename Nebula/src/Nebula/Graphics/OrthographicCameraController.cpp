#include "OrthographicCameraController.h"

#include <algorithm>

#include <Core/PlatformInfo.h>
#include <Core/Input.h>
#include <Events/InputCodes.h>

namespace Nebula {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(new OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(float ts)
	{
		if (Input::IsKeyPressed(KeyCode::R))
		{
			Reset();
		}

		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_CameraPosition.x -= cos((PI / 180) * (m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin((PI / 180) * (m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(KeyCode::D))
		{
			m_CameraPosition.x += cos((PI / 180) * (m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin((PI / 180) * (m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(KeyCode::W))
		{
			m_CameraPosition.x += -sin((PI / 180) * (m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos((PI / 180) * (m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(KeyCode::S))
		{
			m_CameraPosition.x -= -sin((PI / 180) * (m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos((PI / 180) * (m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(KeyCode::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(KeyCode::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera->SetRotation(m_CameraRotation);
		}

		m_Camera->SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	Mat4f OrthographicCameraController::GetTransform()
	{
		Mat4f transform = Mat4f(1.0f) * Mat4f::rotation(PI2R_FACTOR * m_CameraRotation, Vec3f(0,0,1)) * Mat4f::translation(m_CameraPosition);
		return transform;
	}

	void OrthographicCameraController::Reset()
	{
		m_ZoomLevel = 1.0f;
		m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		m_CameraRotation = 0.0f; 
		m_CameraTranslationSpeed = 5.0f;
		m_CameraRotationSpeed = 180.0f;
		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(NEB_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(NEB_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.05f;
		m_ZoomLevel = m_ZoomLevel <= 0.15f ? 0.15f : m_ZoomLevel;
		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetScreenSize().x, (float)e.GetScreenSize().x);
		return false;
	}

}