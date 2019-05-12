#ifndef MILK_VECTOR2_H
#define MILK_VECTOR2_H

#include <cmath>

#include "Matrix3.h"

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

        Vector2& operator*=(float scalar);
        Vector2& operator/=(float scalar);
        Vector2& operator+=(Vector2 v);

        float magnitude();
        Vector2 normalize();

        static Vector2 transform(const Vector2& v, const Matrix3& m);
		static const Vector2 zero();
    };

    // Global operator overloads
    inline bool operator==(const Vector2& v1, const Vector2& v2)
    {
        return v1.x == v2.x && v1.y == v2.y;
    }

    inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
    {
        return Vector2{v1.x + v2.x, v1.y + v2.y};
    }

    inline Vector2 operator*(const Vector2& v, float s)
    {
        return Vector2{v.x * s, v.y * s};
    }

    inline Vector2 operator/(const Vector2& v, float s)
    {
        return Vector2{v.x / s, v.y / s};
    }

    // Member operator overloads
    inline Vector2& Vector2::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;

        return (*this);
    }

    inline Vector2& Vector2::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;

        return (*this);
    }

    inline Vector2& Vector2::operator+=(Vector2 v)
    {
        x += v.x;
        y += v.y;

        return (*this);
    }

    // Member methods
    inline float Vector2::magnitude()
    {
        return std::sqrt(x * x + y * y);
    }

    inline Vector2 Vector2::normalize()
    {
        return *this / magnitude();
    }

	// Static methods
    inline Vector2 Vector2::transform(const Vector2& v, const Matrix3& m)
    {
        return Vector2{(v.x * m.m11) + (v.y * m.m12) + m.m13,
                       (v.x * m.m21) + (v.y * m.m22) + m.m23};
    }
	
	inline const Vector2 Vector2::zero()
	{
		return Vector2{0.f, 0.f};
	}
}

#endif
