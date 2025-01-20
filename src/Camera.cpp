//
// Created by Hubert Klonowski on 24/10/2024.
//

#include "Camera.h"

#include <iostream>
#include <ostream>
#include <valarray>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include <cmath>

#include "Math.h"
#include "Signals.h"

Camera::Camera(sf::RenderWindow& window, CameraMode cameraMode, Player* playerToFollow) {
    this->splitLeftView = window.getDefaultView();
    this->cameraMode = cameraMode;
    this->playerToFollow = playerToFollow;
}


Camera::Camera(sf::RenderWindow& window, CameraMode cameraMode) {
    this->splitLeftView = window.getDefaultView();
    this->cameraMode = cameraMode;

    Signals::SetBoundary.connect([this] (WorldBoundary b) {this->setWorldBoundary(b);});
}

Camera::~Camera() {
}

sf::Vector2f Camera::getPosition() {
    return {getView().getCenter().x, getView().getCenter().y + offsetY};
}


sf::View &Camera::getView() {
    return this->splitLeftView;
}

void Camera::processCameraInput(sf::Event &event) {
    return;
    if (event.type == sf::Event::KeyPressed) {

        //zooming
        if (event.key.code == sf::Keyboard::Equal) {
            if (currentZoom - 1 < minZoom) {
                currentZoom = minZoom;
                return;
            }

            currentZoom -= 1;
            splitLeftView.zoom(zoomFactor);
        }

        if (event.key.code == sf::Keyboard::Dash) {
            if (currentZoom + 1 > maxZoom) {
                currentZoom = maxZoom;
                return;
            }
            currentZoom += 1;
            splitLeftView.zoom(1 + (1 - zoomFactor));
        }

        //toggle camera mode

        if (event.key.code == sf::Keyboard::Space) {
            if (cameraMode == FOLLOW_ONE_PLAYER)
                cameraMode = FOLLOW_TWO_PLAYERS;
            else if (cameraMode == FOLLOW_TWO_PLAYERS)
                cameraMode = FOLLOW_ONE_PLAYER;
        }
    }
}



void Camera::update(sf::RenderWindow& window) {
    float delta = .1f;

    sf::Vector2f target(0.f, 0.f);

    if (cameraMode == FOLLOW_ONE_PLAYER) {
        target.x = this->playerToFollow->getCenterX();
        target.y = this->playerToFollow->getCenterY() + offsetY;

        smoothMoveToTarget(splitLeftView, target);
    }

    if (cameraMode == FOLLOW_TWO_PLAYERS) {
        float distance = std::hypot(
            playerToFollow->getCenterX() - secondPlayerToFollow->getCenterX(),
            playerToFollow->getCenterY() - secondPlayerToFollow->getCenterY()
        );

        if (distance < window.getSize().x ) {
            if (split) {

            }

            if (currentSplit != 0.0f) {
                animateSplit(window, 0.0f);
            }

            target.x = (playerToFollow->getCenterX() + secondPlayerToFollow->getCenterX()) / 2;
            target.y = (playerToFollow->getCenterY() + secondPlayerToFollow->getCenterY()) / 2;

            smoothMoveToTarget(splitLeftView, target);


        } else {

            if (split == false) {
                if (playerToFollow->getCenterX() < secondPlayerToFollow->getCenterX()) {
                    leftTarget = playerToFollow;
                    rightTarget = secondPlayerToFollow;
                } else {
                    leftTarget = secondPlayerToFollow;
                    rightTarget = playerToFollow;
                }
                split = true;
            }

            smoothMoveToTarget(splitLeftView, sf::Vector2f{leftTarget->getCenterX(), leftTarget->getCenterY()});
            smoothMoveToTarget(splitRightView, sf::Vector2f{rightTarget->getCenterX(), rightTarget->getCenterY()});

            animateSplit(window, 0.5f);
        }
    }


}

void Camera::animateSplit(sf::RenderWindow& window, float targetValue) {
    float targetSplit = targetValue;
    float delta = 0.1f;

    if (currentSplit < targetSplit && targetValue > 0.0f && currentSplit < targetSplit) {
        currentSplit = Math::lerp(currentSplit, targetSplit, delta * 0.4);
        if (currentSplit > targetSplit - 0.0001) {
            currentSplit = targetSplit;
        }
    } else if (currentSplit > targetSplit) {
        currentSplit = Math::lerp(currentSplit, targetSplit, delta * 0.7);
        if (currentSplit < 0.0001) {
            currentSplit = 0.0f;
            split = false;
        }
    }

    splitLeftView.setViewport(sf::FloatRect(0, 0, 1.0f - currentSplit, 1));
    splitRightView.setViewport(sf::FloatRect(1.0f - currentSplit, 0, currentSplit, 1));


    splitLeftView.setSize(window.getSize().x * (1.0f - currentSplit), window.getSize().y);
    splitRightView.setSize(window.getSize().x * currentSplit, window.getSize().y);
}

void Camera::smoothMoveToTarget(sf::View &view, sf::Vector2f target) {
    float delta = 0.2f;
    sf::Vector2f movement = sf::Vector2f(target.x, target.y) - view.getCenter();

    if (boundary.minX + boundary.maxX > view.getSize().x && boundary.minY + boundary.maxY > view.getSize().y) {
        if (snapToEdges) {
            if (target.x - view.getSize().x / 2 < boundary.minX) {
                movement.x = boundary.minX + view.getSize().x / 2 - view.getCenter().x;
            }
            if (target.x + view.getSize().x / 2 > boundary.maxX) {
                movement.x = boundary.maxX - view.getSize().x / 2 - view.getCenter().x;
            }
            if (target.y - view.getSize().y / 2 < boundary.minY) {
                movement.y = boundary.minY + view.getSize().y / 2 - view.getCenter().y;
            }
            if (target.y + view.getSize().y / 2 > boundary.maxY) {
                movement.y = boundary.maxY - view.getSize().y / 2 - view.getCenter().y;
            }
        }
    }

    view.move(movement * delta);
}

void Camera::setPlayerToFollow(Player *playerToFollow) {
    this->playerToFollow = playerToFollow;
}

void Camera::setSecondPlayerToFollow(Player *secondPlayerToFollow) {
    this->secondPlayerToFollow = secondPlayerToFollow;
}

void Camera::setWorldBoundary(WorldBoundary boundary) {
    this->boundary = boundary;
}

sf::View &Camera::getSplitViewLeft() {
    return splitLeftView;
}

sf::View &Camera::getSplitViewRight() {
    return splitRightView;
}

CameraMode Camera::getCameraMode() {
    return cameraMode;
}

bool Camera::isSplit() {
    return this->split;
}

float Camera::getOffsetY() {
    return offsetY;
}
