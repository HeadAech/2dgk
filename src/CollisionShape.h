//
// Created by Hubert Klonowski on 05/12/2024.
//

#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H
#include <SFML/System/Vector2.hpp>

enum CollisionShapeType {
    CIRCLE,
    BOX
};

enum TriggerType {
    NEXT_STAGE,
    PREVIOUS_STAGE,
    GAME_OVER,
    EMPTY,
    POINT
};

class CollisionShape {

    sf::Vector2f position = {0, 0};
    sf::Vector2f size = {0, 0};

    CollisionShapeType type = BOX;

    void setBounds() {
        left = position.x;
        top = position.y;
        right = position.x + size.x;
        bottom = position.y + size.y;
    }

    bool trigger = false;
    TriggerType triggerType = EMPTY;

public:

    float left;
    float top;
    float right;
    float bottom;
    float radius;


    bool triggered = false;


    CollisionShape(sf::Vector2f position, sf::Vector2f size) {
        this->position = position;
        this->size = size;
        setBounds();
    }

    CollisionShape(sf::Vector2f position, sf::Vector2f size, CollisionShapeType type) {
        this->position = position;
        this->size = size;
        this->type = type;
        setBounds();
    }

    CollisionShape(sf::Vector2f position, float radius) {
        this->position = position;
        this->radius = radius;
        this->type = CIRCLE;
        this->size = {radius * 2, radius * 2};
        setBounds();
    }

    void setPosition(sf::Vector2f position) {
        this->position = position;
        setBounds();
    }

    sf::Vector2f getPosition() {
        return position;
    }

    sf::Vector2f getSize() {
        return size;
    }

    CollisionShapeType getType() {
        return type;
    }

    sf::Vector2f getCenter() {
        if (type == BOX) return {position.x + size.x / 2, position.y + size.y / 2};
        if (type == CIRCLE) return {position.x + radius, position.y + radius};
        return {0,0};
    }

    bool isTrigger() {
        return trigger;
    }

    TriggerType getTriggerType() {
        return triggerType;
    }


    void setTrigger(bool trigger) {
        this->trigger = trigger;
    }

    void setTriggerType(TriggerType type) {
        this->trigger = true;
        this->triggerType = type;
    }
};

#endif //COLLISIONSHAPE_H
