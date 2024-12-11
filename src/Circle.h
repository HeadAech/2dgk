//
// Created by Hubert Klonowski on 21/11/2024.
//

#ifndef CIRCLE_H
#define CIRCLE_H
#include <random>
#include <SFML/Graphics/CircleShape.hpp>
#include <__random/random_device.h>

#include "WorldGenerator.h"

enum Mode {
    SIMULATION,
    FOLLOW
};

class Circle {

    sf::Vector2f position;

    float radius = 5;


    WorldBoundary boundary;

    float weight = 1;

    sf::Vector2f velocity = sf::Vector2f(1, 0);

    Mode mode = Mode::SIMULATION;

    sf::Vector2f destination = sf::Vector2f(0, 0);


    public:

    int ID;

    bool updatingCollision = false;

    sf::CircleShape shape;

    Circle(int ID, sf::Vector2f position, float radius, WorldBoundary boundary) {
        this->ID = ID;
        this->position = position;
        this->radius = radius;
        this->boundary = boundary;
        shape = sf::CircleShape(radius);
        shape.setRadius(radius);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(position);
    }

    Circle(const int ID, const float radius, WorldBoundary boundary) {
        this->ID = ID;
        this->radius = radius;
        this->boundary = boundary;
        shape = sf::CircleShape(radius);
        shape.setRadius(radius);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(position);
    }

    void setDestination(sf::Vector2f destination) {
        this->destination = destination;
    }

    float getWeight() const {
        return weight;
    }

    void setRandomVelocity() {
        // Random engine setup
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-2.0f, 2.0f);

        // Generate random x and y components
        float x = dis(gen);
        float y = dis(gen);
        setVelocity(x, y);
    }

    void setPosition(float x, float y) {
        this->position.x = x;
        this->position.y = y;
    }

    void setPosition(sf::Vector2f position) {
        this->position = position;
    }


    void setRandomPosition() {
        // Ensure WorldBoundary provides dimensions of the boundary
        float minX = boundary.minX + radius;
        float maxX = boundary.maxX - radius;
        float minY = boundary.minY + radius;
        float maxY = boundary.maxY - radius;

        // Random engine setup
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> disX(minX, maxX);
        std::uniform_real_distribution<float> disY(minY, maxY);

        // Generate random position within boundaries
        position = sf::Vector2f(disX(gen), disY(gen));
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    sf::Vector2f getCenter() const {
        float x = position.x + radius;
        float y = position.y + radius;
        return {x, y};
    }

    sf::Vector2f getNextTickCenter() const {
        float x = position.x + velocity.x;
        float y = position.y + velocity.y;
        return {x, y};
    }

    sf::Vector2f getVelocity() const {
        return velocity;
    }

    void setRandomColor() {
        std::vector<sf::Color> colors = {
            sf::Color::Red,
            sf::Color::Green,
            sf::Color::Blue,
            sf::Color::Yellow,
            sf::Color::Magenta,
            sf::Color::Cyan
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, colors.size() - 1);

        shape.setFillColor(colors[dis(gen)]);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(2);

    }

    void setMode(Mode mode) {
        this->mode = mode;
    }

    float getRadius() const {
        return radius;
    }

    void setVelocity(float x, float y) {
        velocity.x = x;
        velocity.y = y;
    }

    void setVelocity(sf::Vector2f velocity) {
        this->velocity = velocity;
    }

    void update() {
        if (mode == SIMULATION) {
            this->position.x += velocity.x;
            this->position.y += velocity.y;
        }

        if (mode == FOLLOW) {
            setPosition(destination);
        }
        // std::cout << position.x << " " << position.y << std::endl;
        shape.setPosition(position.x, position.y);
    }

};

#endif //CIRCLE_H
