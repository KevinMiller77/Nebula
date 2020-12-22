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
            V.Z = c.X * c.Y * s.Z - s.X * s.Y * c.Z;
        }

        Mat4f AsMat4f()
        {

            Mat4f Result(1.0f);
            float qxx(V.X * S);
            float qyy(V.Y * V.Y);
            float qzz(V.Z * V.Z);
            float qxz(V.X * V.Z);
            float qxy(V.X * V.Y);
            float qyz(V.Y * V.Z);
            float qwx(S * V.X);
            float qwy(S * V.Y);
            float qwz(S * V.Z);

            Result[0][0] = 1.0f - 2.0f * (qyy +  qzz);
            Result[1][0] = 2.0f * (qxy + qwz);
            Result[2][0] = 2.0f * (qxz - qwy);
            Result[3][0] = 0.0f;


            Result[0][1] = 2.0f * (qxy - qwz);
            Result[1][1] = 1.0f - 2.0f * (qxx +  qzz);
            Result[2][1] = 2.0f * (qyz + qwx);
            Result[3][1] = 0.0f;

            Result[0][2] = 2.0f * (qxz + qwy);
            Result[1][2] = 2.0f * (qyz - qwx);
            Result[2][2] = 1.0f - 2.0f * (qxx +  qyy);
            Result[3][2] = 0.0f;

            Result[0][3] = 0.0f;
            Result[1][3] = 0.0f;
            Result[2][3] = 0.0f;
            Result[3][3] = 1.0f;

            return Result;
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