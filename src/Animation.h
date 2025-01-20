//
// Created by Hubert Klonowski on 20/01/2025.
//

#ifndef ANIMATION_H
#define ANIMATION_H
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>

enum PlayStyle {
    PS_NORMAL,
    PS_LOOP,
    PS_LOOP_REVERSE,
    PS_PINGPONG,
    PS_REVERSE,
    PS_PINGPONG_REVERSE,
};

class Animation {
    sf::RectangleShape* shape = nullptr;

    sf::Texture* spriteSheet = nullptr;

    sf::Vector2i frameSize = {32, 32};

    float currentTime = 0.0f;

    float duration = 0.0f;

    std::vector<sf::Vector2i> framePositions;

    unsigned int currentFrame = 0;

    bool playing = false;

    PlayStyle playStyle = PS_NORMAL;

    void updateFrame(bool firstFrame = false) {
        if (currentFrame >= framePositions.size() || currentFrame < 0) {
            currentFrame = 0;
        }
        sf::Vector2i position = framePositions[firstFrame ? 0 : currentFrame];
        shape->setTextureRect(sf::IntRect(position.x * frameSize.x, position.y * frameSize.y, frameSize.x, frameSize.y));
    }

    int direction = 1;

    int playCount = 0;

public:

    Animation(sf::Texture* spriteSheet, sf::RectangleShape* shape) {
        this->spriteSheet = spriteSheet;
        this->shape = shape;
        shape->setTexture(spriteSheet);
    }

    void Update(float deltaTime) {
        if (!playing) return;
        if (playStyle == PS_NORMAL && playCount > 0) return;

        currentTime += deltaTime;

        if (currentTime >= duration) {
            currentTime = 0.0f;
            switch (playStyle) {
                case PS_NORMAL:
                    currentFrame = (currentFrame + 1) % framePositions.size();
                    if (currentFrame == framePositions.size() - 1) {
                        Stop();
                        playCount++;
                    }
                    break;
                case PS_LOOP:
                    currentFrame = (currentFrame + 1) % framePositions.size();
                    break;
                case PS_PINGPONG:
                    currentFrame += direction;
                    if (currentFrame >= framePositions.size() - 1 || currentFrame <= 0) {
                        direction *= -1;
                    }
                    break;
                case PS_PINGPONG_REVERSE:
                    currentFrame += direction;
                    if (currentFrame >= framePositions.size() - 1 || currentFrame <= 0) {
                        direction *= -1;
                    }
                    break;
                case PS_REVERSE:
                    currentFrame = currentFrame <= 0 ? framePositions.size() - 1 : currentFrame - 1;
                    break;
                default:
                    currentFrame = (currentFrame + 1) % framePositions.size();
                    break;
            }
            // std::cout << "Frame: " << currentFrame << std::endl;
            updateFrame();
        }
    }

    void SetFirstFrame() {
        updateFrame(true);
    }

    void AddFrame(sf::Vector2i position) {
        framePositions.push_back(position);
    }

    void SetFramePositions(std::vector<sf::Vector2i> positions) {
        this->framePositions = positions;
    }

    void SetFrameSize(sf::Vector2i frameSize) {
        this->frameSize = frameSize;
    }

    void SetDuration(float duration) {
        this->duration = duration;
    }

    void Play() {
        if (playStyle == PS_REVERSE || playStyle == PS_PINGPONG_REVERSE) {
            currentFrame = framePositions.size() - 1;
            direction = -1;
        } else
            currentFrame = 0;
        currentTime = 0.0f;
        playCount = 0;
        playing = true;
        updateFrame();
    }

    void Stop() {
        playing = false;
    }

    bool IsPlaying() {
        return playing;
    }

    void SetPlayStyle(PlayStyle playStyle) {
        this->playStyle = playStyle;
    }

    PlayStyle GetPlayStyle() {
        return playStyle;
    }

};

#endif //ANIMATION_H
