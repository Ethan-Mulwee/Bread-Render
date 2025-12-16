#ifndef BRL_COLOR
#define BRL_COLOR

#include "vector_type.hpp"

namespace brl {
    struct Color {
        smath::vector4 vector;
    };

    static constexpr Color WHITE = Color{1.0f, 1.0f, 1.0f, 1.0f};
    static constexpr Color BLACK = Color{0.0f, 0.0f, 0.0f, 1.0f};
    static constexpr Color DARK_GREY = Color{0.1f, 0.1f, 0.1f, 1.0f};
    static constexpr Color GREY = Color{0.2f, 0.2f, 0.2f, 1.0f};
    static constexpr Color BLUE_GREY = Color{0.054902f, 0.101964f, 0.15294f, 1.0f};
    static constexpr Color RED = Color{1.000f, 0.200f, 0.322f, 1.0f};
    static constexpr Color FREYA_RED = Color{0.955216f, 0.088624f, 0.354514f, 1.0f};
    static constexpr Color PURE_RED = Color{1.000f, 0.000f, 0.000f, 1.0f};
    static constexpr Color GREEN = Color{0.471f, 0.863f, 0.000f, 1.0f};
    static constexpr Color PURE_GREEN = Color{0.0f, 1.000f, 0.000f, 1.0f};
    static constexpr Color BLUE = Color{0.157f, 0.565f, 1.0f, 1.0f};
    static constexpr Color FREYA_BLUE = Color{0.18878f, 0.599248f, 0.820255f, 1.0f};
    static constexpr Color PURE_BLUE = Color{0.000f, 0.000f, 1.0f, 1.0f};
    static constexpr Color ORANGE = Color{1.0f,0.385f,0.136f,1.0f};
}

#endif