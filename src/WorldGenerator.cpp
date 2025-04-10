//
// Created by Hubert Klonowski on 17/10/2024.
//

#include "WorldGenerator.h"

#include <fstream>
#include <iostream>
#include <random>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/FileInputStream.hpp>

#include "Block.h"
#include "Signals.h"

WorldGenerator::WorldGenerator(std::string mapLayoutPath, std::string blocksPath, sf::Texture* spriteSheet) {
    this->mapLayoutPath = mapLayoutPath;
    this->blocksPath = blocksPath;
    this->spriteSheet = spriteSheet;

    Signals::DeleteBlock.connect([this](CollisionShape* shape) {DeleteBlock(shape);});
}

WorldGenerator::~WorldGenerator() {

}

void WorldGenerator::getBlocksData() {
    std::ifstream blocksFile(blocksPath);

    char delimeter = ';';

    std::cout << "Reading blocks data from " << blocksPath << std::endl;

    std::string line;
    int i = 0;

    std::vector<std::string> block;

    while(getline(blocksFile, line)) {
        if (line.empty())
            continue;

        block.push_back(line);

        i++;
        if (i == 6) {
            i = 0;
            // Find the position of the comma
            size_t delimiterPos = block[4].find(',');
            std::string part1;
            std::string part2;
            // Check if the delimiter exists in the string
            if (delimiterPos != std::string::npos) {
                // Split the string into two parts
                part1 = block[4].substr(0, delimiterPos);  // From the start to the comma
                part2 = block[4].substr(delimiterPos + 1);  // From the comma to the end

            } else {
                std::cout << "No delimiter found!" << std::endl;
            }
            BlockData data(block[0], block[1], std::stof( block[2]), std::stof(block[3]), {std::stoi(part1), std::stoi(part2)}, block[5] == "true");
            blocksData.push_back(data);
            block.clear();
        }
    }

    for (unsigned int i = 0; i < blocksData.size(); i++) {
        this->textures.insert(std::make_pair(blocksData[i].character, blocksData[i].textureCoords));
    }

    blocksFile.close();
}

void WorldGenerator::updateBoundary(float x, float y) {
    if (x > boundary.maxX)
        boundary.maxX = x;
    if (y > boundary.maxY)
        boundary.maxY = y;
}



void WorldGenerator::createBlock(std::string line, int lineIndex) {
    int charIndex = 0;
    for(char& c : line) {
        std::string symbol(1, c);
        // sf::Texture *texture = this->textures[symbol];
        sf::Vector2i textureCoords = this->textures[symbol];

        BlockData data = getDataForChar(symbol);

        // float size = data.sizeX;
        if (data.name == "air") {
            charIndex++;
            continue;
        }

        float size = 70;
        // sf::RectangleShape block;
        // block.setSize(sf::Vector2f(size, size));
        // block.setPosition(size * charIndex, size * lineIndex);

        Block block(data.name == "trapdoor" ? this->textures["."] : textureCoords, spriteSheet);
        block.setSize(size, size);
        block.setTextureSize(data.sizeX, data.sizeY);
        block.setPosition(std::round(size * charIndex), std::round(size * lineIndex));
        block.setCollidable(data.name == "trapdoor" ? false : data.collidable);
        block.setName(data.name);
        if (data.collidable) {
            block.createCollisionShape();
            // if (data.name == "trapdoor")
            //     block.setTriggerOnCollisionShape(NEXT_STAGE);
        }

        // if (data.name == "spawnpoint0") {
        //     Signals::SetSpawnPoint.emit(0, {block.getPosition().x + block.getSize().x/2, block.getPosition().y + block.getSize().y/2});
        // }
        //
        // if (data.name == "spawnpoint1") {
        //     Signals::SetSpawnPoint.emit(1, {block.getPosition().x + block.getSize().x/2, block.getPosition().y + block.getSize().y/2});
        // }

        updateBoundary(block.getPosition().x + size, block.getPosition().y + size);

        // sf::Texture texture;
        // texture.loadFromFile(texturesFolderPath + textureName);
        // block.setTexture(texture);
        blocks.push_back(block);
        charIndex++;
    }
}

// void WorldGenerator::loadTextures() {
//     for (const auto& pair : texturesData) {
//         std::string symbol = pair.first;
//         std::string texturePath = pair.second;
//
//         // sf::Texture *texture = new sf::Texture();
//         // if (!texture->loadFromFile(texturePath)) {
//         //     std::cout << "Failed to load texture: " <<  texturePath << std::endl;
//         //     continue;
//         // }
//
//         textures[symbol] = texture;
//     }
// }

void WorldGenerator::loadData() {
    this->getBlocksData();
    // this->loadTextures();
}

void WorldGenerator::generateWorld() {
    blocks.clear();
    boundary.reset();
    std::ifstream layoutFile(mapLayoutPath);

    std::string line;
    int lineIndex = 0;
    while(getline(layoutFile, line)) {
        createBlock(line, lineIndex);
        lineIndex++;
    }

    layoutFile.close();

    std::vector<int> spawnPoints0;
    std::vector<int> spawnPoints1;
    std::vector<int> trapdoorPos;
    std::vector<int> points = {};

    for(int i = 0; i < blocks.size(); i++) {
        if (blocks[i].getName() == "trapdoor")
            trapdoorPos.push_back(i);
        if (blocks[i].getName() == "spawnpoint0")
            spawnPoints0.push_back(i);
        if (blocks[i].getName() == "spawnpoint1")
            spawnPoints1.push_back(i);
        if (blocks[i].getName() == "pink_feather") {
            points.push_back(i);
        }
    }

    for (auto point : points) {
        blocks[point].setCollidable(true);
        blocks[point].createCollisionShape();
        blocks[point].setTriggerOnCollisionShape(POINT);
    }

    Signals::SetMaxPoints.emit(points.size());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, spawnPoints0.size() - 1);

    int randomSpawnPoint0Index = spawnPoints0[dis(gen)];

    sf::Vector2f sp0 = {blocks[randomSpawnPoint0Index].getPosition().x + blocks[randomSpawnPoint0Index].getSize().x/2, blocks[randomSpawnPoint0Index].getPosition().y + blocks[randomSpawnPoint0Index].getSize().y/2};

    Signals::SetSpawnPoint.emit(0, sp0);

    std::uniform_int_distribution<> dis1(0, spawnPoints1.size() - 1);

    int randomSpawnPoint1Index = spawnPoints1[dis1(gen)];
    sf::Vector2f sp1 = {blocks[randomSpawnPoint1Index].getPosition().x + blocks[randomSpawnPoint1Index].getSize().x/2, blocks[randomSpawnPoint1Index].getPosition().y + blocks[randomSpawnPoint1Index].getSize().y/2};

    Signals::SetSpawnPoint.emit(1, sp1);

    Signals::SetPositionForPlayerId.emit(0, sp0);
    Signals::SetPositionForPlayerId.emit(1, sp1);
    //
    // Signals::SetPositionForPlayerId.emit(0, {blocks[randomSpawnPoint0Index].getPosition().x + blocks[randomSpawnPoint0Index].getSize().x/2, blocks[randomSpawnPoint0Index].getPosition().y + blocks[randomSpawnPoint0Index].getSize().y/2});
    // Signals::SetPositionForPlayerId.emit(1, {blocks[randomSpawnPoint1Index].getPosition().x + blocks[randomSpawnPoint1Index].getSize().x/2, blocks[randomSpawnPoint1Index].getPosition().y + blocks[randomSpawnPoint1Index].getSize().y/2});

    std::uniform_int_distribution<> dis2(0, trapdoorPos.size() - 1);

    if (!trapdoorPos.empty()) {
        int randomTrapdoorIndex = trapdoorPos[dis2(gen)];

        blocks[randomTrapdoorIndex].setCollidable(true);
        blocks[randomTrapdoorIndex].createCollisionShape();
        blocks[randomTrapdoorIndex].setTriggerOnCollisionShape(NEXT_STAGE);
        blocks[randomTrapdoorIndex].setTexture(this->textures["$"]);

        this->trapdoorPosition = {blocks[randomTrapdoorIndex].getPosition().x + blocks[randomTrapdoorIndex].getSize().x/2, blocks[randomTrapdoorIndex].getPosition().y + blocks[randomTrapdoorIndex].getSize().y/2};
    }
    std::cout << "Generating world..." << std::endl;

    Signals::SetBoundary.emit(this->getBoundary());
}

void WorldGenerator::drawWorld(sf::RenderWindow &window) {

    for(Block block : blocks) {
        if (block.getName() == "spawnpoint0" || block.getName() == "spawnpoint1" || block.getName() == "air") {
            continue;
        }
        window.draw(block.shape);
    }
}


WorldBoundary WorldGenerator::getBoundary() {
    return boundary;
}

BlockData WorldGenerator::getDataForChar(std::string c) {
    for (int i = 0; i < blocksData.size(); i++) {
        if (blocksData[i].character == c) {
            return blocksData[i];
        }
    }
    return blocksData[0];
}

void WorldGenerator::setMapLayoutPath(std::string mapLayoutPath) {
    this->mapLayoutPath = mapLayoutPath;
}

void WorldGenerator::DeleteBlock(CollisionShape *shape) {
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].collisionShape == shape) {
            blocks.erase(blocks.begin() + i);
        }
    }
}
