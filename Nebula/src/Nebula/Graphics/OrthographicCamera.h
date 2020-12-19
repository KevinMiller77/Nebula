#pragma once

#include <Math/math.h>
#include "Camera.h"
#include <Utils/Logging.h>

namespace Nebula {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const Vec3f& GetPosition() const { return Pos; }
		void SetPosition(const Vec3f& position) { Pos = position; RecalculateViewMatrix(); }

		float GetRotation() const { return Rot; }
		void SetRotation(float rotation) { Rot = rotation; RecalculateViewMatrix(); }

		const Mat4f& GetViewMatrix() const override { return ViewMatrix; }
		const Mat4f GetViewProjection() const override { return GetProjection() * ViewMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		Mat4f ViewMatrix;

		Vec3f Pos = { 0.0f, 0.0f, 0.0f };
		float Rot = 0.0f;
	};

}
