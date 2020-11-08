#pragma once

#include "OrthographicCamera.h"

#include <algorithm>

#include <Events/WindowEvent.h>
#include <Events/MouseEvent.h>

#include <Core/PlatformInfo.h>
#include <Core/Input.h>
#include <Core/Ref.h>
#include <Events/InputCodes.h>

namespace Nebula {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(float ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera* GetCamera() { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }

		Mat4f GetTransform();

		void SetAspectRatio(float ar) { m_AspectRatio = ar; }

		void Reset();
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera* m_Camera;

		bool m_Rotation;

		Vec3f m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}