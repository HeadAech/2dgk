//
// Created by Hubert Klonowski on 10/10/2024.
//

#include "Player.h"

#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Signals.h"

Player::Player() {}

Player::Player(int id, InputMethod inputMethod, int x, int y, sf::Texture& texture) {
    Player::id = id;
    Player::x = x;
    Player::y = y;
    Player::inputMethod = inputMethod;

    sprite.setPosition(x, y);
    sprite.setSize(sf::Vector2f(size, size));
    sprite.setTexture(&texture);

    Signals::SetPositionForPlayerId.connect([this](int id, sf::Vector2f pos) { this->onSetPositionForPlayerId(id, pos);});
}

int Player::getId() {
    return Player::id;
}

InputMethod Player::getInputMethod() {
    return Player::inputMethod;
}

float Player::getX() {
    return Player::x;
}

float Player::getY() {
    return Player::y;
}

float Player::getCenterX() {
    return Player::x + size/2;
}

float Player::getCenterY() {
    return Player::y + size/2;
}



sf::Vector2f Player::getVelocity() {
    return Player::velocity;
}


void Player::setX(float x) {
    Player::x = x;
    sprite.setPosition(x, sprite.getPosition().y);
}

void Player::setY(float y) {
    Player::y = y;
    sprite.setPosition(sprite.getPosition().x, y);
}

void Player::update(sf::RenderWindow& window) {
    const float deAcceleration = 0.33f;

    const float maxSpeed = 5.5f;
    const float speed = 5.5f;

    const float slowRate = .80f;

    sf::Vector2f acceleration;

    if (getInputMethod() == KEYBOARD_WASD) {
        bool isSPressed = false;
        bool isDPressed = false;
        bool isWPressed = false;
        bool isAPressed = false;


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            acceleration.x -= deAcceleration;
            isAPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            acceleration.x += deAcceleration;
            isDPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            acceleration.y -= deAcceleration;
            isWPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            acceleration.y += deAcceleration;
            isSPressed = true;
        }

        // std::cout << "Keyboard {W: " << isWPressed << ", A: " << isAPressed << ", S: " << isSPressed << ", D: " << isDPressed << "}" << std::endl;
    }

    if (getInputMethod() == KEYBOARD_IJKL) {
        bool isSPressed = false;
        bool isDPressed = false;
        bool isWPressed = false;
        bool isAPressed = false;


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            acceleration.x -= deAcceleration;
            isAPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            acceleration.x += deAcceleration;
            isDPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
            acceleration.y -= deAcceleration;
            isWPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            acceleration.y += deAcceleration;
            isSPressed = true;
        }

        // std::cout << "Keyboard {W: " << isWPressed << ", A: " << isAPressed << ", S: " << isSPressed << ", D: " << isDPressed << "}" << std::endl;
    }

    if (getInputMethod() == KEYBOARD_ARROWS) {
        bool isSPressed = false;
        bool isDPressed = false;
        bool isWPressed = false;
        bool isAPressed = false;


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            acceleration.x -= deAcceleration;
            isAPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            acceleration.x += deAcceleration;
            isDPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            acceleration.y -= deAcceleration;
            isWPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            acceleration.y += deAcceleration;
            isSPressed = true;
        }

        // std::cout << "Keyboard {W: " << isWPressed << ", A: " << isAPressed << ", S: " << isSPressed << ", D: " << isDPressed << "}" << std::endl;
    }

    if (getInputMethod() == MOUSE) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorldPosition = window.mapPixelToCoords(mousePosition);
        // std::cout << "Mouse {x: " << mousePosition.x << ", y: " << mousePosition.y << "}" << std::endl;

        sf::Vector2f direction = sf::Vector2f(mouseWorldPosition.x - getCenterX(), mouseWorldPosition.y - getCenterY());

        acceleration = sf::Vector2f(direction.x, direction.y) * deAcceleration * 0.1f;
    }

    velocity += acceleration * speed;

    this->x += velocity.x;
    this->y += velocity.y;

    velocity = slowRate * velocity;

    sprite.setPosition(this->x, this->y);
    collisionShape->setPosition({this->x, this->y});

    if (velocity.x > maxSpeed) {
        velocity.x = maxSpeed;
    }

    if (velocity.x < -maxSpeed) {
        velocity.x = -maxSpeed;
    }

    if (velocity.y > maxSpeed) {
        velocity.y = maxSpeed;
    }

    if (velocity.y < -maxSpeed) {
        velocity.y = -maxSpeed;
    }

    // std::cout << collisionShape->left << " " << collisionShape->right << " " << collisionShape->top << " " << collisionShape->bottom << std::endl;
}

Player *Player::clone() {
    return new Player(*this);
}

int Player::getSize() {
    return this->size;
}

sf::Vector2f Player::getPosition() {
    return {x, y};
}

void Player::onSetPositionForPlayerId(int id, sf::Vector2f pos) {
    if (this->id == id) {
        this->setX(pos.x);
        this->setY(pos.y);
    }
}
