//
// Created by Hubert Klonowski on 14/11/2024.
//

#ifndef MATH_H
#define MATH_H


class Math {

public:

    static float lerp(float a, float b, float t) {
        return a * (1.0f - t) + b * t;
    }
};

#endif //MATH_H
