//
// Created by Hubert Klonowski on 20/01/2025.
//

#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Camera.h"

class Layer {
public:
    sf::Texture* texture;
    sf::Sprite sprite;
    float speed;

    Layer(sf::Texture* texture, sf::Vector2f size, float scale = 1.0f) {
        this->texture = texture;
        texture->setRepeated(true);
        // texture->setSmooth(true);
        sprite.setTexture(*texture);
        sprite.setScale(scale, scale);
        UpdateTexture(size);
    }

    void UpdateTexture(sf::Vector2f size) {
        sprite.setTextureRect(sf::IntRect(-10, -10, size.x, size.y));
    }
};

class Background {
    std::vector<Layer> layers;
    float scrollSpeed = 0.5f;

    Camera* camera;

    sf::Vector2f camStartPos;

    sf::Vector2f position;

public:

    Background(Camera* camera) {
        this->camera = camera;
        camStartPos = camera->getPosition();
    }

    ~Background() {}

    void AddLayer(const std::string texturePath, float scale = 1.0f) {
        sf::Texture* texture = new sf::Texture();
        texture->loadFromFile(texturePath);
        Layer layer(texture, camera->getView().getSize(), scale);
        int i = layers.size();
        layer.speed = 0.1f * (i + 1);
        layers.push_back(layer);
    }

    void Update(float deltaTime, sf::RenderWindow& window) {

        float distance = camera->getPosition().x - camStartPos.x;

        position = {camera->getPosition().x - camera->getView().getSize().x/2 - 20, camera->getPosition().y - camera->getView().getSize().y/2 };

        for (auto& layer : layers) {
            float speed = layer.speed * scrollSpeed;

            // Calculate texture offset
            float textureOffsetX = std::fmod(distance * speed, layer.texture->getSize().x);

            // Update textureRect to scroll
            layer.sprite.setTextureRect(sf::IntRect(
                static_cast<int>(textureOffsetX), -20,
                camera->getView().getSize().x + 500, camera->getView().getSize().y - camera->getOffsetY() + 20
            ));

            // Update sprite position
            layer.sprite.setPosition(position);
        }
    }

    void Draw(sf::RenderWindow& window) {
        for (auto& layer : layers) {
            window.draw(layer.sprite);
        }
    }
};

#endif //BACKGROUND_H
