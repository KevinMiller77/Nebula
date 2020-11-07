#pragma once
#include <memory.h>
#include "Vec3.h"
#include "Vec4.h"

namespace Nebula
{
    template <typename T> 
    struct Mat4
    {
        union {
            T elements[4 * sizeof(Vec4<T>)];
            Vec4<T> rows[4];
        };
        
        Mat4()  { memset(elements, 0, 4 * 4 * sizeof(T)); }
        Mat4(T identity)
        {
            memset(elements, 0, 4 * 4 * sizeof(T));
            elements[0 * 4 + 0] = identity;
            elements[1 * 4 + 1] = identity;
            elements[2 * 4 + 2] = identity;
            elements[3 * 4 + 3] = identity;
        }
        Mat4(Vec4<T> row0, Vec4<T> row1, Vec4<T> row2, Vec4<T> row3)
        {
            memcpy(rows[0].elements, row0.elements, 4 * sizeof(T));
            memcpy(rows[1].elements, row1.elements, 4 * sizeof(T));
            memcpy(rows[2].elements, row2.elements, 4 * sizeof(T));
            memcpy(rows[3].elements, row3.elements, 4 * sizeof(T));
        }
        ~Mat4() = default;

        static Mat4 orthographic(float left, float right, float top, float bottom, float near, float far)
        {
            Mat4<float> result(1.0f);

            result.elements[0 + 0 * 4] = 2.0f / (right - left);

            result.elements[1 + 1 * 4] = 2.0f / (top - bottom);

            result.elements[2 + 2 * 4] = -2.0f / (near - far);

            result.elements[0 + 3 * 4] = -(left + right) / (left - right);
            result.elements[1 + 3 * 4] = -(bottom + top) / (bottom - top);
            result.elements[2 + 3 * 4] = -(far + near) / (far - near);

            return result;
        }
        static Mat4 perspective(float fov, float aspectRatio, float near, float far)
        {
            Mat4<float> result(1.0f);

            float q = (float) (1.0f / tan(0.5f * fov * (3.14123f / 180.0f)));
            float a = (float) (q / aspectRatio);

            float b = (near + far) / (near - far);
            float c = (2.0f * near * far) / (near - far);

            result.elements[0 + 0 * 4] = a;
            result.elements[1 + 1 * 4] = q;
            result.elements[2 + 2 * 4] = b;
            result.elements[3 + 2 * 4] = -1.0f;
            result.elements[2 + 3 * 4] = c;

            return result;
        }

        static Mat4 translation(const Vec3<T>& translation)
        {
            Mat4<float> result(1.0f);

            result.elements[0 + 3 * 4] = translation.X;
            result.elements[1 + 3 * 4] = translation.Y;
            result.elements[2 + 3 * 4] = translation.Z;

            return result;
        }
        static Mat4 rotation(float angle, const Vec3<T>& axis)
        {
            Mat4<float> result(1.0f);

            float r = angle;
            float c = (float) (cos(r));
            float s = (float) (sin(r));
            float omc = 1.0f - c;
            
            float x = axis.X;
            float y = axis.Y;
            float z = axis.Z;

            result.elements[0 + 0 * 4] = x * omc + c;
            result.elements[1 + 0 * 4] = y * x * omc + z * s;
            result.elements[2 + 0 * 4] = x * z * omc - y * s;

            result.elements[0 + 1 * 4] = x * y * omc - z * s;
            result.elements[1 + 1 * 4] = y * omc + c;
            result.elements[2 + 1 * 4] = y * z * omc + x * s;

            result.elements[0 + 2 * 4] = x * z * omc + y * s;
            result.elements[1 + 2 * 4] = y * z * omc - x * s;
            result.elements[2 + 2 * 4] = z * omc + c;
            
            return result;
        }
        
        static Mat4 scale(const Vec3<T>& scale)
        {
            Mat4<float> result(1.0f);

            result.elements[0 + 0 * 4] = scale.X;
            result.elements[1 + 1 * 4] = scale.Y;
            result.elements[2 + 2 * 4] = scale.Z;

            return result;
        }
        
        const Vec4<T>& operator[](int index) const  { return rows[index]; }
        Vec4<T>& operator[](int index)              { return rows[index]; }
        
        void operator+=(const Mat4<T>& other)       { add(other); }
        void operator-=(const Mat4<T>& other)       { subtract(other); }
        void operator*=(const Mat4<T>& other)       { multiply(other); }
        
        friend Mat4<T> operator+(Mat4<T> left, const Mat4<T>& right)
        {
            left.add(right);
            return left;
        }
        friend Mat4<T> operator-(Mat4<T> left, const Mat4<T>& right)
        {
            left.subtract(right);
            return left;
        }
        friend Mat4<T> operator*(Mat4<T> left, const Mat4<T>& right)
        {
            left.multiply(right);
            return left;
        }
        friend Vec4<T> operator*(Mat4<T> left, const Vec4<T> &right)
        {
            return left.multiply(right);
        }

        friend Mat4<T> operator*(Mat4<T> left, const T &right)
        {
            Mat4<T> out;
            for (int i = 0; i < 16; i++)
            {
                out.elements[i] = left.elements[i] * right;
            }
            return out;
        }

        Vec4<T> Mat4<T>::multiply(const Vec4<T> &other)
        {
            return Vec4<T>(
                rows[0].X * other.X + rows[1].X * other.Y + rows[2].X * other.Z + rows[3].X * other.W,
                rows[0].Y * other.X + rows[1].Y * other.Y + rows[2].Y * other.Z + rows[3].Y * other.W,
                rows[0].Z * other.X + rows[1].Z * other.Y + rows[2].Z * other.Z + rows[3].Z * other.W,
                rows[0].W * other.X + rows[1].W * other.Y + rows[2].W * other.Z + rows[3].W * other.W
            );
        }
        
        Mat4 invertMatrix()
        {
            Mat4<float> newMat = Mat4<float>(1.0f);
            float det = 0;
            int i;

            newMat.elements[0] = elements[5]  * elements[10] * elements[15] - 
                    elements[5]  * elements[11] * elements[14] - 
                    elements[9]  * elements[6]  * elements[15] + 
                    elements[9]  * elements[7]  * elements[14] +
                    elements[13] * elements[6]  * elements[11] - 
                    elements[13] * elements[7]  * elements[10];

            newMat.elements[4] = -elements[4]  * elements[10] * elements[15] + 
                    elements[4]  * elements[11] * elements[14] + 
                    elements[8]  * elements[6]  * elements[15] - 
                    elements[8]  * elements[7]  * elements[14] - 
                    elements[12] * elements[6]  * elements[11] + 
                    elements[12] * elements[7]  * elements[10];

            newMat.elements[8] = elements[4]  * elements[9] * elements[15] - 
                    elements[4]  * elements[11] * elements[13] - 
                    elements[8]  * elements[5] * elements[15] + 
                    elements[8]  * elements[7] * elements[13] + 
                    elements[12] * elements[5] * elements[11] - 
                    elements[12] * elements[7] * elements[9];

            newMat.elements[12] = -elements[4]  * elements[9] * elements[14] + 
                    elements[4]  * elements[10] * elements[13] +
                    elements[8]  * elements[5] * elements[14] - 
                    elements[8]  * elements[6] * elements[13] - 
                    elements[12] * elements[5] * elements[10] + 
                    elements[12] * elements[6] * elements[9];

            newMat.elements[1] = -elements[1]  * elements[10] * elements[15] + 
                    elements[1]  * elements[11] * elements[14] + 
                    elements[9]  * elements[2] * elements[15] - 
                    elements[9]  * elements[3] * elements[14] - 
                    elements[13] * elements[2] * elements[11] + 
                    elements[13] * elements[3] * elements[10];

            newMat.elements[5] = elements[0]  * elements[10] * elements[15] - 
                    elements[0]  * elements[11] * elements[14] - 
                    elements[8]  * elements[2] * elements[15] + 
                    elements[8]  * elements[3] * elements[14] + 
                    elements[12] * elements[2] * elements[11] - 
                    elements[12] * elements[3] * elements[10];

            newMat.elements[9] = -elements[0]  * elements[9] * elements[15] + 
                    elements[0]  * elements[11] * elements[13] + 
                    elements[8]  * elements[1] * elements[15] - 
                    elements[8]  * elements[3] * elements[13] - 
                    elements[12] * elements[1] * elements[11] + 
                    elements[12] * elements[3] * elements[9];

            newMat.elements[13] = elements[0]  * elements[9] * elements[14] - 
                    elements[0]  * elements[10] * elements[13] - 
                    elements[8]  * elements[1] * elements[14] + 
                    elements[8]  * elements[2] * elements[13] + 
                    elements[12] * elements[1] * elements[10] - 
                    elements[12] * elements[2] * elements[9];

            newMat.elements[2] = elements[1]  * elements[6] * elements[15] - 
                    elements[1]  * elements[7] * elements[14] - 
                    elements[5]  * elements[2] * elements[15] + 
                    elements[5]  * elements[3] * elements[14] + 
                    elements[13] * elements[2] * elements[7] - 
                    elements[13] * elements[3] * elements[6];

            newMat.elements[6] = -elements[0]  * elements[6] * elements[15] + 
                    elements[0]  * elements[7] * elements[14] + 
                    elements[4]  * elements[2] * elements[15] - 
                    elements[4]  * elements[3] * elements[14] - 
                    elements[12] * elements[2] * elements[7] + 
                    elements[12] * elements[3] * elements[6];

            newMat.elements[10] = elements[0]  * elements[5] * elements[15] - 
                    elements[0]  * elements[7] * elements[13] - 
                    elements[4]  * elements[1] * elements[15] + 
                    elements[4]  * elements[3] * elements[13] + 
                    elements[12] * elements[1] * elements[7] - 
                    elements[12] * elements[3] * elements[5];

            newMat.elements[14] = -elements[0]  * elements[5] * elements[14] + 
                    elements[0]  * elements[6] * elements[13] + 
                    elements[4]  * elements[1] * elements[14] - 
                    elements[4]  * elements[2] * elements[13] - 
                    elements[12] * elements[1] * elements[6] + 
                    elements[12] * elements[2] * elements[5];

            newMat.elements[3] = -elements[1] * elements[6] * elements[11] + 
                    elements[1] * elements[7] * elements[10] + 
                    elements[5] * elements[2] * elements[11] - 
                    elements[5] * elements[3] * elements[10] - 
                    elements[9] * elements[2] * elements[7] + 
                    elements[9] * elements[3] * elements[6];

            newMat.elements[7] = elements[0] * elements[6] * elements[11] - 
                    elements[0] * elements[7] * elements[10] - 
                    elements[4] * elements[2] * elements[11] + 
                    elements[4] * elements[3] * elements[10] + 
                    elements[8] * elements[2] * elements[7] - 
                    elements[8] * elements[3] * elements[6];

            newMat.elements[11] = -elements[0] * elements[5] * elements[11] + 
                    elements[0] * elements[7] * elements[9] + 
                    elements[4] * elements[1] * elements[11] - 
                    elements[4] * elements[3] * elements[9] - 
                    elements[8] * elements[1] * elements[7] + 
                    elements[8] * elements[3] * elements[5];

            newMat.elements[15] = elements[0] * elements[5] * elements[10] - 
                    elements[0] * elements[6] * elements[9] - 
                    elements[4] * elements[1] * elements[10] + 
                    elements[4] * elements[2] * elements[9] + 
                    elements[8] * elements[1] * elements[6] - 
                    elements[8] * elements[2] * elements[5];

            det = elements[0] * newMat.elements[0] + elements[1] * newMat.elements[4] + elements[2] * newMat.elements[8] + elements[3] * newMat.elements[12];
            if (det == 0)
                return Mat4<float>(rows[0], rows[1], rows[2], rows[3]);
            
            det = 1.0f / det;
            
            Vec4<float> newRows[4];
            for (i = 0; i < 16; i+=4)
                newRows[i / 4] = Vec4<float>(newMat.elements[i] * det, newMat.elements[i + 1] * det, newMat.elements[i + 2] * det, newMat.elements[i + 3] * det);
            return Mat4<float>(newRows[0], newRows[1], newRows[2], newRows[3]);
        }
    private:
        void add(const Mat4<T>& other)
        {
            for (int i = 0; i < 4 * 4; i++) {
                elements[i] += other.elements[i];
            }
        }
        void subtract(const Mat4<T>& other)
        {
            for (int i = 0; i < 4 * 4; i++) {
                elements[i] -= other.elements[i];
            }
        }
        void multiply(const Mat4<T>& other)
        {
            T result[16];
    
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    //Dot-product for each element of result
                    float resultSum = 0;
                    for (int i = 0; i < 4; i++) {
                        resultSum += elements[row * 4 + i] * other.elements[i * 4 + col];
                    }
                    result[row * 4 + col] = resultSum;
                }
            }
            
            memcpy(elements, result, 4 * 4 * sizeof(T));
        }


    };

    typedef Mat4<float> Mat4f;
    typedef Mat4<int>   Mat4i;
    typedef Mat4<unsigned int>   Mat4u;
}