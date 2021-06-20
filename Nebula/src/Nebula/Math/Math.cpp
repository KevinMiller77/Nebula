#include "Math.h"

namespace Nebula{

#define EPISILON_MAX 1.192092896e-07F

    static bool epsilonEqual
	(
		float const& x,
		float const& y,
		float const& epsilon
	)
	{
		return fabsf(x - y) < epsilon;
	}

    static bool epsilonNotEqual(float const& x, float const& y, float const& epsilon)
	{
		return fabsf(x - y) >= epsilon;
	}

    static Vec3f _scale(Vec3f const& v, float desiredLength)
	{
		return v * desiredLength / Vec3f::Magnitude(v);
	}

    bool DecomposeTransform(const Mat4f& transform, Vec3f& translation, Vec3f& rotation, Vec3f& scale)
    {
		// From glm::decompose in matrix_decompose.inl

		Mat4f LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], 0.0f, EPISILON_MAX))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], 0.0f, EPISILON_MAX) ||
			epsilonNotEqual(LocalMatrix[1][3], 0.0f, EPISILON_MAX) ||
			epsilonNotEqual(LocalMatrix[2][3], 0.0f, EPISILON_MAX))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = 0.0f;
			LocalMatrix[3][3] = 1.0f;
		}

		// Next take care of translation (easy).
		translation = { LocalMatrix[3].X, LocalMatrix[3].Y, LocalMatrix[3].Z };
		LocalMatrix[3] = Vec4f(0, 0, 0, LocalMatrix[3].W);

		Vec3f Row[3], Pdum3;

		// Now get scale and shear.
		for (unsigned int i = 0; i < 3; ++i)
			for (unsigned int j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.X = Vec3f::Magnitude(Row[0]);
		Row[0] = _scale(Row[0], 1.0f);
		scale.Y = Vec3f::Magnitude(Row[1]);
		Row[1] = _scale(Row[1], 1.0f);
		scale.Z = Vec3f::Magnitude(Row[2]);
		Row[2] = _scale(Row[2], 1.0f);


		rotation.Y = asin(-Row[0][2]);
		if (cosf(rotation.Y) != 0) {
			rotation.X = atan2(Row[1][2], Row[2][2]);
			rotation.Z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.X = atan2(-Row[2][0], Row[1][1]);
			rotation.Z = 0;
		}


		return true;
	}
}