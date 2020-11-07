#pragma once
#include "MathCommon.h"

namespace Nebula
{
    template <typename T>
    struct Vec4
    {
        union {
            T elements[4];
            struct { T X; T Y; T Z; T W; };
        };
        
        Vec4()                      : X((T)0),  Y((T)0),  Z((T)0),  W((T)0) {}
        Vec4(T x, T y, T z, T w)    : X(x),     Y(y),     Z(z),     W(w){}
        Vec4(T all)                 : X(all),     Y(all),     Z(all),     W(all){}
        ~Vec4()                 = default;
        
        const T& operator[](int index)  const { return elements[index]; }
        T& operator[](int index)        { return elements[index]; }

        float Dot(const Vec4<T>& other)                                     { return (float)(X * other.X + Y * other.Y + Z * other.Z + W * other.W); }
        static float Dot(const Vec4<T>& left, const Vec4<T>& right)         { return (float)(left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W); }

        float Magnitude()                                                   { return (float)sqrt((float)(X * X + Y * Y + Z * Z + W * W)); }
        static float Magnitude(const Vec4& in)                              { return (float)sqrt((float)(in.X * in.X + in.Y * in.Y + in.Z * in.Z + in.W * in.W)); }

        //  Return vlaue is in radians
        float AngleBetween(const Vec4<T>& other)
        { 
            float dot = Dot(other);
            float mag = Magnitude() * Magnitude(other);
            assert(mag != 0.0f);
            return (float)acos(dot / mag); 
        }
        //  Return vlaue is in radians
        static float AngleBetween(const Vec4& left, const Vec4& right)      { return left.AngleBetween(other); }
        float AngleBetweenDeg(const Vec4<T>& other)                         { return RAD_TO_DEG(AngleBetween(other)); }
        static float AngleBetweenDeg(const Vec4& left, const Vec4& right)   { return RAD_TO_DEG(AngleBetween(left, right)); }

        Vec4<T> Normalize()    
        { 
            float mag = Magnitude();
            assert(mag != 0); 
            X /= mag;
            Y /= mag;
            Z /= mag;
            W /= mag;
            return Vec4<T>(X, Y ,Z, W);
        }
        static Vec4 Normalize(Vec4 in)
        {
            return in.Normalize();
        }

        void operator+=(const Vec4& other)  { add(other); }
        void operator-=(const Vec4& other)  { subtract(other); }
        void operator*=(const Vec4& other)  { multiply(other); }
        void operator/=(const Vec4& other)  { divide(other); }
        
        friend Vec4<T> operator+(Vec4<T> left, Vec4<T> right)   { return Vec4(<T>left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W); }
        friend Vec4<T> operator-(Vec4<T> left, Vec4<T> right)   { return Vec4(<T>left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W); }
        friend Vec4<T> operator*(Vec4<T> left, Vec4<T> right)   { return Vec4(<T>left.X * right.X, left.Y * right.Y, left.Z * right.Z, left.W * right.W); }
        friend Vec4<T> operator/(Vec4<T> left, Vec4<T> right)   { assert((right.X != (T)(0) && right.Y != (T)(0)) && (right.Z != (T)(0) && right.W != (T)(0)));   // Divide by 0
                                                                     return Vec4<T>(left.X / right.X, left.Y / right.Y, left.Z / right.Z, left.W / right.W); }

        private:
            void add(const Vec4<T>& other)
            {
                X += other.X;
                Y += other.Y;
                Z += other.Z;
                W += other.W;
            }
            void subtract(const Vec4<T>& other)
            {
                X -= other.X;
                Y -= other.Y;
                Z -= other.Z;
                W -= other.W;
            }
            void multiply(const Vec4<T>& other)
            {
                X *= other.X;
                Y *= other.Y;
                Z *= other.Z;
                W *= other.W;
            }
            void divide(const Vec4<T>& other)
            {
                // Divide by 0
                assert((right.X != (T)(0) && right.Y != (T)(0)) && (right.Z != (T)(0) && right.W != (T)(0)));
                X /= other.X;
                Y /= other.Y;
                Z /= other.Z;
                W /= other.W;
            }
    };

    typedef Vec4<float> Vec4f;
    typedef Vec4<int>   Vec4i;
    typedef Vec4<unsigned int>   Vec4u;

}