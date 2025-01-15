// #include "../external/glm/glm.hpp"
// #include "../external/glm/gtc/matrix_transform.hpp"
// #include "../external/glm/gtc/type_ptr.hpp"
#include <iostream>

// #include "../include/Math.hpp"

# define M_PI		3.14159265358979323846	/* pi */


struct vec4
{
    float x, y, z, w;

    vec4(float x, float y, float z, float w){
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    vec4(float a){
        x = y = z = w = a;
    }

    vec4(){
        *this = vec4(0);
    }

    vec4(const vec4& o) = default;

    float operator[](int index) const{
        return (&x)[index];
    }
    
    float operator[](int index){
        return (&x)[index];
    }

    vec4 operator+(const vec4& o) const{
        std::cout << "add: " << x << " " << o.x << std::endl;
        return {x + o.x, y + o.y, z + o.z, w + o.w};
    }
    vec4 operator-(const vec4& o) const{
        return {x - o.x, y - o.y, z - o.z, w - o.w};
    }
    float dot(const vec4& o){
        return x * o.x + y * o.y + z * o.z + w * o.w;
    }
    vec4 operator*(const vec4& o) const{
        return {x * o.x, y * o.y, z * o.z, w * o.w};
    }
    vec4 operator/(const vec4& o) const{
        return {x / o.x, y / o.y, z / o.z, w / o.w};
    }
    vec4 operator*(float o){
        return {x * o, y * o, z * o, w * o};
    }
    vec4 operator/(float o){
        return {x / o, y / o, z / o, w / o};
    }
    vec4 cross(const vec4& o) const{
        return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x, 0};
    }
};

struct mat4
{
    vec4 data[4] = {0};

    mat4 (float f){
        data[0] = vec4(f);
        data[1] = vec4(f);
        data[2] = vec4(f);
        data[3] = vec4(f);
    }

    mat4() = default;
    mat4(const mat4& o) = default;

    mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3) {
        std::cout << "mat constructor" << std::endl;
        std::cout << v0.x << std::endl;
        data[0] = v0;
        data[1] = v1;
        data[2] = v2;
        data[3] = v3;
    }

    vec4 operator[](int index) const
    {
        return data[index];
    }

    vec4 &operator[](int index)
    {
        return data[index];
    }

    mat4 operator+(const mat4& o)
    {
        return {data[0] + o[0], data[1] + o[1], data[2] + o[2], data[3] + o[3]};
    }
};

float radians(float degree)
{
    return degree * M_PI / 180;
}

float degrees(float radian)
{
    return radian * 180 / M_PI;
}

int main(int argc, char const *argv[])
{
    // vec4 vec = {0, 1, 2, 3};

    // std::cout << vec.x << std::endl;
    // std::cout << vec[0] << std::endl;

    mat4 mato;
    mato[0] = vec4(1, 0, 0, 0);
    mat4 mata;
    mata[0] = vec4(1, 0, 0, 0);
    mato = mato + mata;

    std::cout << mato[0].x << std::endl;
    mato[0] = vec4(1, 0, 0, 0);
    std::cout << mato[0].x << std::endl;
    vec4 v = vec4(1, 0, 0, 0);
    std::cout << v.x << std::endl;
    vec4 v2 = v;
    std::cout << v2.x << std::endl;
    vec4 v3 = v + v;
    std::cout << v3.x << std::endl;
    mat4 test;
    test[0] = v;
    std::cout << test[0].x << std::endl;
    // glm::mat4 a = glm::mat4(1.0);

    // std::cout << a[0] << std::endl;

    return 0;
}
