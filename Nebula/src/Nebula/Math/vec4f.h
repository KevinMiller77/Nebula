#ifndef __KevEngine_VEC4__
#define __KevEngine_VEC4__

//4 Float vector
struct Vec4f {
    union {
        float elements[4];
        struct {float x; float y; float z; float w;};
    };
    
    Vec4f();
    Vec4f(float x, float y, float z, float w);
    ~Vec4f();
    
    const float& operator[](int index) const;
    float& operator[](int index);
    
    inline unsigned int ColorAsInt() 
    { 
        unsigned int c;
        unsigned int r = (int)(x * 255.0f);
        unsigned int g = (int)(y * 255.0f);
        unsigned int b = (int)(z * 255.0f);
        unsigned int a = (int)(w * 255.0f);
        
        c = a << 24 | b << 16 | g << 8 | r;
        return c;
    }

    void operator+=(const Vec4f& other);
    void operator-=(const Vec4f& other);
    void operator*=(const Vec4f& other);
    void operator/=(const Vec4f& other);
    
    friend Vec4f operator+(Vec4f left, Vec4f right);
    friend Vec4f operator-(Vec4f left, Vec4f right);
    friend Vec4f operator*(Vec4f left, Vec4f right);
    friend Vec4f operator/(Vec4f left, Vec4f right);
    
    private:
    void add(const Vec4f& other);
    void subtract(const Vec4f& other);
    void multiply(const Vec4f& other);
    void divide(const Vec4f& other);
};

#endif