#include "OrthographicCamera.h"
#include <Utils/Logging.h>

namespace Nebula {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: ProjectionMatrix(Mat4f::orthographic(left, right, bottom, top, -1.0f, 1.0f)), ViewMatrix(1.0f)
	{
		ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		ProjectionMatrix = Mat4f::orthographic(left, right, bottom, top, -1.0f, 1.0f);
		ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		Mat4f transform = Mat4f(1.0f) * Mat4f::rotation(Rot * (MATH_PI/180), Vec3f(0, 0, 1)) * Mat4f::translation(Pos);
		ViewMatrix = transform.invertMatrix();
		ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
	}

}