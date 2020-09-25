#ifndef __KevEngine_Vec2__
#define __KevEngine_Vec2__

//2 Float vector
struct Vec2f {
    union {
        float elements[2];
        struct {float x; float y;};
    };
    
    Vec2f();
    Vec2f(float x, float y);
    ~Vec2f();
    
    float dot(const Vec2f& other);
    static float dot(const Vec2f& vec, const Vec2f& other);

    Vec2f normal();
    static Vec2f normal(const Vec2f& vec);

    const float& operator[](int index) const;
    float& operator[](int index);
    
    void operator+=(const Vec2f& other);
    void operator-=(const Vec2f& other);
    void operator*=(const Vec2f& other);
    void operator/=(const Vec2f& other);
    
    friend Vec2f operator+(Vec2f left, Vec2f right);
    friend Vec2f operator-(Vec2f left, Vec2f right);
    friend Vec2f operator*(Vec2f left, Vec2f right);
    friend Vec2f operator/(Vec2f left, Vec2f right);
    
    private:
    void add(const Vec2f& other);
    void subtract(const Vec2f& other);
    void multiply(const Vec2f& other);
    void divide(const Vec2f& other);
};

#endif
