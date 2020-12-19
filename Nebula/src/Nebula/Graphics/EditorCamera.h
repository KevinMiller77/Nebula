#pragma once

#include <Math/Math.h>

#include "Camera.h"
#include <Events/Event.h>
#include <Events/MouseEvent.h>

namespace Nebula {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() { UpdateProjection(); };
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(float ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const Mat4f& GetViewMatrix() const { return m_ViewMatrix; }
		const Mat4f GetViewProjection() const { return m_ViewMatrix * m_Projection; }

		Vec3f GetUpDirection() const;
		Vec3f GetRightDirection() const;
		Vec3f GetForwardDirection() const;
		const Vec3f& GetPosition() const { return m_Position; }
		Quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const Vec2f& delta);
		void MouseRotate(const Vec2f& delta);
		void MouseZoom(float delta);

		Vec3f CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		Mat4f m_ViewMatrix;
		Vec3f m_Position = { 0.0f, 0.0f, 0.0f };
		Vec3f m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		Vec2f m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 5.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}
