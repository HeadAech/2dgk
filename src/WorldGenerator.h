//
// Created by Hubert Klonowski on 17/10/2024.
//

#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H
#include <map>
#include <string>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Block.h"


class Block;

namespace sf {
    class RenderWindow;
}

struct WorldBoundary {
    float minX;
    float minY;
    float maxX;
    float maxY;

    WorldBoundary() {
        this->minX = 0;
        this->minY = 0;
        this->maxX = 0;
        this->maxY = 0;
    }

    void reset() {
        this->minX = 0;
        this->minY = 0;
        this->maxX = 0;
        this->maxY = 0;
    }
};

class WorldGenerator {

private:
    WorldBoundary boundary;

    std::string mapLayoutPath;
    std::string blocksPath;
    std::string texturesFolderPath = "data/textures/";


    void createBlock(std::string line, int lineIndex);

    sf::Texture* spriteSheet = nullptr;

    std::map<std::string, sf::Vector2i> textures;

    std::vector<BlockData> blocksData;
    std::map<std::string, std::string> texturesData;

    void getBlocksData();
    void loadTextures();

    void updateBoundary(float x, float y);

public:
    std::vector <Block> blocks;

    sf::Vector2f trapdoorPosition;

    WorldGenerator(std::string mapLayoutPath, std::string blocksPath, sf::Texture* spriteSheet);
    ~WorldGenerator();

    void generateWorld();
    void drawWorld(sf::RenderWindow& window);
    void loadData();

    BlockData getDataForChar(std::string c);

    float getSizeX();
    float getSizeY();

    WorldBoundary getBoundary();

    void setMapLayoutPath(std::string mapLayoutPath);

    void DeleteBlock(CollisionShape* shape);
};



#endif //WORLDGENERATOR_H
