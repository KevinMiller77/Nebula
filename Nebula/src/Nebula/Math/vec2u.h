#ifndef __KevEngine_Vec2_U__
#define __KevEngine_Vec2_U__

//2 unsigned int vector
struct Vec2u {
    union {
        unsigned int elements[2];
        struct {unsigned int x; unsigned int y;};
    };
    
    Vec2u();
    Vec2u(unsigned int x, unsigned int y);
    ~Vec2u();
    
    const unsigned int& operator[](int index) const;
    unsigned int& operator[](int index);
    
    void operator+=(const Vec2u& other);
    void operator-=(const Vec2u& other);
    void operator*=(const Vec2u& other);
    void operator/=(const Vec2u& other);
    void operator=(const Vec2u& other);
    
    friend Vec2u operator+(Vec2u left, Vec2u right);
    friend Vec2u operator-(Vec2u left, Vec2u right);
    friend Vec2u operator*(Vec2u left, Vec2u right);
    friend Vec2u operator/(Vec2u left, Vec2u right);
    
    private:
    void add(const Vec2u& other);
    void subtract(const Vec2u& other);
    void multiply(const Vec2u& other);
    void divide(const Vec2u& other);
};

#endif