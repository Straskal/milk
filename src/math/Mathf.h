#ifndef MILK_MATHF_H
#define MILK_MATHF_H

namespace milk
{
    class Mathf
    {
    public:
        static float clamp(float value, float min, float max)
        {
            if (value < min)
                return min;

            if (value > max)
                return max;

            return value;
        }
    };
}

#endif
