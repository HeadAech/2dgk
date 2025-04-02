//
// Created by Hubert Klonowski on 20/01/2025.
//

#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <fstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Camera.h"

class Layer {
public:
    sf::Texture* texture;
    sf::Sprite sprite;
    float speed;

    bool lockY = false;

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
    std::vector<std::vector<Block>> backgroundBlocks = {
        {},
        {},
        {},
        {},
        {},
        {}
    };

    WorldGenerator* worldGenerator;

public:

    Background(Camera* camera) {
        this->camera = camera;
        camStartPos = camera->getPosition();
    }

    ~Background() {}

    void ReadBackgroundFile(std::string path, int layerIdx) {
        std::ifstream layoutFile(path);

        std::string line;
        int lineIndex = 0;
        while(getline(layoutFile, line)) {
            createBlock(line, lineIndex, layerIdx);
            lineIndex++;
        }

        layoutFile.close();
    }

    void createBlock(std::string line, int lineIndex, int layerIdx) {
        int charIndex = 0;
        for(char& c : line) {
            std::string symbol(1, c);
            // sf::Texture *texture = this->textures[symbol];
            sf::Vector2i textureCoords = worldGenerator->textures[symbol];

            BlockData data = worldGenerator->getDataForChar(symbol);

            // float size = data.sizeX;
            if (data.name == "air") {
                charIndex++;
                continue;
            }

            float size = 70;

            Block block(data.name == "trapdoor" ? worldGenerator->textures["."] : textureCoords, worldGenerator->spriteSheet);
            block.setSize(size, size);
            block.setTextureSize(data.sizeX, data.sizeY);
            block.setPosition(std::round(size * charIndex), std::round(size * lineIndex));
            block.setCollidable(false);
            block.setName(data.name);

            backgroundBlocks.at(layerIdx).push_back(block);
            charIndex++;
        }
    }

    void AddLayerFromBlockLayout(int layerIdx, sf::Vector3f tint, float scale = 1.0f, bool lockY = false) {
        sf::RenderTexture renderTexture;
        renderTexture.create(1400, 5500);
        renderTexture.clear(sf::Color::Transparent);
        for (Block block : backgroundBlocks[layerIdx]) {
            block.shape.setFillColor(sf::Color(tint.x, tint.y, tint.z));
            renderTexture.draw(block.shape);
        }
        renderTexture.display();
        sf::Texture* texture =  new sf::Texture(renderTexture.getTexture());
        AddLayer(texture, scale, lockY);
    }

    void AddLayer(const std::string texturePath, float scale = 1.0f, bool lockY = false) {
        sf::Texture* texture = new sf::Texture();
        texture->loadFromFile(texturePath);
        Layer layer(texture, camera->getView().getSize(), scale);
        layer.lockY = lockY;
        int i = layers.size();
        layer.speed = 0.1f * (i + 1);
        layers.push_back(layer);
    }

    void AddLayer(sf::Texture* texture, float scale = 1.0f, bool lockY = false) {
        Layer layer(texture, camera->getView().getSize(), scale);
        int i = layers.size();
        layer.speed = 0.1f * (i + 1);
        layer.lockY = lockY;
        layers.push_back(layer);
    }

    void Update(float deltaTime, sf::RenderWindow& window) {

        float distance = camera->getPosition().x - camStartPos.x;

        position = {camera->getPosition().x - camera->getView().getSize().x/2 - 20, camera->getPosition().y - camera->getView().getSize().y/2 };

        for (auto& layer : layers) {
            float speed = layer.speed * scrollSpeed;

            // Calculate texture offset
            float textureOffsetX = std::fmod(distance * speed, layer.texture->getSize().x);

            // Adjust position with light reaction to Y-coordinate changes
            float layerY = position.y; // Default position

            if (layer.lockY) {
                float reactionFactor = 0.2f; // Adjust this factor to control the intensity
                layerY -= (camera->getPosition().y - camStartPos.y) * reactionFactor;
            }

            // Update textureRect to scroll
            layer.sprite.setTextureRect(sf::IntRect(
                static_cast<int>(textureOffsetX), -20,
                camera->getView().getSize().x + 500, camera->getView().getSize().y - camera->getOffsetY() + 20
            ));

            layer.sprite.setPosition(position.x, layerY);
        }
    }

    void Draw(sf::RenderWindow& window) {
        for (auto& layer : layers) {
            window.draw(layer.sprite);
        }
    }

    void IncreaseLayerSpeed(int i) {
        layers.at(i).speed += 0.1f;
    }

    void DecreaseLayerSpeed(int i) {
        layers.at(i).speed -= 0.1f;
    }

    float GetScrollSpeed(int i) {
        return layers.at(i).speed;
    }

    void IncreaseScrollSpeed() {
        scrollSpeed += 0.1f;
    }

    void DecreaseScrollSpeed() {
        scrollSpeed -= 0.1f;
    }

    float GetScrollSpeed() {
        return scrollSpeed;
    }

    void SetWorldGenerator(WorldGenerator* worldGenerator) {
        this->worldGenerator = worldGenerator;
    }
};

#endif //BACKGROUND_H
