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

    const float deAcceleration = 0.33f;

    const float slowRate = .80f;

    const float maxSpeed = 400.5f * slowRate;
    const float speed = 400.5f;

    public:

    int id;
    float x;
    float y;
    sf::Vector2f velocity = sf::Vector2f(0, 0);

    sf::Vector2f acceleration = sf::Vector2f(0, 0);

    int size = 60;

    InputMethod inputMethod;

    bool isOnFloor = false;

    //jumping
    float jumpHeight = 50.0f;
    float jumpTimeToPeak = 1.8f;
    float jumpTimeToDescent = 1.5f;
    float jumpDistance = 1.0f;

    // float jumpVelocity = ((2.0f * jumpHeight) / jumpTimeToPeak) * -1.0f;
    float jumpGravity = ((-2.0f * jumpHeight) / (jumpTimeToPeak * jumpTimeToPeak) * -1.0f);
    float fallGravity = ((-2.0f * jumpHeight) / (jumpTimeToDescent * jumpTimeToDescent) * -1.0f);

    float gravityMultiplier = 10.0f;

    bool canDoubleJump = false;

    sf::RectangleShape sprite;

    CollisionShape* collisionShape;

    CollisionShape* groundCheck;

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
        groundCheck = new CollisionShape({getPosition().x, getPosition().y}, {getSize() * 0.8f, 5.0f}, type);
    }

    void setCircleCollisionShape(float radius) {
        collisionShape = new CollisionShape(getPosition(), radius);
        groundCheck = new CollisionShape({getPosition().x, getPosition().y}, 5.0f);
    }

    void applyForce(sf::Vector2f force);
    void jump();

    void update(sf::RenderWindow& window, float delta);
    void input(sf::Event& event);

    float getGravity();

    Player *clone();

    void onSetPositionForPlayerId(int id, sf::Vector2f pos);
};



#endif //PLAYER_H
