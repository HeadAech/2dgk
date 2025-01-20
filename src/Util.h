//
// Created by Hubert Klonowski on 20/01/2025.
//

#ifndef UTIL_H
#define UTIL_H
#include <iomanip>
#include <sstream>

class Util {
public:
    static std::string toStringWithTwoDecimalPlaces(float value) {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2) << value;
        return stream.str();
    }
};

#endif //UTIL_H
