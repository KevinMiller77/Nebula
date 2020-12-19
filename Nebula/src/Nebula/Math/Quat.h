#pragma once

#include "MathCommon.h"
#include "Vec3.h"
#include "Mat4.h"


namespace Nebula
{
    class Quat
    {
    private:
        float S = 0.0f;
        Vec3f V = Vec3f(0.0f);

    public:
        Quat() = default;
        Quat(float s, Vec3f v)
            :   S(s), V(v) {}
        Quat(const Quat& n)
            :   S(n.S), V(n.V) {}

        Quat(Vec3f n, float r)
        {
            float s = sinf(r * 0.5f);
            float c = cosf(r * 0.5f);

            V.X = n.X * s;
            V.Y = n.Y * s;
            V.Z = n.Z * s;
            S = c;

            Normalize();
        }

        Quat(Vec3f n)
        {
            Vec3f c = { cosf(n.X * 0.5f), cosf(n.Y * 0.5f), cosf(n.Z * 0.5f) };
            Vec3f s = { sinf(n.X * 0.5f), sinf(n.Y * 0.5f), sinf(n.Z * 0.5f) };

            S   = c.X * c.Y * c.Z + s.X * s.Y * s.Z;
            V.X = s.X * c.Y * c.Z - c.X * s.Y * s.Z;
            V.Y = c.X * s.Y * c.Z + s.X * c.Y * s.Z;
            V.Z = -1.0f * (c.X * c.Y * s.Z - s.X * s.Y * c.Z);

            // Vec3f vx = { 1, 0, 0}, vy = { 0, 1, 0 }, vz = { 0, 0, 1 };
            // Quat qx(vx, n.X);
            // Quat qy(vy, n.Y);
            // Quat qz(vz, n.Z);

            // Quat qt = qx * qy * qz;

            // S = qt.S;
            // V = qt.V;
        }

        Mat4f AsMat4f()
        {

            float x = V.X;
            float y = V.Y;
            float z = V.Z;
            float w = S;

            // Mat4f mat(1.0f);
            // float xx      = x * x;
            // float xy      = x * y;
            // float xz      = x * z;
            // float xw      = x * w;

            // float yy      = y * y;
            // float yz      = y * z;
            // float yw      = y * w;

            // float zz      = z * z;
            // float zw      = z * w;

            // mat[0]  = 1 - 2 * ( yy + zz );
            // mat[4]  =     2 * ( xy - zw );
            // mat[8]  =     2 * ( xz + yw );

            // mat[1]  =     2 * ( xy + zw );
            // mat[5]  = 1 - 2 * ( xx + zz );
            // mat[9]  =     2 * ( yz - xw );

            // mat[2]  =     2 * ( xz - yw );
            // mat[6]  =     2 * ( yz + xw );
            // mat[10] = 1 - 2 * ( xx + yy );

            // mat[3]  = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;
            // mat[15] = 1;

            Mat4f out(1.0f);

            Vec4f curRow(0.0f);
            curRow.X = 1.0f - 2.0f * (y * y + z * z);
            curRow.Y = 2.0f * (x * y - z * w);
            curRow.Z = 2.0f * (x * z + y * w);
            curRow.W = 0.0f;
            out[0] = curRow;

            curRow = Vec4f(0.0f);
            curRow.X = 2.0f * (x * y + z * w);
            curRow.Y = 1 - 2.0f * (x * x + z * z);
            curRow.Z = 2.0f * (y * z - x * w);
            curRow.W = 0.0f;
            out[1] = curRow;
            
            curRow = Vec4f(0.0f);
            curRow.X = 2.0f * (x * z - y * w);
            curRow.Y = 2.0f * (y * z + x * w);
            curRow.Z = 1.0f - 2.0f * (x * x + y * y);
            curRow.W = 0.0f;
            out[2] = curRow;

            curRow = { 0.0f, 0.0f, 0.0f, 1.0f };
            out[3] = curRow;

            return out;
        }

        Vec3f Rotate(Vec3f v)
        {
            Vec3f QuatVector = {V.X, V.Y, V.Z};
            Vec3f uv = QuatVector.Cross(v);
            Vec3f uuv = QuatVector.Cross(uv);

            return v + ((uv * S) + uuv) * 2.0f;
        }

        float Normal()
        { 
            float r = S * S;
            float i = V.Magnitude() * V.Magnitude();
            return sqrt(r + i); 
        }

        void Normalize()
        {
            if (Normal() == 0.0f)
                return;
            
            float n = 1.0f / Normal();
            S *= n;
            V *= n;
        }

        void UnitNormalize()
        {
            float angle = DEG_TO_RAD(S);
            V.Normalize();

            S = cosf( angle * 0.5f );
            V *= sinf(angle * 0.5f);
        }

        Quat Conjugate()
        {
            float s = S;
            Vec3f v = V * -1.0f;

            return Quat(s, v);
        }

        Quat Inverse()
        {
            float abs = Normal();
            abs *= abs;
            abs = 1.0f / abs;

            Quat conj = Conjugate();

            float s = conj.S * abs;
            Vec3f i = conj.V * abs;

            return Quat(s, i);
        }

        Vec3f Rotate(float angle, Vec3f axis)
        {
            Quat p(0, V);
            
            Vec3f a = axis.Normalize();
            Quat q(angle, a);
            q.UnitNormalize();

            Quat qInv = q.Inverse();
            Quat rotatedQuat(q * p * qInv);

            return rotatedQuat.V;
        }

        Quat operator=(const Quat& n)
            { this->S = n.S; this->V = n.V; }
        
        Quat operator*(const Quat& n)
            { return multiply(n); }
        Quat operator*(const Vec3f& n)
            { return multiply(n); }
        Quat operator*(const float n)
            { return Quat(n * S, n * V); }

        void operator+=(const Quat& n)
            { this->S += n.S; this->V += n.V; }
        void operator-=(const Quat& n)
            { this->S -= n.S; this->V -= n.V; }
        void operator*=(const Quat& n)
            { *(this) = multiply(n); }

        void operator*=(const float& n)
            { this->S * n; this->V * n; }

    private:
        Quat multiply(const Quat& n)
        {
            float s = S * n.S - V.Dot(n.V);
            Vec3f i = n.V * S + V * n.S + V.Cross(n.V);

            return Quat(s, i); 
        }

        Quat multiply(const Vec3f& n)
        {
            Quat intermidiate(0, n);
            return *this * intermidiate;
        }
    };
}