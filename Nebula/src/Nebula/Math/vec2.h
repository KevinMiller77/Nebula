#pragma once
#include "MathCommon.h"

namespace Nebula
{
    template <typename T>
    struct Vec2
    {
        union {
            T elements[2];
            struct {T X; T Y;};
        };
        
        Vec2()          : X((T)0),  Y((T)0) {}
        Vec2(T x, T y)  : X(x),     Y(y) {}
        Vec2(T all)     : X(all),     Y(all) {}
        ~Vec2()         = default;
        
        const T& operator[](int index)  const { return elements[index]; }
        T& operator[](int index)        { return elements[index]; }

        float Dot(const Vec2<T>& other)                                     { return (float)(X * other.X + Y * other.Y); }
        static float Dot(const Vec2<T>& left, const Vec2<T>& right)         { return (float)(left.X * right.X + left.Y * right.Y); }

        float Magnitude()                                                   { return (float)sqrt((float)(X * X + Y * Y)); }
        static float Magnitude(const Vec2& in)                              { return (float)sqrt((float)(in.X * in.X + in.Y * in.Y)); }

        //  Return vlaue is in radians
        float AngleBetween(const Vec2<T>& other)
        { 
            float dot = Dot(other);
            float mag = Magnitude() * Magnitude(other);
            assert(mag != 0.0f);
            return (float)acos(dot / mag); 
        }
        //  Return vlaue is in radians
        static float AngleBetween(const Vec2& left, const Vec2& right)      { return left.AngleBetween(other); }
        float AngleBetweenDeg(const Vec2<T>& other)                         { return RAD_TO_DEG(AngleBetween(other)); }
        static float AngleBetweenDeg(const Vec2& left, const Vec2& right)   { return RAD_TO_DEG(AngleBetween(left, right)); }

        void operator+=(const Vec2& other)  { add(other); }
        void operator-=(const Vec2& other)  { subtract(other); }
        void operator*=(const Vec2& other)  { multiply(other); }
        void operator/=(const Vec2& other)  { divide(other); }
        
        friend Vec2<T> operator+(Vec2<T> left, Vec2<T> right)   { return Vec2(<T>left.X + right.X, left.Y + right.Y); }
        friend Vec2<T> operator-(Vec2<T> left, Vec2<T> right)   { return Vec2(<T>left.X - right.X, left.Y - right.Y); }
        friend Vec2<T> operator*(Vec2<T> left, Vec2<T> right)   { return Vec2(<T>left.X * right.X, left.Y * right.Y); }
        friend Vec2<T> operator/(Vec2<T> left, Vec2<T> right)   { assert(right.X != (T)(0) && right.Y != (T)(0));   // Divide by 0
                                                                     return Vec2<T>(left.X / right.X, left.Y / right.Y); }

        private:
            void add(const Vec2<T>& other)
            {
                X += other.X;
                Y += other.Y;
            }
            void subtract(const Vec2<T>& other)
            {
                X -= other.X;
                Y -= other.Y;
            }
            void multiply(const Vec2<T>& other)
            {
                X *= other.X;
                Y *= other.Y;
            }
            void divide(const Vec2<T>& other)
            {
                // Divide by 0
                assert(other.Y != (T)(0) && other.X != (T)(0));
                X /= other.X;
                Y /= other.Y;
            }
    };

    typedef Vec2<float> Vec2f;
    typedef Vec2<int>   Vec2i;
    typedef Vec2<unsigned int>   Vec2u;

}