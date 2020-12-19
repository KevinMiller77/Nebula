#pragma once
#include "MathCommon.h"

namespace Nebula
{
    template <typename T>
    struct Vec3
    {
        union {
            T elements[3];
            struct { T X; T Y; T Z; };
        };
        
        Vec3()                  : X((T)0),  Y((T)0),  Z((T)0) {}
        Vec3(T x, T y, T z)     : X(x),     Y(y),    Z(z) {}
        Vec3(T all)              : X(all),     Y(all),     Z(all) {}
        ~Vec3()                 = default;
        
        const T& operator[](int index)  const { return elements[index]; }
        T& operator[](int index)        { return elements[index]; }

        float Dot(const Vec3<T>& other)                                     { return (float)(X * other.X + Y * other.Y + Z * other.Z); }
        static float Dot(const Vec3<T>& left, const Vec3<T>& right)         { return (float)(left.X * right.X + left.Y * right.Y + left.Z * right.Z); }

        float Magnitude()                                                   { return (float)sqrt((float)(X * X + Y * Y + Z * Z)); }
        static float Magnitude(const Vec3& in)                              { return (float)sqrt((float)(in.X * in.X + in.Y * in.Y + in.Z * in.Z)); }

        bool IsNonzero()
        {
            bool one = X == 0;
            bool too = Y == 0;
            bool fre = Z == 0;

            return !(one && too && fre);
        }

        //  Return vlaue is in radians
        float AngleBetween(const Vec3<T>& other)
        { 
            float dot = Dot(other);
            float mag = Magnitude() * Magnitude(other);
            assert(mag != 0.0f);
            return (float)acos(dot / mag); 
        }
        //  Return vlaue is in radians
        static float AngleBetween(const Vec3& left, const Vec3& right)      { return left.AngleBetween(right); }
        float AngleBetweenDeg(const Vec3<T>& other)                         { return RAD_TO_DEG(AngleBetween(other)); }
        static float AngleBetweenDeg(const Vec3& left, const Vec3& right)   { return RAD_TO_DEG(AngleBetween(left, right)); }

        Vec3<T> Cross(const Vec3<T> other)
        {
            Vec3<T> result = Vec3<T>(0);
            result.X = Y * other.Z + Z * other.Y;
            result.Y = X * other.Z + Z * other.X;
            result.Z = X * other.Y + Y * other.X;
            return result;
        }
        static Vec3 Cross(const Vec3& left, const Vec3& right)              { return left.Cross(right); }

        Vec3<T> Normalize()    
        { 
            float mag = Magnitude();
            assert(mag != 0); 
            X /= mag;
            Y /= mag;
            Z /= mag;
            return Vec3<T>(X, Y ,Z);
        }
        static Vec3 Normalize(Vec3 in)
        {
            return in.Normalize();
        }

        void operator+=(const Vec3& other)  { add(other); }
        void operator-=(const Vec3& other)  { subtract(other); }
        void operator*=(const Vec3& other)  { multiply(other); }
        void operator/=(const Vec3& other)  { divide(other); }
        
        friend Vec3<T> operator+(Vec3<T> left, Vec3<T> right)   { return Vec3<T>(left.X + right.X, left.Y + right.Y, left.Z + right.Z); }
        friend Vec3<T> operator-(Vec3<T> left, Vec3<T> right)   { return Vec3<T>(left.X - right.X, left.Y - right.Y, left.Z - right.Z); }
        friend Vec3<T> operator*(Vec3<T> left, Vec3<T> right)   { return Vec3<T>(left.X * right.X, left.Y * right.Y, left.Z * right.Z); }
        friend Vec3<T> operator/(Vec3<T> left, Vec3<T> right)   { assert((right.X != (T)(0) && right.Y != (T)(0)) && right.Z != (T)(0));   // Divide by 0
                                                                     return Vec3<T>(left.X / right.X, left.Y / right.Y, left.Z / right.Z); }

        private:
            void add(const Vec3<T>& other)
            {
                X += other.X;
                Y += other.Y;
                Z += other.Z;
            }
            void subtract(const Vec3<T>& other)
            {
                X -= other.X;
                Y -= other.Y;
                Z -= other.Z;
            }
            void multiply(const Vec3<T>& other)
            {
                X *= other.X;
                Y *= other.Y;
                Z *= other.Z;
            }
            void divide(const Vec3<T>& other)
            {
                // Divide by 0
                assert((other.Y != (T)(0) && other.X != (T)(0)) && other.Z != (T)(0));
                X /= other.X;
                Y /= other.Y;
                Z /= other.Z;
            }
    };
    typedef Vec3<float> Vec3f;
    typedef Vec3<int>   Vec3i;
    typedef Vec3<unsigned int>   Vec3u;
}