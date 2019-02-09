#ifndef MILK_COLOR_H
#define MILK_COLOR_H

#include <cstdint>

namespace milk
{
    struct Color
    {
        /// A Color!
        /// \param red: unsigned 8 bit integer
        /// \param blue: unsigned 8 bit integer
        /// \param green: unsigned 8 bit integer
        /// \param alpha: unsigned 8 bit integer
        explicit Color(uint8_t red = 0x00, uint8_t blue = 0x00, uint8_t green = 0x00, uint8_t alpha = 0xFF)
                : red(red),
                  blue(blue),
                  green(green),
                  alpha(alpha)
        {
        }

        uint8_t red;
        uint8_t blue;
        uint8_t green;
        uint8_t alpha;

        /// \returns transparent.
        static Color clear()
        {
            return Color{0x00, 0x00, 0x00, 0x00};
        }

        /// \returns black.
        static Color black()
        {
            return Color{0x00, 0x00, 0x00, 0xFF};
        }
    };
}

#endif
