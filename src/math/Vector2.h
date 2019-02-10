#ifndef MILK_VECTOR2_H
#define MILK_VECTOR2_H

#include <cmath>

namespace milk
{
    struct Vector2
    {
        float x;
        float y;

        Vector2()
                : x(0),
                  y(0)
        {
        }

        Vector2(float a, float b)
        {
            x = a;
            y = b;
        }

        Vector2& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;

            return (*this);
        }

        Vector2& operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;

            return (*this);
        }

        Vector2& operator+=(Vector2 v)
        {
            x += v.x;
            y += v.y;

            return (*this);
        }

        bool operator==(const Vector2& v2)
        {
            return x == v2.x && y == v2.y;
        }

        Vector2 operator+(const Vector2& v2)
        {
            return {x + v2.x, y + v2.y};
        }

        Vector2 operator*(float s)
        {
            return {x * s, y * s};
        }

        Vector2 operator/(float s)
        {
            return {x / s, y / s};
        }

        float magnitude()
        {
            return std::sqrt(x * x + y * y);
        }

        Vector2 normalize()
        {
            return *this / magnitude();
        }
    };
}

#endif
