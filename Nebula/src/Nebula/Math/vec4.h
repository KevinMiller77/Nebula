#ifndef __KevEngine_VEC4__
#define __KevEngine_VEC4__

//4 Float vector
struct Vec4 {
    union {
        struct {float x; float y; float z; float w;};
        float elements[4];
    };
    
    Vec4();
    Vec4(float x, float y, float z, float w);
    ~Vec4();
    
    const float& operator[](int index) const;
    float& operator[](int index);
    
    void operator+=(const Vec4& other);
    void operator-=(const Vec4& other);
    void operator*=(const Vec4& other);
    void operator/=(const Vec4& other);
    
    friend Vec4 operator+(Vec4 left, Vec4 right);
    friend Vec4 operator-(Vec4 left, Vec4 right);
    friend Vec4 operator*(Vec4 left, Vec4 right);
    friend Vec4 operator/(Vec4 left, Vec4 right);
    
    private:
    void add(const Vec4& other);
    void subtract(const Vec4& other);
    void multiply(const Vec4& other);
    void divide(const Vec4& other);
};

#endif