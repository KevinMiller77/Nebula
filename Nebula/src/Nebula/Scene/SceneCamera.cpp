
#include "SceneCamera.h"

#include <Math/math.h>

namespace Nebula {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		CamProjectionType = ProjectionType::Orthographic;
		OrthographicSize = size;
		OrthographicNear = nearClip;
		OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		CamProjectionType = ProjectionType::Perspective;
		PerspectiveFOV = verticalFOV;
		PerspectiveNear = nearClip;
		PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (CamProjectionType == ProjectionType::Perspective)
		{
			ViewProjectionMatrix = Mat4f::perspective(PerspectiveFOV, AspectRatio, PerspectiveNear, PerspectiveFar);
		}
		else
		{
			float orthoLeft = -OrthographicSize * AspectRatio * 0.5f;
			float orthoRight = OrthographicSize * AspectRatio * 0.5f;
			float orthoBottom = -OrthographicSize * 0.5f;
			float orthoTop = OrthographicSize * 0.5f;

			ViewProjectionMatrix = Mat4f::orthographic(orthoLeft, orthoRight, orthoBottom, orthoTop, OrthographicNear, OrthographicFar);
		}
		
	}

}
