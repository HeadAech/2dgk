//
// Created by Hubert Klonowski on 11/12/2024.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <iostream>
#include <ostream>

#include "Physics.h"
#include "Signals.h"
#include "WorldGenerator.h"

class GameManager {
private:

    sf::Vector2f player1SpawnPos;
    sf::Vector2f player2SpawnPos;

    WorldGenerator* worldGenerator;
    Physics* physics;

    int labIndex = 1;
    int maxLevel = 3;

    std::map<int, int> stats;

    public:

    GameManager() {
        Signals::NextStage.connect([this] (int playerId) {this->onNextStage(playerId);});
        Signals::SetSpawnPoint.connect([this] (int num, sf::Vector2f pos) {this->onSetSpawnPoint(num, pos);});
    }

    void setWorldGenerator(WorldGenerator* w) {
        this->worldGenerator = w;
    }

    void setPhysicsPointer(Physics* p) {
        this->physics = p;
    }

    void onNextStage(int playerId) {
        std::cout << "onNextStage" << std::endl;
        Signals::ShowMessage.emit("Player " + std::to_string(playerId) + " reached the trapdoor!");
        stats[playerId]++;
        labIndex++;
        std::cout << labIndex << std::endl;

        if (maxLevel < labIndex) {
            Signals::ShowMessage.emit("GG!\nPlayer 0 reached the trapdoor " + std::to_string(stats[0]) + " times. \nPlayer 1 reached the trapdoor " + std::to_string(stats[1]) + " times.");
            labIndex = 1;
            stats[0] = 0;
            stats[1] = 0;

        }

        worldGenerator->setMapLayoutPath("data/level/labyrinth" + std::to_string(labIndex) + ".txt");
        worldGenerator->generateWorld();
        setBlocksForPhysics();

    }

    void onSetSpawnPoint(int num, sf::Vector2f pos) {
        if (num == 0) {
            player1SpawnPos = pos;
        }
        if (num == 1) {
            player2SpawnPos = pos;
        }
    }

    sf::Vector2f getPlayer1SpawnPos() {
        return player1SpawnPos;
    }

    sf::Vector2f getPlayer2SpawnPos() {
        return player2SpawnPos;
    }

    void setBlocksForPhysics() {
        physics->clearBlocks();
        for (Block block : worldGenerator->blocks) {
            if (block.collidable)
                physics->addBlock(block.collisionShape);
        }
    }
};

#endif //GAMEMANAGER_H
