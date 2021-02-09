#ifndef AVT_COLOR_H
#define AVT_COLOR_H

#include "Vec4D.h"

class Color {

private:
    Vec4D _rgba;
    inline Color(Vec4D rgba) : _rgba(rgba) {};

public:
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color DARK_RED;
    static const Color DARK_GREEN;
    static const Color DARK_BLUE;
    static const Color DARKER_RED;
    static const Color DARKER_GREEN;
    static const Color DARKER_BLUE;

    operator Vec4D() const { return _rgba; }
};

#endif //AVT_COLOR_H
