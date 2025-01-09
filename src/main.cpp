#include <iostream>
#include <SFML/Graphics.hpp>

#include "Button.h"
#include "Camera.h"
#include "GameManager.h"
#include "GuideArrow.h"
#include "Interface.h"
#include "Physics.h"
#include "Player.h"
#include "WorldGenerator.h"
#include "Signals.h"

int main() {
    int width = 900;
    int height = 740;

    sf::RenderWindow window(sf::VideoMode(width, height), "2DGK");

    window.setKeyRepeatEnabled(false);

    GameManager gameManager;

    Camera camera(window, FOLLOW_ONE_PLAYER);

    sf::Texture circleTexture;
    sf::Texture dogTexture;

    sf::Texture arrowTexture;


    //loading textures
    if (!circleTexture.loadFromFile("data/img/circle.png"))
    {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }

    if (!dogTexture.loadFromFile("data/img/dog.png"))
    {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }

    if (!arrowTexture.loadFromFile("data/img/arrow.png"))
    {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }

    sf::Color bgColor = sf::Color(3,78, 128);

    Player *player1 = new Player(0, KEYBOARD_WASD, 100, 100, dogTexture);
    Player *player2 = new Player(1, KEYBOARD_ARROWS, 400, 400, circleTexture);

    player1->setCollisionShape(BOX);
    player2->setCircleCollisionShape(player2->getSize()/2);


    auto* guideArrow0 = new GuideArrow(&arrowTexture);

    auto* guideArrow1 = new GuideArrow(&arrowTexture);


    camera.setPlayerToFollow(player1);
    camera.setSecondPlayerToFollow(player2);

    WorldGenerator* worldGenerator = new WorldGenerator("data/level/platformer1.txt", "data/level/blocks.txt");
    worldGenerator->loadData();
    worldGenerator->generateWorld();

    gameManager.setWorldGenerator(worldGenerator);

    camera.setWorldBoundary(worldGenerator->getBoundary());

    Physics* physics = new Physics();

    gameManager.setPhysicsPointer(physics);

    physics->addPlayer(player1);
    physics->addPlayer(player2);

    gameManager.setBlocksForPhysics();

    // for (int i = 0; i < 20; i++) {
    //     Circle* circle = new Circle(i, 20, worldGenerator.getBoundary());
    //     circle->setRandomPosition();
    //     circle->setRandomVelocity();
    //     circle->setRandomColor();
    //     physics->addCircle(circle);
    // }

    //testing
    // Circle* c_0 = new Circle(1, sf::Vector2f(50, 70), 20, worldGenerator.getBoundary());
    // c_0->setVelocity(2, 0);
    // Circle* c_1 = new Circle(2, sf::Vector2f(250, 100), 20, worldGenerator.getBoundary());
    // c_1->setVelocity(-2, 0);
    // physics->addCircle(c_0);
    // physics->addCircle(c_1);

    Circle* circle1 = new Circle(99, sf::Vector2f(worldGenerator->getBoundary().minX, worldGenerator->getBoundary().maxY), 10, worldGenerator->getBoundary());
    circle1->shape.setFillColor(sf::Color::Red);

    physics->setBoundaries(worldGenerator->getBoundary());

    sf::Font font;
    if (!font.loadFromFile("data/fonts/Roboto-Regular.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    // Create a button.
    Button* b_toggleSeparation = new Button(TOGGLE,100, 50, 140, 50, "Separate circles", font, [physics]() {
        physics->toggleSeparation();
    });
    b_toggleSeparation->setToggled(false);

    // Create a button.
    Button* b_toggleCircles = new Button(TOGGLE, 250, 50, 120, 50, "Collision with\ncircles", font, [physics]() {
        physics->toggleCollisionWithCircles();
    });
    b_toggleCircles->setToggled(true);

    Interface* interface = new Interface(&font);
    interface->addButton(b_toggleSeparation);
    interface->addButton(b_toggleCircles);

    Signals::SetPositionForPlayerId.emit(0, gameManager.getPlayer1SpawnPos());
    Signals::SetPositionForPlayerId.emit(1, gameManager.getPlayer2SpawnPos());

    sf::Clock clock;
    auto* t = new sf::Text("Game Over", font, 30);
    t->setPosition({100, 100});
    t->setFillColor(sf::Color::Black);

    // interface->setClock(&clock);
    // sf::Vector2f target = worldGenerator->trapdoorPosition;

    std::vector<Player*> players = {player1, player2};

    float delta = 0.0f;
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }

            camera.processCameraInput(event);

            player1->oneShotInput(event);
            // player2->oneShotInput(event);
        }
        player1->input(event);
        // player2->input(event);

        delta = clock.restart().asSeconds();

        int views = camera.isSplit() ? 2 : 1;

        camera.update(window);

        //update players
        player1->update(window, delta);
        // player2->update(window, delta);

        for(int i = 0; i < physics->circles.size(); i++) {
            physics->circles[i]->update();
        }

        //circles collision
        physics->checkCollision(WALLS);
        physics->checkCollision(CIRCLES);
        physics->checkCollisionForPlayersWithBlocks();

        if (player1->y > 2500) {
            Signals::SetPositionForPlayerId.emit(0, gameManager.getPlayer1SpawnPos());
        }

        //update buttons
        for (auto i = 0; i < interface->buttons.size(); i++) {
            auto b = interface->buttons[i];
            b->handleEvent(event, window);
            b->updatePosition(camera.getView());
        }
        interface->updatePosition(camera.getView(), window);
        guideArrow0->update({player1->getCenterX(), player1->getCenterY()}, worldGenerator->trapdoorPosition);
        // guideArrow1->update({player2->getCenterX(), player2->getCenterY()}, worldGenerator->trapdoorPosition);


        for (int i = 0; i < views; i++) {

            if (camera.isSplit()) {
                window.setView(i == 0 ? camera.getSplitViewLeft() : camera.getSplitViewRight());
            } else {
                window.setView(camera.getSplitViewLeft());
            }

            worldGenerator->drawWorld(window);

            window.draw(player1->sprite);
            // window.draw(player2->sprite);

            interface->draw(window);

            guideArrow0->draw(window);
            // guideArrow1->draw(window);

            for (int j = 0; j < physics->circles.size(); j++) {
                window.draw(physics->circles[j]->shape);
            }

        }

        window.setFramerateLimit(120);
        window.display();
        window.clear(bgColor);

    }
}
