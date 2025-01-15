#ifndef MATH_HPP
#define MATH_HPP

struct vec4
{
    float x, y, z, w;
};

struct mat4
{
    // float data[4][4];
    float data[16];

    mat4()
    {
        for (int i = 0; i < 16; i++)
                data[i] = 0;
    }

    // mat4 operator+(const mat4& other)
    // {
    //     for (int i = 0; i < 16; i++)
    //     {
    //         this[i] += other[i];
    //     }
    // }

    float operator[](int index)
    {
        return this->data[index];
    }
};

#endif