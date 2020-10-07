#pragma once

#include <Graphics/Camera.h>

typedef unsigned int uint32_t;

namespace Nebula {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

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

		ProjectionType GetProjectionType() const { return CamProjectionType; }
		void SetProjectionType(ProjectionType type) { CamProjectionType = type; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		ProjectionType CamProjectionType = ProjectionType::Orthographic;

		float PerspectiveFOV = 45.0f * PI2R_FACTOR;
		float PerspectiveNear = 0.01f, PerspectiveFar = 1000.0f;

		float OrthographicSize = 10.0f;
		float OrthographicNear = -1.0f, OrthographicFar = 1.0f;

		float AspectRatio = 0.0f;
	};

}
