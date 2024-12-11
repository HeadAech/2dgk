//
// Created by Hubert Klonowski on 17/10/2024.
//

#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "CollisionShape.h"

struct BlockData {
    std::string name;
    std::string character;
    float sizeX;
    float sizeY;
    std::string texturePath;
    bool collidable;

    BlockData(std::string name, std::string character, float sizeX, float sizeY, std::string texturePath, bool collidable) {
        this->name = name;
        this->character = character;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->texturePath = texturePath;
        this->collidable = collidable;
    }
};

class Block {
private:
    std::string name;
    sf::Texture* texture;

    public:
    bool collidable = true;

    sf::RectangleShape shape;

    CollisionShape* collisionShape;

    Block(sf::Texture* texture) {
        this->texture = texture;
        shape.setTexture(texture);
    }


    void setName(std::string name) {
        this->name = name;
    }

    void setTexture(sf::Texture* texture) {
        this->texture = texture;
        shape.setTexture(texture);
    }

    std::string getName() {
        return this->name;
    }

    void setCollidable(bool v) {
        this->collidable = v;
    }

    void setSize(float x, float y) {
        shape.setSize({x, y});
    }

    void setPosition(float x, float y) {
        shape.setPosition(x, y);
    }

    void createCollisionShape(bool isTrigger = false) {
        collisionShape = new CollisionShape(shape.getPosition(), shape.getSize(), BOX);
        collisionShape->setTrigger(isTrigger);
        // std::cout << "Generated collision shape at " << shape.getPosition().x << " " << shape.getPosition().y << std::endl;
        // std::cout << "Size " << shape.getSize().x << " " << shape.getSize().y << std::endl;

    }

    void setTriggerOnCollisionShape(TriggerType type = EMPTY) const {
        collisionShape->setTriggerType(type);
    }

    sf::Vector2f getPosition() {
        return shape.getPosition();
    }

    sf::Vector2f getSize() {
        return shape.getSize();
    }
};



#endif //BLOCK_H
