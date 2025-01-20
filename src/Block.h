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
    // std::string texturePath;
    sf::Vector2i textureCoords;
    bool collidable;

    // BlockData(std::string name, std::string character, float sizeX, float sizeY, std::string texturePath, bool collidable) {
    //     this->name = name;
    //     this->character = character;
    //     this->sizeX = sizeX;
    //     this->sizeY = sizeY;
    //     this->texturePath = texturePath;
    //     this->collidable = collidable;
    // }

    BlockData(std::string name, std::string character, float sizeX, float sizeY, sf::Vector2i textureCoords, bool collidable) {
        this->name = name;
        this->character = character;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->textureCoords = textureCoords;
        this->collidable = collidable;
    }
};

class Block {
private:
    std::string name;

    sf::Vector2i textureCoords;
    sf::Vector2i frameSize = {32, 32};

    public:
    bool collidable = true;

    sf::Vector2f size;

    sf::RectangleShape shape;

    CollisionShape* collisionShape;

    Block(sf::Vector2i textureCoords, sf::Texture* spriteSheet) {
        shape.setTexture(spriteSheet);
        setTexture(textureCoords);
    }

    void setSize(float sizeX, float sizeY) {
        this->size = sf::Vector2f(sizeX, sizeY);
    }

    void setTextureSize(float x, float y) {
        shape.setSize(sf::Vector2f(x, y));
    }

    void setName(std::string name) {
        this->name = name;
    }

    void setTexture(sf::Texture* texture) {
        shape.setTexture(texture);
    }

    std::string getName() {
        return this->name;
    }

    void setCollidable(bool v) {
        this->collidable = v;
    }

    void setTexture(sf::Vector2i textureCoords) {
        this->textureCoords = textureCoords;
        shape.setTextureRect(sf::IntRect(textureCoords.x * frameSize.x + 1, textureCoords.y * frameSize.y + 1, frameSize.x - 1 , frameSize.y - 1));
    }

    // void setSize(float x, float y) {
    //     shape.setSize({x, y});
    // }

    void setPosition(float x, float y) {
        shape.setPosition(x, y);
    }

    void createCollisionShape(bool isTrigger = false) {
        collisionShape = new CollisionShape(shape.getPosition(), getSize(), BOX);
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
        return size;
    }
};



#endif //BLOCK_H
