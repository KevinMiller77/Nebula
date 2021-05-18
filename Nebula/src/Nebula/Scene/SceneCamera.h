#pragma once

#include <stdio.h>
#include <Graphics/Camera.h>
#include <Math/math.h>
#include <Utils/Logging.h>

namespace Nebula {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		SceneCamera(float aspectRatio) : Camera() { AspectRatio = aspectRatio; }
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(unsigned int width, unsigned int height);
		
        virtual const Mat4f& GetViewMatrix() const override { return ViewMatrix; };
        virtual const Mat4f GetViewProjection() const override { return ViewMatrix * m_Projection; }

		float GetPerspectiveVerticalFOV() const { return PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float verticalFov) { PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { PerspectiveFar = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return OrthographicSize; }
		void SetOrthographicSize(float size) { OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { OrthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { OrthographicFar = farClip; RecalculateProjection(); }

		void SetFixedAspectRatio(bool isFixed) { HasFixedAspectRatio = isFixed; RecalculateProjection(); }
		bool GetFixedAspectRatio() { return HasFixedAspectRatio; }

		ProjectionType GetProjectionType() const { return CamProjectionType; }
		void SetProjectionType(ProjectionType type) { CamProjectionType = type; RecalculateProjection(); }
		

		bool WantsMainCamera() { return mainCamera; }
		void WantsMainCamera(bool mainCam) { mainCamera = mainCam; }

	private:
		void RecalculateProjection();
	private:
		ProjectionType CamProjectionType = ProjectionType::Orthographic;

		Mat4f ViewMatrix = Mat4f(1.0f);

		//Degrees
		float PerspectiveFOV = 75.0f;
		float PerspectiveNear = 0.01f, PerspectiveFar = 1000.0f;

		float OrthographicSize = 10.0f;
		float OrthographicNear = -1.0f, OrthographicFar = 1.0f;

		float AspectRatio = 0.0f;

		bool HasFixedAspectRatio = false;
		float FixedAspectRatio = 16.0f / 9.0f;

		bool mainCamera = true;

		friend class Scene;
	};

}
