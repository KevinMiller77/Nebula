#pragma once

#include "MathCommon.h"
#include "Vec3.h"
#include "Mat4.h"


namespace Nebula{
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

            // Quat result = Quat({1, 0, 0}, n.X) * Quat({0, 1, 0}, n.Y) * Quat({0, 0, 1}, n.Z);
            // S = result.S;
            // V = result.V;

            Normalize();
        }

        Vec3f GetForwardVec() {
            return Rotate({0.0f, 0.0f, -1.0f});
        }
        Vec3f GetUpVec() {
            return Rotate({0.0f, 1.0f, 0.0f});
        }
        Vec3f GetRightVec() {
            return Rotate({1.0f, 0.0f, -1.0f});
        }


        float GetS() const {
            return S;
        }

        Vec3f GetV() const {
            return V;
        }

        Vec3f GetEulerAngles() {
            return V;
        }

        Mat4f AsMat4f()
        {

            Mat4f Result(1.0f);
            float qxx(V.X * V.X);
            float qyy(V.Y * V.Y);
            float qzz(V.Z * V.Z);
            float qxz(V.X * V.Z);
            float qxy(V.X * V.Y);
            float qyz(V.Y * V.Z);
            float qwx(S * V.X);
            float qwy(S * V.Y);
            float qwz(S * V.Z);

            Result.elements[0 + 0 * 4] = 1.0f - 2.0f * (qyy +  qzz);
            Result.elements[0 + 1 * 4] = 2.0f * (qxy + qwz);
            Result.elements[0 + 2 * 4] = 2.0f * (qxz - qwy);
            Result.elements[0 + 3 * 4] = 0.0f;

            Result.elements[1 + 0 * 4] = 2.0f * (qxy - qwz);
            Result.elements[1 + 1 * 4] = 1.0f - 2.0f * (qxx + qzz);
            Result.elements[1 + 2 * 4] = 2.0f * (qyz + qwx);
            Result.elements[1 + 3 * 4] = 0.0f;

            Result.elements[2 + 0 * 4] = 2.0f * (qxz + qwy);
            Result.elements[2 + 1 * 4] = 2.0f * (qyz - qwx);
            Result.elements[2 + 2 * 4] = 1.0f - 2.0f * (qxx + qyy);
            Result.elements[2 + 3 * 4] = 0.0f;

            Result.elements[3 + 0 * 4] = 0.0f;
            Result.elements[3 + 1 * 4] = 0.0f;
            Result.elements[3 + 2 * 4] = 0.0f;
            Result.elements[3 + 3 * 4] = 1.0f;

            return Result;
        }

        Vec3f Rotate(Vec3f v)
        {

            Vec3f QuatVector = GetV();
            Vec3f uv(QuatVector.Cross(v));
            Vec3f uuv(QuatVector.Cross(uv));

            return v + ((uv * S) + uuv) * 2.0f;
        }

        Vec3f Forward() {
            Vec3f out;
            out.X = 2.0f * (V.X * V.Z + S * V.Y);
            out.Y = 2.0f * (V.Y * V.Z - S * V.X);
            out.Z = 1.0f - 2.0f * (V.X * V.X + V.Y * V.Y);

            return out.Normalize();
        }

        float Normal()
        { 
            double r = S * S;
            double i = V.Magnitude() * V.Magnitude();
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
            { this->S = n.S; this->V = n.V; return *this; }
        
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