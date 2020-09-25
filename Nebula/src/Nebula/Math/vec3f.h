#ifndef __KevEngine_VEC3__
#define __KevEngine_VEC3__

//3 Float vector
struct Vec3f {
    union {
        float elements[3];
        struct {float x; float y; float z;};
    };
    
    Vec3f();
    Vec3f(float x, float y, float z);
    ~Vec3f();
    
    const float& operator[](int index) const;
    float& operator[](int index);
    
    void operator+=(const Vec3f& other);
    void operator-=(const Vec3f& other);
    void operator*=(const Vec3f& other);
    void operator/=(const Vec3f& other);

    bool operator==(const Vec3f& other);
    
    friend Vec3f operator+(Vec3f left, Vec3f right);
    friend Vec3f operator-(Vec3f left, Vec3f right);
    friend Vec3f operator*(Vec3f left, Vec3f right);
    friend Vec3f operator/(Vec3f left, Vec3f right);
    
    private:
    void add(const Vec3f& other);
    void subtract(const Vec3f& other);
    void multiply(const Vec3f& other);
    void divide(const Vec3f& other);
};

#endif