//
// Created by Hubert Klonowski on 11/12/2024.
//

#include "Signals.h"

//gamemanager
Signal<int> Signals::NextStage;
Signal<int, sf::Vector2f> Signals::SetSpawnPoint;

//player
Signal<int, sf::Vector2f> Signals::SetPositionForPlayerId;

//ui
Signal<std::string> Signals::ShowMessage;

//camera
Signal<WorldBoundary> Signals::SetBoundary;

//points
Signal<> Signals::IncreasePoint;
Signal<int> Signals::UpdatePoints;
Signal<int> Signals::SetMaxPoints;

//delete __block
Signal<CollisionShape*> Signals::DeleteBlock;