//
// Created by Hubert Klonowski on 10/10/2024.
//

#include "Player.h"

#include <iostream>
#include <SFML/Window/Event.hpp>
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

void Player::input(sf::Event &event) {
    if (getInputMethod() == KEYBOARD_WASD) {
        bool isSPressed = false;
        bool isDPressed = false;
        bool isWPressed = false;
        bool isAPressed = false;
        bool isSpacePressed = false;


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            acceleration.x -= deAcceleration;
            isAPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            acceleration.x += deAcceleration;
            isDPressed = true;
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                if (isOnFloor) {
                    this->jump();
                } else {
                    if (canDoubleJump) {
                        this->jump();
                        canDoubleJump = false;
                    }
                }
                isSpacePressed = true;
            }
        }
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

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::I) {
                if (isOnFloor) {
                    this->jump();
                } else {
                    if (canDoubleJump) {
                        this->jump();
                        canDoubleJump = false;
                    }
                }
                isWPressed = true;
            }
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

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                if (isOnFloor) {
                    this->jump();
                } else {
                    if (canDoubleJump) {
                        std::cout << "Arrow Double Jumped" << std::endl;
                        this->jump();
                        canDoubleJump = false;
                    }
                }
                isWPressed = true;
            }
        }

        // std::cout << "Keyboard {W: " << isWPressed << ", A: " << isAPressed << ", S: " << isSPressed << ", D: " << isDPressed << "}" << std::endl;
    }
}


void Player::update(sf::RenderWindow& window, float delta) {

    if (getInputMethod() == MOUSE) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorldPosition = window.mapPixelToCoords(mousePosition);
        // std::cout << "Mouse {x: " << mousePosition.x << ", y: " << mousePosition.y << "}" << std::endl;

        sf::Vector2f direction = sf::Vector2f(mouseWorldPosition.x - getCenterX(), mouseWorldPosition.y - getCenterY());

        acceleration = sf::Vector2f(direction.x, direction.y) * deAcceleration * 0.1f;
    }

    velocity.y += getGravity() * delta;

    // Air control - reduce horizontal acceleration while in air
    if (!isOnFloor) {
        acceleration.x *= 0.5f;  // Reduce air control
    }

    // Apply horizontal movement
    velocity.x += acceleration.x * speed;
    velocity.x = slowRate * velocity.x;

    // velocity += acceleration * speed * delta;



    if (isOnFloor && velocity.y > 0) {
        velocity.y = 0;
    }

    if (isOnFloor && canDoubleJump == false) {
        canDoubleJump = true;
    }

    if (velocity.x > maxSpeed && isOnFloor) {
        velocity.x = maxSpeed;
    }

    if (velocity.x < -maxSpeed && isOnFloor) {
        velocity.x = -maxSpeed;
    }

    velocity.y = std::clamp(velocity.y, -1000.0f, 1000.0f);

    // std::cout << "delta time: " << delta << std::endl;
    std::cout << "velocity: " << velocity.x << ", " << velocity.y << std::endl;
    // std::cout << "Can double jump: " << canDoubleJump << std::endl;
    // std::cout << "acceleration: " << acceleration.x << ", " << acceleration.y << std::endl;

    this->x += velocity.x * delta + 0.5f * acceleration.x * delta * delta;
    this->y += velocity.y * delta + 0.5f * acceleration.y * delta * delta;

    sprite.setPosition(this->x, this->y);
    collisionShape->setPosition({this->x, this->y});
    if (groundCheck->getType() == BOX)
        groundCheck->setPosition({this->x + size/2 - (size * 0.8f / 2), this->y + size});
    if (groundCheck->getType() == CIRCLE)
        groundCheck->setPosition({this->x + size/2, this->y + size});
    acceleration = {0,0};
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

void Player::applyForce(sf::Vector2f force) {
    acceleration += force;
}

void Player::jump() {
    // Apply vertical jump velocity
    velocity.y = initialJumpVelocity;
    
    // Boost horizontal velocity when jumping
    if (std::abs(velocity.x) > 0.1f) {  // Only boost if already moving horizontally
        velocity.x *= horizontalJumpBoost;
    }
}

float Player::getGravity() {
    // float jumpGravity = ((-2.0f * jumpHeight * (std::abs(velocity.x) * std::abs(velocity.x))) / (jumpDistance * jumpDistance) * -1.0f);
    return velocity.y < 0.0 ? jumpGravity : fallGravity;
}
