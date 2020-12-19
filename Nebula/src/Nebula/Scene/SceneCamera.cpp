
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

	void SceneCamera::SetViewportSize(unsigned int width, unsigned int height)
	{
		AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (CamProjectionType == ProjectionType::Perspective)
		{
			m_Projection = Mat4f::perspective(PerspectiveFOV, HasFixedAspectRatio ? FixedAspectRatio : AspectRatio, PerspectiveNear, PerspectiveFar);
		}
		else
		{
			float AR = HasFixedAspectRatio ? FixedAspectRatio : AspectRatio;
			float orthoLeft = -OrthographicSize * 0.5f * AR;
			float orthoRight = OrthographicSize * 0.5f * AR;
			float orthoBottom = -OrthographicSize * 0.5f;
			float orthoTop = OrthographicSize * 0.5f;

			m_Projection = Mat4f::orthographic(orthoLeft, orthoRight, orthoBottom, orthoTop, OrthographicNear, OrthographicFar);
		}
		
	}

}
