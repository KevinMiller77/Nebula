#pragma once
#include "Vec3.h"

namespace Nebula
{
    template <typename T> 
    struct Mat3
    {
        union {
            Vec3<T> rows[3];
            T elements[3 * sizeof(Vec3<T>)];
        };
        
        Mat3()  { memset(elements, (T)0, 3 * 3 * sizeof(T)); }
        Mat3(T identity)
        {
            memset(elements, 0, 3 * 3 * sizeof(T));
            elements[0 * 3 + 0] = identity;
            elements[1 * 3 + 1] = identity;
            elements[2 * 3 + 2] = identity;
        }
        Mat3(Vec3<T> row0, Vec3<T> row1, Vec3<T> row2)
        {
            memcpy(rows[0].elements, row0.elements, 3 * sizeof(T));
            memcpy(rows[1].elements, row1.elements, 3 * sizeof(T));
            memcpy(rows[2].elements, row2.elements, 3 * sizeof(T));
        }
        ~Mat3() = default;
        
        const Vec3<T>& operator[](int index) const  { return rows[index]; }
        Vec3<T>& operator[](int index)              { return rows[index]; }
        
        void operator+=(const Mat3<T>& other)       { add(other); }
        void operator-=(const Mat3<T>& other)       { subtract(other); }
        void operator*=(const Mat3<T>& other)       { multiply(other); }
        
        friend Mat3<T> operator+(Mat3<T> left, const Mat3<T>& right)
        {
            left.add(right);
            return left;
        }
        friend Mat3<T> operator-(Mat3<T> left, const Mat3<T>& right)
        {
            left.add(right);
            return left;
        }
        friend Mat3<T> operator*(Mat3<T> left, const Mat3<T>& right)
        {
            left.add(right);
            return left;
        }
        
    private:
        void add(const Mat3<T>& other)
        {
            for (int i = 0; i < 3 * 3; i++) {
                elements[i] += other.elements[i];
            }
        }
        void subtract(const Mat3<T>& other)
        {
            for (int i = 0; i < 3 * 3; i++) {
                elements[i] -= other.elements[i];
            }
        }
        void multiply(const Mat3<T>& other)
        {
            T result[9];
    
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    //Dot-product for each element of result
                    float resultSum = 0;
                    for (int i = 0; i < 3; i++) {
                        resultSum += elements[row * 3 + i] * other.elements[i * 3 + col];
                    }
                    result[row * 3 + col] = resultSum;
                }
            }
            
            memcpy(elements, result, 3 * 3 * sizeof(T));
        }

    };

    typedef Mat3<float> Mat3f;
    typedef Mat3<int>   Mat3i;
    typedef Mat3<unsigned int>   Mat3u;
}