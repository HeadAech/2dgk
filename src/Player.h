//
// Created by Hubert Klonowski on 10/10/2024.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/WindowBase.hpp>

#include "CollisionShape.h"

namespace sf {
    class Window;
}

enum InputMethod {
    KEYBOARD_WASD,
    KEYBOARD_IJKL,
    KEYBOARD_ARROWS,
    MOUSE
};

class Player {

    private:

    int id;
    float x;
    float y;
    sf::Vector2f velocity = sf::Vector2f(0, 0);

    int size = 60;

    InputMethod inputMethod;


    public:
    sf::RectangleShape sprite;

    CollisionShape* collisionShape;

    Player();

    Player(int id, InputMethod inputMethod, int x, int y, sf::Texture& texture);

    int getId();
    float getX();
    float getY();
    InputMethod getInputMethod();
    sf::Vector2f getVelocity();

    sf::Vector2f getPosition();

    float getCenterX();
    float getCenterY();

    int getSize();

    void setX(float x);
    void setY(float y);

    void setCollisionShape(CollisionShapeType type) {
        collisionShape = new CollisionShape(getPosition(), {float(getSize()), float(getSize())}, type);
    }

    void setCircleCollisionShape(float radius) {
        collisionShape = new CollisionShape(getPosition(), radius);
    }

    void update(sf::RenderWindow& window);

    Player *clone();

    void onSetPositionForPlayerId(int id, sf::Vector2f pos);
};



#endif //PLAYER_H
