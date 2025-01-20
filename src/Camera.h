//
// Created by Hubert Klonowski on 24/10/2024.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "Player.h"
#include "WorldGenerator.h"

enum CameraMode {
    FOLLOW_ONE_PLAYER,
    FOLLOW_TWO_PLAYERS
};

class Camera {
private:
    WorldBoundary boundary;

    // sf::View view;
    sf::View splitLeftView;
    sf::View splitRightView;
    Player* leftTarget;
    Player* rightTarget;

    Player *playerToFollow;

    Player *secondPlayerToFollow;

    CameraMode cameraMode;

    float currentSplit = 0.0f;

    bool snapToEdges = true;

    int maxZoom = 3;
    int currentZoom = 0;
    int minZoom = -3;
    float zoomFactor = 0.8f;

    float offsetY = -80.0f;

    bool split = false;

    public:
    Camera(sf::RenderWindow& window, CameraMode cameraMode, Player* playerToFollow);
    explicit Camera(sf::RenderWindow& window, CameraMode cameraMode);
    ~Camera();

    void update(sf::RenderWindow& window);

    void animateSplit(sf::RenderWindow& window, float targetValue);

    void smoothMoveToTarget(sf::View &view, sf::Vector2f target);

    sf::Vector2f getPosition();

    sf::View& getView();

    sf::View& getSplitViewLeft();
    sf::View& getSplitViewRight();

    void processCameraInput(sf::Event &event);
    void setWorldBoundary(WorldBoundary boundary);
    CameraMode getCameraMode();

    bool isSplit();

    Player& getFirstPlayerOOB();

    // setters

    void setPlayerToFollow(Player* playerToFollow);
    void setSecondPlayerToFollow(Player* secondPlayerToFollow);

    float getOffsetY();
};

#endif //CAMERA_H
