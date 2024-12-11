//
// Created by Hubert Klonowski on 11/12/2024.
//

#ifndef GUIDEARROW_H
#define GUIDEARROW_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class GuideArrow {
public:

    sf::Sprite* sprite;
    sf::Texture* texture;

    sf::Vector2f target;
    sf::Vector2f source;
    float distance = 80;

    GuideArrow(sf::Texture* texture, sf::Vector2f target, sf::Vector2f source) {
        this->texture = texture;
        this->target = target;
        this->source = source;
        createSprite();
    }

    GuideArrow(sf::Texture* texture) {
        this->texture = texture;
        createSprite();
    }

    void createSprite() {
        sprite = new sf::Sprite();
        sprite->setTexture(*texture);
        sprite->setScale(0.01f, 0.01f);
        sprite->setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
    }

    void setTarget(sf::Vector2f target) {
        this->target = target;
    }

    void setSource(sf::Vector2f source) {
        this->source = source;
    }

    void update(sf::Vector2f source, sf::Vector2f target) {
        this->target = target;
        this->source = source;
        float deltaX = target.x - source.x;
        float deltaY = target.y - source.y;

        // Calculate the angle to the target in radians
        float angle = std::atan2(deltaY, deltaX);

        // Position the arrow around the player at a fixed radius
        float x = source.x + distance * std::cos(angle);
        float y = source.y + distance * std::sin(angle);


        // Update the arrow's position
        sprite->setPosition(x, y);

        // Rotate the arrow to face the target
        float rotationAngle = angle * 180.f / 3.14159f;  // Convert radians to degrees
        sprite->setRotation(rotationAngle);
    }

    float getDistanceToTarget() const {
        return std::sqrt(std::pow(target.x - source.x, 2) + std::pow(target.y - source.y, 2));
    }

    void draw(sf::RenderWindow& window) {
        if (getDistanceToTarget() > 500)
            window.draw(*sprite);
    }
};

#endif //GUIDEARROW_H
