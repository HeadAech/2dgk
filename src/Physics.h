//
// Created by Hubert Klonowski on 21/11/2024.
//

#ifndef PHYSICS_H
#define PHYSICS_H
#include <vector>

#include "Circle.h"

#include <algorithm>
#include <complex>
#include <execution>

#include "Signals.h"

enum CollisionType {
    CIRCLES,
    WALLS,
    PLAYERS
};

enum Direction {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    OVERLAP
};

class Physics {

    std::vector<Player*> players;
    std::vector<CollisionShape*> blocks;

    WorldBoundary boundary;

    bool collideWithWalls = true;
    bool collideWithCircles = true;
    bool separating = false;

    void collisionWithWall(Circle* c) const {
        sf::Vector2f center = c->getCenter();

        if (center.x + c->getVelocity().x - c->getRadius() <= boundary.minX + 1 || center.x + c->getVelocity().x + c->getRadius() >= boundary.maxX - 1) {
            c->setVelocity(-c->getVelocity().x, c->getVelocity().y);
            // std::cout << "COLLISION IN X" << std::endl;
        }

        if (center.y + c->getVelocity().y - c->getRadius() <= boundary.minY + 1 || center.y + c->getVelocity().y + c->getRadius() >= boundary.maxY - 1)
        {
            c->setVelocity(c->getVelocity().x, -c->getVelocity().y);
            // std::cout << "COLLISION IN Y" << std::endl;

        }
    }


    // void collisionWithCircles(Circle* c1, Circle* c2) {
    //     if (c1->ID == c2->ID) {
    //         return;
    //     }
    //
    //     float distance = getDistanceBetweenCircles(c1, c2);
    //
    //     if (distance < c1->getRadius() + c2->getRadius()) {
    //         c1->updatingCollision = true;
    //         c2->updatingCollision = true;
    //         // std::cout << "Collision occurred!" << std::endl;
    //         sf::Vector2f centerDiff = c1->getPosition() - c2->getPosition();
    //         distance = getDistanceBetweenCircles(c1, c2);
    //
    //         sf::Vector2f normal = centerDiff / distance;
    //
    //         float tx = -normal.y;
    //         float ty = normal.x;
    //
    //         float dpTan1 = c1->getVelocity().x * tx + c1->getVelocity().y * ty;
    //         float dpTan2 = c2->getVelocity().x * tx + c2->getVelocity().y * ty;
    //
    //         float dpNorm1 = c1->getVelocity().x * normal.x + c1->getVelocity().y * normal.y;
    //         float dpNorm2 = c2->getVelocity().x * normal.x + c2->getVelocity().y * normal.y;
    //
    //         float m1 = (dpNorm1 * (c1->getWeight() - c2->getWeight()) + 2.0f * c2->getWeight() * dpNorm2) / (c1->getWeight() + c2->getWeight());
    //         float m2 = (dpNorm2 * (c2->getWeight() - c1->getWeight()) + 2.0f * c1->getWeight() * dpNorm1) / (c1->getWeight() + c2->getWeight());
    //
    //         c1->setVelocity(tx * dpTan1 + normal.x * m1, ty * dpTan1 + normal.y * m1);
    //         c2->setVelocity(tx * dpTan2 + normal.x * m2, ty * dpTan2 + normal.y * m2);
    //     }
    // }

    // void seperateCircles(Circle* c1, Circle* c2) {
    //     if (c1->ID == c2->ID) return;
    //
    //     float distance = getDistanceBetweenCircles(c1, c2);
    //
    //     if (distance <= c1->getRadius() + c2->getRadius()) {
    //         sf::Vector2f s_v = getSeparationVector(c1, c2);
    //
    //         c1->setPosition(c1->getPosition() - s_v);
    //         c2->setPosition(c2->getPosition() + s_v);
    //     }
    //
    //
    // }

    // sf::Vector2f getSeparationVector(Circle* c1, Circle* c2) {
    //     sf::Vector2f v1 = c2->getCenter() - c1->getCenter();
    //
    //     float v1_d = getDistanceBetweenCircles(c1, c2);
    //
    //     return (v1/v1_d) * ((c1->getRadius() + c2->getRadius() - v1_d));
    // }

    bool isCollidingWithBoxShape(CollisionShape* shape1, CollisionShape* shape2) {
        return shape1->right > shape2->left && shape2->right > shape1->left && shape1->bottom > shape2->top && shape2->bottom > shape1->top;
    }

    bool isCollidingWithCircleShape(CollisionShape* shape1, CollisionShape* shape2) {
        sf::Vector2f c = shape1->getCenter();
        sf::Vector2f f = getF(c, shape2);
        sf::Vector2f distance = c - f;
        float distanceSquared = std::sqrt(distance.x * distance.x + distance.y * distance.y);
        return distanceSquared < shape1->radius;

    }

    static sf::Vector2f getF(sf::Vector2f c, CollisionShape* shape2) {
        return {clamp(shape2->left, shape2->right, c.x), clamp(shape2->top, shape2->bottom, c.y)};
    }

    sf::Vector2f getSeperationVectorBox(CollisionShape* shape1, CollisionShape* shape2) {
        float left = shape1->right - shape2->left;
        float right = shape2->right - shape1->left;
        float top = shape1->bottom - shape2->top;
        float bottom = shape2->bottom - shape1->top;

        sf::Vector2f s = {0, 0};
        if (left < right) {
            s.x = -left;
        } else {
            s.x = right;
        }

        if (top < bottom) {
            s.y = -top;
        } else {
            s.y = bottom;
        }

        if (std::abs(s.x) < std::abs(s.y)) {
            s.y = 0;
        }
        else if (std::abs(s.x) > std::abs(s.y)) {
            s.x = 0;
        }

        return s;

    }

    sf::Vector2f getSeparationVectorCircle(CollisionShape* shape1, CollisionShape* shape2) {
        //shape1 circle, shape2 box
        sf::Vector2f c = shape1->getCenter();
        sf::Vector2f f = getF(c, shape2);
        if (c == f) {
            return getSeperationVectorBox(shape1, shape2);
        }

        sf::Vector2f distance = c - f;
        float distanceSquared = std::sqrt(distance.x * distance.x + distance.y * distance.y);

        return (distance/distanceSquared) * (shape1->radius - distanceSquared);
    }

    void applySeperationToPlayer(Player* player, std::vector<CollisionShape*>& collisionShapes) {
        std::vector<sf::Vector2f> ss;

        for (auto collisionShape : collisionShapes) {
            CollisionShapeType type = player->collisionShape->getType();
            sf::Vector2f a;
            if (type == BOX) a = getSeperationVectorBox(player->collisionShape, collisionShape);
            else if (type == CIRCLE) a = getSeparationVectorCircle(player->collisionShape, collisionShape);
            ss.push_back(a);
        }

        //TODO needs improvement
        sf::Vector2f s;
        if (player->collisionShape->getType() == CIRCLE) {

            s = combineSeparationVectors(ss);
        }
        else {
            if (ss.size() > 2) s = combineSeparationVectors(ss);
            else             s = getCombinedSeparationVector(ss);
        }

        player->setX(player->getX() + s.x);
        player->setY(player->getY() + s.y);
    }

    sf::Vector2f combineSeparationVectors(const std::vector<sf::Vector2f>& separationVectors) {
        sf::Vector2f combinedVector(0.f, 0.f);

        // Sum up all separation vectors
        for (const sf::Vector2f& vec : separationVectors) {
            combinedVector += vec;
        }

        return combinedVector;
    }

    sf::Vector2f resolveCornerCollision(const std::vector<sf::Vector2f>& separationVectors) {

        // Combine all separation vectors
        sf::Vector2f combinedVector = combineSeparationVectors(separationVectors);

        // Optional: Normalize the result for uniform movement
        if (combinedVector != sf::Vector2f(0.f, 0.f)) {
            float magnitude = std::sqrt(combinedVector.x * combinedVector.x + combinedVector.y * combinedVector.y);
            combinedVector /= magnitude; // Normalize
        }

        return combinedVector;
    }

    Direction getRelativeDirectionByCenters(CollisionShape* shape1, CollisionShape* shape2) {
        sf::Vector2f pos1 = shape1->getPosition();
        sf::Vector2f size1 = shape1->getSize();
        sf::Vector2f pos2 = shape2->getPosition();
        sf::Vector2f size2 = shape2->getSize();

        // Calculate centers of the shapes
        sf::Vector2f center1 = {pos1.x + size1.x / 2, pos1.y + size1.y / 2};
        sf::Vector2f center2 = {pos2.x + size2.x / 2, pos2.y + size2.y / 2};

        // Compute the difference between the centers
        sf::Vector2f delta = center2 - center1;

        // Determine the dominant axis
        if (std::fabs(delta.x) > std::fabs(delta.y)) {
            return (delta.x > 0) ? LEFT : RIGHT; // Positive x: shape1 is to the LEFT of shape2
        } else {
            return (delta.y > 0) ? TOP : BOTTOM; // Positive y: shape1 is ABOVE shape2
        }
    }

    std::string directionToString(Direction dir) {
        switch (dir) {
            case TOP:
                return "TOP";
            case BOTTOM:
                return "BOTTOM";
            case LEFT:
                return "LEFT";
            case RIGHT:
                return "RIGHT";
            case OVERLAP:
                return "OVERLAP";
            default:
                return "UNKNOWN";
        }
    }

    sf::Vector2f getCombinedSeparationVector(const std::vector<sf::Vector2f>& separationVectors) {
        sf::Vector2f combined = {0, 0};

        for (const auto& vec : separationVectors) {
            if (std::abs(vec.x) > std::abs(combined.x)) {
                combined.x = vec.x; // Priorytet większej separacji na osi X
            }
            if (std::abs(vec.y) > std::abs(combined.y)) {
                combined.y = vec.y; // Priorytet większej separacji na osi Y
            }
        }

        // Jeśli separacja na jednej osi jest bardziej istotna, wyzeruj drugą oś
        if (std::abs(combined.x) < std::abs(combined.y)) {
            combined.x = 0;
        } else if (std::abs(combined.x) > std::abs(combined.y)) {
            combined.y = 0;
        }

        return combined;
    }

   static float getDistanceBetweenCircles(Circle* c1, Circle* c2){
        sf::Vector2f v1 = c2->getCenter() - c1->getCenter();
        return sqrt(v1.x * v1.x + v1.y * v1.y);
    }

    static float getDistanceBetweenCirclesNextTick(Circle* c1, Circle* c2){
        sf::Vector2f v1 = c2->getNextTickCenter() - c1->getNextTickCenter();
        return sqrt(v1.x * v1.x + v1.y * v1.y);
    }

    static float clamp(float min, float max, float value) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    public:

    std::vector<Circle*> circles;

    Physics(){}

    ~Physics(){}

    void addCircle(Circle* c) {
        circles.push_back(c);
    }

    void setBoundaries(WorldBoundary b) {
        boundary = b;
    }

    void checkCollision(CollisionType type) {

        // destroyOutOfBoundsCircles();
        teleportOutOfBoundsCircles();

        if (type==WALLS && collideWithWalls) {
            for (int i = 0; i < circles.size(); i++) {
                collisionWithWall(circles[i]);
            }
        }

        for (int i = 0; i < circles.size(); i++) {
            circles[i]->updatingCollision = false;
        }

        if (type == CIRCLES && circles.size() > 1) {
            for (int i = 0; i < circles.size(); i++) {
                for (int j = 0; j < circles.size(); j++) {
                    if (circles[i]->updatingCollision && circles[j]->updatingCollision) continue;
                    // if (collideWithCircles)
                    //     collisionWithCircles(circles[i], circles[j]);
                    // if (separating)
                    //     seperateCircles(circles[i], circles[j]);
                }
            }

        }

    }

    void checkCollisionForPlayersWithBlocks() {
        for (Player* player : players) {
            std::vector<CollisionShape*> collidingWith;
            for (CollisionShape* collisionShape : blocks) {
                CollisionShapeType t = player->collisionShape->getType();
                bool collision = false;
                if (t == CIRCLE) {
                    if (isCollidingWithCircleShape(player->collisionShape, collisionShape)) {
                        // std::cout << "Circle collision" << std::endl;
                        if(collisionShape->isTrigger()) {
                            if (!collisionShape->triggered) {
                                if (collisionShape->getTriggerType() == NEXT_STAGE) {
                                    collisionShape->triggered = true;
                                    Signals::NextStage.emit(player->getId());
                                }
                            }
                        } else
                            collision = true;
                    }
                } else if (t == BOX) {
                    if (isCollidingWithBoxShape(player->collisionShape, collisionShape)) {
                        // std::cout << "Collision! Player: " << player->getId() << " " << " box: " << collisionShape->left << std::endl;
                        // applySeperationToPlayer(player, collisionShape);
                        if(collisionShape->isTrigger()) {
                            if (!collisionShape->triggered) {
                                if (collisionShape->getTriggerType() == NEXT_STAGE) {
                                    collisionShape->triggered = true;
                                    Signals::NextStage.emit(player->getId());
                                }
                            }
                        } else
                            collision = true;
                    }
                }

                if (collision) {
                    if (std::find(collidingWith.begin(), collidingWith.end(), collisionShape) == collidingWith.end())
                        collidingWith.push_back(collisionShape);
                }
                // break;
            }

            if (collidingWith.size() > 0) {
                applySeperationToPlayer(player, collidingWith);

            }
            // std::cout << collidingWith.size() << std::endl;
            collidingWith.clear();
        }
    }

    void teleportOutOfBoundsCircles() {
        for (int i = 0; i < circles.size(); i++) {
            auto c = circles[i];
            if (c->getCenter().x > boundary.maxX || c->getCenter().x < boundary.minX) {
                c->setRandomPosition();
            }

            if(c->getCenter().y > boundary.maxY || c->getCenter().y < boundary.minY) {
                c->setRandomPosition();
            }
        }
    }

    void destroyOutOfBoundsCircles() {
        for (int i = 0; i < circles.size(); i++) {
            auto c = circles[i];
            if (c->getCenter().x > boundary.maxX || c->getCenter().x < boundary.minX) {
                circles.erase(find(circles.begin(), circles.end(), c));
            }

            if(c->getCenter().y > boundary.maxY || c->getCenter().y < boundary.minY) {
                circles.erase(find(circles.begin(), circles.end(), c));
            }
        }
    }

    void toggleCollisionWithWalls() {
        this->collideWithWalls = !this->collideWithWalls;
    }

    void toggleCollisionWithCircles() {
        this->collideWithCircles = !this->collideWithCircles;
    }

    void toggleSeparation() {
        this->separating = !this->separating;
    }

    bool isCollidingWithCircles() {
        return collideWithCircles;
    }

    bool isCollidingWithWalls() {
        return collideWithWalls;
    }

    void addPlayer(Player* player) {
        players.push_back(player);
    }

    void removePlayer(Player* player) {
        players.erase(std::remove(players.begin(), players.end(), player));
    }

    void addBlock(CollisionShape* collisionShape) {
        blocks.push_back(collisionShape);
    }

    void clearBlocks() {
        blocks.clear();
    }

};

#endif //PHYSICS_H
