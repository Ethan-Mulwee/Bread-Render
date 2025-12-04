#ifndef BRL_COLOR
#define BRL_COLOR

#include "vector_type.hpp"

namespace brl {
    struct Color {
        smath::vector4 vector;
    };

    static constexpr Color WHITE = Color{1.0f, 1.0f, 1.0f, 1.0f};
    static constexpr Color RED = Color{1.000f, 0.200f, 0.322f, 1.0f};
    static constexpr Color PURE_RED = Color{1.000f, 0.000f, 0.000f, 1.0f};
    static constexpr Color GREEN = Color{0.471f, 0.863f, 0.000f, 1.0f};
    static constexpr Color PURE_GREEN = Color{0.0f, 1.000f, 0.000f, 1.0f};
    static constexpr Color BLUE = Color{0.157f, 0.565f, 1.0f, 1.0f};
    static constexpr Color PRE_BLUE = Color{0.000f, 0.000f, 1.0f, 1.0f};
    static constexpr Color ORANGE = Color{1.0f,0.385f,0.136f,1.0f};
}

#endif