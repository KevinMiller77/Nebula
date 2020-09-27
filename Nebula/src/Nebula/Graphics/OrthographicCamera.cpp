#include "OrthographicCamera.h"
#include <Utils/Logging.h>

namespace Nebula {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: ProjectionMatrix(Mat4f::orthographic(left, right, bottom, top, -10.0f, 10.0f)), ViewMatrix(1.0f)
	{
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		ProjectionMatrix = Mat4f::orthographic(left, right, bottom, top, -10.0f, 10.0f);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		Mat4f transform = Mat4f::translation(Pos);
		Mat4f rot = Mat4f::rotation(Rot * (PI/180), Vec3f(0, 0, 1));
		Mat4f sub = transform * rot;

		ViewMatrix = sub.invertMatrix();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}

}