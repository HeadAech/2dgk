//
// Created by Hubert Klonowski on 11/12/2024.
//

#ifndef SIGNALS_H
#define SIGNALS_H
#include <SFML/System/Vector2.hpp>

#include "Signal.h"
#include "WorldGenerator.h"

class Signals {

public:

    static Signal<int> NextStage;
    static Signal<int, sf::Vector2f> SetSpawnPoint;

    static Signal<int, sf::Vector2f> SetPositionForPlayerId;

    static Signal<std::string> ShowMessage;

    static Signal<WorldBoundary> SetBoundary;
};

#endif //SIGNALS_H
