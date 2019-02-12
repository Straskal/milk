#ifndef MILK_MATRIX3_H
#define MILK_MATRIX3_H

namespace milk
{
    struct Matrix3
    {
        float m11, m12, m13;
        float m21, m22, m23;
        float m31, m32, m33;

        Matrix3()
                : Matrix3{1.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 1.0f}
        {
        }

        Matrix3(float m11, float m12, float m13,
                float m21, float m22, float m23,
                float m31, float m32, float m33)
        {
            this->m11 = m11;
            this->m12 = m12;
            this->m13 = m13;
            this->m21 = m21;
            this->m22 = m22;
            this->m23 = m23;
            this->m31 = m31;
            this->m32 = m32;
            this->m33 = m33;
        }

        static Matrix3 identity();

        static Matrix3 createTranslation(float x, float y);
    };

    // Global operators
    inline bool operator==(const Matrix3& m1, const Matrix3& m2)
    {
        return m1.m11 == m2.m11 && m1.m12 == m2.m12 && m1.m13 == m2.m13
            && m1.m21 == m2.m21 && m1.m22 == m2.m22 && m1.m23 == m2.m23
            && m1.m31 == m2.m31 && m1.m32 == m2.m32 && m1.m33 == m2.m33;
    }

    inline Matrix3 operator*(const Matrix3& m1, const Matrix3& m2)
    {
        Matrix3 matrix;

        matrix.m11 = (m1.m11 * m2.m11) + (m1.m12 * m2.m21) + (m1.m13 * m2.m31);
        matrix.m12 = (m1.m11 * m2.m12) + (m1.m12 * m2.m22) + (m1.m13 * m2.m32);
        matrix.m13 = (m1.m11 * m2.m13) + (m1.m12 * m2.m23) + (m1.m13 * m2.m33);

        matrix.m21 = (m1.m21 * m2.m11) + (m1.m22 * m2.m21) + (m1.m23 * m2.m31);
        matrix.m22 = (m1.m21 * m2.m12) + (m1.m22 * m2.m22) + (m1.m23 * m2.m32);
        matrix.m23 = (m1.m21 * m2.m13) + (m1.m22 * m2.m23) + (m1.m23 * m2.m33);

        matrix.m31 = (m1.m31 * m2.m11) + (m1.m32 * m2.m21) + (m1.m33 * m2.m31);
        matrix.m32 = (m1.m31 * m2.m12) + (m1.m32 * m2.m22) + (m1.m33 * m2.m32);
        matrix.m33 = (m1.m31 * m2.m13) + (m1.m32 * m2.m23) + (m1.m33 * m2.m33);

        return matrix;
    }

    inline Matrix3 Matrix3::identity()
    {
        return Matrix3{1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 1.0f};
    }

    inline Matrix3 Matrix3::createTranslation(float x, float y)
    {
        auto translation = Matrix3::identity();

        translation.m13 = x;
        translation.m23 = y;

        return translation;
    }
}

#endif
