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

void Player::oneShotInput(sf::Event &event) {
    if (getInputMethod() == KEYBOARD_WASD) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::S) {
                applyForce({0, 900});
            }

            if (event.key.code == sf::Keyboard::Space) {
                if (isOnFloor) {
                    this->jump();
                } else {
                    if (canDoubleJump) {
                        this->jump();
                        canDoubleJump = false;
                    }
                }
            }

            if (event.key.code == sf::Keyboard::Up) {
                jumpHeight += 10;
                calculateJumpParameters();
                printInfo();
            }

            if (event.key.code == sf::Keyboard::Down) {
                jumpHeight -= 10;
                calculateJumpParameters();
                printInfo();
            }

            if (event.key.code == sf::Keyboard::Left) {
                jumpDistance -= 10;
                calculateJumpParameters();
                printInfo();
            }

            if (event.key.code == sf::Keyboard::Right) {
                jumpDistance += 10;
                calculateJumpParameters();
                printInfo();
            }
        }
    }
}


void Player::input(sf::Event &event) {
    if (getInputMethod() == KEYBOARD_WASD) {
        bool isSPressed = false;
        bool isDPressed = false;
        bool isWPressed = false;
        bool isAPressed = false;
        bool isSpacePressed = false;


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            direction = -1;
            if (!isOnFloor && velocity.x < -50)
                return;
            if (!isOnFloor)
                acceleration.x -= deAcceleration / 10;
            else
                acceleration.x -= deAcceleration;
            isAPressed = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            direction = 1;
            if (!isOnFloor && velocity.x > 50)
                return;
            if (!isOnFloor)
                acceleration.x += deAcceleration / 10;
            else
                acceleration.x += deAcceleration;
            isDPressed = true;
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

    // Apply horizontal movement
    velocity.x += acceleration.x * speed;

    if (isOnFloor) {
        velocity.x *= slowRate;
    }


    if (isOnFloor && velocity.y > 0) {
        velocity.y = 0;
    }

    if (isOnFloor && canDoubleJump == false) {
        canDoubleJump = true;
    }

    velocity.y = std::clamp(velocity.y, -2000.0f, 2000.0f);

    this->x += velocity.x * delta + 0.5f * acceleration.x * delta * delta;
    this->y += velocity.y * delta + 0.5f * acceleration.y * delta * delta;

    sprite.setPosition(this->x, this->y);
    //WARN: Collision shape is off-set, because of the sprite rendering of the ground (not a full block)
    collisionShape->setPosition({this->x, this->y - 20});
    if (groundCheck->getType() == BOX)
        groundCheck->setPosition({this->x + size/2 - (size * 0.8f / 2), this->y + size - 20});
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
    velocity += force;
}

void Player::jump() {
    // Apply vertical jump velocity
    // calculateJumpParameters();
    velocity.y = initialJumpVelocity;
    velocity.x = jumpVelocity * direction;
    printInfo();
}

float Player::getGravity() {
    // float jumpGravity = ((-2.0f * jumpHeight * (std::abs(velocity.x) * std::abs(velocity.x))) / (jumpDistance * jumpDistance) * -1.0f);
    return velocity.y < 0.0 ? jumpGravity : jumpGravity;
}

void Player::printInfo() const {
    std::cout << std::endl << "-----" << std::endl;
    std::cout << "Jump Height: " << jumpHeight << std::endl;
    std::cout << "Jump Distance: " << jumpDistance << std::endl;
    std::cout << "Jump Velocity: " << jumpVelocity << std::endl;
    std::cout << "Gravity: " << jumpGravity << std::endl;
    std::cout << "Initial Jump Velocity: " << initialJumpVelocity << std::endl;
    std::cout << "-----" << std::endl;
}

void Player::calculateJumpParameters() {
    jumpVelocity = jumpDistance / jumpTimeToPeak;
    initialJumpVelocity = (-2 * jumpHeight * jumpVelocity) / jumpDistance;
    jumpGravity = (2 * jumpHeight * (jumpVelocity * jumpVelocity)) / (jumpDistance * jumpDistance);
}

