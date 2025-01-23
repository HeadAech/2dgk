#include <iostream>
#include <SFML/Graphics.hpp>

#include "Background.h"
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

    sf::Texture spriteSheet;


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

    if (!spriteSheet.loadFromFile("data/textures/pairrots_spritesheet.png")) {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }

    sf::Color bgColor = sf::Color(3,78, 128);

    Player *player1 = new Player(0, KEYBOARD_WASD, 100, 100, dogTexture);
    Player *player2 = new Player(1, KEYBOARD_ARROWS, 400, 400, circleTexture);

    player1->setCollisionShape(BOX);
    player2->setCircleCollisionShape(player2->getSize()/2);

    Animation pinkParrotAnimationIdle(&spriteSheet, &player1->sprite);
    std::vector<sf::Vector2i> pinkParrotFramesIdle = {
        {0,0},
        {1,0}
    };
    pinkParrotAnimationIdle.SetFramePositions(pinkParrotFramesIdle);
    pinkParrotAnimationIdle.SetDuration(0.7f);
    pinkParrotAnimationIdle.SetPlayStyle(PS_LOOP);
    pinkParrotAnimationIdle.Play();
    player1->AddAnimation(pinkParrotAnimationIdle);

    Animation pinkParrotAnimationFly(&spriteSheet, &player1->sprite);
    std::vector<sf::Vector2i> pinkParrotFramesFly = {
        {2,0},
        {3,0},
        {4,0},
        {0, 1},
        {1,1},
        {2,1},
        {3,1},
    };
    pinkParrotAnimationFly.SetFramePositions(pinkParrotFramesFly);
    pinkParrotAnimationFly.SetDuration(0.04f);
    pinkParrotAnimationFly.SetPlayStyle(PS_PINGPONG_REVERSE);
    player1->AddAnimation(pinkParrotAnimationFly);

    Animation pinkParrotAnimationFall(&spriteSheet, &player1->sprite);
    std::vector<sf::Vector2i> pinkParrotFramesFall = {
        {2,0},
        {3,0},
        {4,0},
        {0, 1},
        {1,1},
        {2,1},
        {3,1},
    };
    pinkParrotAnimationFall.SetFramePositions(pinkParrotFramesFly);
    pinkParrotAnimationFall.SetDuration(0.055f);
    pinkParrotAnimationFall.SetPlayStyle(PS_NORMAL);
    player1->AddAnimation(pinkParrotAnimationFall);

    Animation pinkParrotWalk(&spriteSheet, &player1->sprite);
    std::vector<sf::Vector2i> pinkParrotFramesWalk = {
        {2,4},
        {3,4},
    };
    pinkParrotWalk.SetFramePositions(pinkParrotFramesWalk);
    pinkParrotWalk.SetDuration(0.1f);
    pinkParrotWalk.SetPlayStyle(PS_LOOP);
    player1->AddAnimation(pinkParrotWalk);

    auto* guideArrow0 = new GuideArrow(&arrowTexture);

    auto* guideArrow1 = new GuideArrow(&arrowTexture);


    camera.setPlayerToFollow(player1);
    camera.setSecondPlayerToFollow(player2);

    WorldGenerator* worldGenerator = new WorldGenerator("data/level/test1.txt", "data/level/blocks.txt", &spriteSheet);
    worldGenerator->loadData();
    worldGenerator->generateWorld();

    gameManager.setWorldGenerator(worldGenerator);

    camera.setWorldBoundary(worldGenerator->getBoundary());

    Physics* physics = new Physics();

    gameManager.setPhysicsPointer(physics);

    physics->addPlayer(player1);
    physics->addPlayer(player2);

    gameManager.setBlocksForPhysics();

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

    Signals::SetPositionForPlayerId.emit(0, gameManager.getPlayer1SpawnPos());
    Signals::SetPositionForPlayerId.emit(1, gameManager.getPlayer2SpawnPos());

    sf::Clock clock;
    auto* t = new sf::Text("Game Over", font, 30);
    t->setPosition({100, 100});
    t->setFillColor(sf::Color::Black);

    interface->setClock(&clock);
    // sf::Vector2f target = worldGenerator->trapdoorPosition;

    std::vector<Player*> players = {player1, player2};

    //merge textures


    Background* background = new Background(&camera);
    background->SetWorldGenerator(worldGenerator);

    background->AddLayer("data/img/background/back.png", 1.0f);

    background->ReadBackgroundFile("data/img/background/clouds2.txt", 0);
    background->AddLayerFromBlockLayout(0, {230,230,230}, 1.3f, false);

    background->ReadBackgroundFile("data/img/background/clouds.txt", 1.0f);
    background->AddLayerFromBlockLayout(1, {255, 255, 255}, 1.2f, false);

    background->ReadBackgroundFile("data/img/background/front.txt", 2);
    background->AddLayerFromBlockLayout(2, {128, 128, 128}, 1.4f, true);

    //Layer 1 controls

    Button* b_layer1IncreaseSpeed = new Button(ONE_SHOT, 100, 40, 40, 40, "+", font, [background, interface]() {
        background->IncreaseLayerSpeed(0);
        interface->setTextValue(0, background->GetScrollSpeed(0));
    });
    interface->addButton(b_layer1IncreaseSpeed);

    Button* b_layer1DecreaseSpeed = new Button(ONE_SHOT, 150, 40, 40, 40, "-", font, [background, interface]() {
        background->DecreaseLayerSpeed(0);
        interface->setTextValue(0, background->GetScrollSpeed(0));
    });
    interface->addButton(b_layer1DecreaseSpeed);

    sf::Text layer1Text("Layer 1", font, 16);
    Text layer1TextObj;
    layer1TextObj.text = layer1Text;
    layer1TextObj.relativePosition = sf::Vector2f(20, 20);
    interface->addText(&layer1TextObj);
    sf::Text layer1Value(Util::toStringWithTwoDecimalPlaces(background->GetScrollSpeed(0)), font, 16);
    Text layer1textValue;
    layer1textValue.text = layer1Value;
    layer1textValue.relativePosition = sf::Vector2f(20, 40);
    interface->addTextValue(&layer1textValue);


    //Layer 2 controls

    Button* b_layer2IncreaseSpeed = new Button(ONE_SHOT, 100, 90, 40, 40, "+", font, [background, interface]() {
    background->IncreaseLayerSpeed(1);
    interface->setTextValue(1, background->GetScrollSpeed(1));
    });
    interface->addButton(b_layer2IncreaseSpeed);

    Button* b_layer2DecreaseSpeed = new Button(ONE_SHOT, 150, 90, 40, 40, "-", font, [background, interface]() {
        background->DecreaseLayerSpeed(1);
        interface->setTextValue(1, background->GetScrollSpeed(1));
    });
    interface->addButton(b_layer2DecreaseSpeed);

    sf::Text layer2Text("Layer 2", font, 16);
    Text layer2TextObj;
    layer2TextObj.text = layer2Text;
    layer2TextObj.relativePosition = sf::Vector2f(20, 70);
    interface->addText(&layer2TextObj);
    sf::Text layer2Value(Util::toStringWithTwoDecimalPlaces(background->GetScrollSpeed(1)), font, 16);
    Text layer2textValue;
    layer2textValue.text = layer2Value;
    layer2textValue.relativePosition = sf::Vector2f(20, 90);
    interface->addTextValue(&layer2textValue);

    //Layer 3 controls
    Button* b_layer3IncreaseSpeed = new Button(ONE_SHOT, 100, 140, 40, 40, "+", font, [background, interface]() {
    background->IncreaseLayerSpeed(2);
    interface->setTextValue(2, background->GetScrollSpeed(2));
    });
    interface->addButton(b_layer3IncreaseSpeed);

    Button* b_layer3DecreaseSpeed = new Button(ONE_SHOT, 150, 140, 40, 40, "-", font, [background, interface]() {
        background->DecreaseLayerSpeed(2);
        interface->setTextValue(2, background->GetScrollSpeed(2));
    });
    interface->addButton(b_layer3DecreaseSpeed);

    sf::Text layer3Text("Layer 3", font, 16);
    Text layer3TextObj;
    layer3TextObj.text = layer3Text;
    layer3TextObj.relativePosition = sf::Vector2f(20, 120);
    interface->addText(&layer3TextObj);
    sf::Text layer3Value(Util::toStringWithTwoDecimalPlaces(background->GetScrollSpeed(2)), font, 16);
    Text layer3textValue;
    layer3textValue.text = layer3Value;
    layer3textValue.relativePosition = sf::Vector2f(20, 140);
    interface->addTextValue(&layer3textValue);

    //Background controls
    Button* b_backgroundIncreaseSpeed = new Button(ONE_SHOT, 100, 190, 40, 40, "+", font, [background, interface]() {
    background->IncreaseScrollSpeed();
    interface->setTextValue(3, background->GetScrollSpeed());
    });
    interface->addButton(b_backgroundIncreaseSpeed);

    Button* b_backgroundDecreaseSpeed = new Button(ONE_SHOT, 150, 190, 40, 40, "-", font, [background, interface]() {
        background->DecreaseScrollSpeed();
        interface->setTextValue(3, background->GetScrollSpeed());
    });
    interface->addButton(b_backgroundDecreaseSpeed);

    sf::Text backgroundSpeedText("BG speed", font, 16);
    Text backgroundSpeedTextObj;
    backgroundSpeedTextObj.text = backgroundSpeedText;
    backgroundSpeedTextObj.relativePosition = sf::Vector2f(20, 170);
    interface->addText(&backgroundSpeedTextObj);
    sf::Text backgroundSpeedValue(Util::toStringWithTwoDecimalPlaces(background->GetScrollSpeed()), font, 16);
    Text backgroundSpeedtextValue;
    backgroundSpeedtextValue.text = backgroundSpeedValue;
    backgroundSpeedtextValue.relativePosition = sf::Vector2f(20, 190);
    interface->addTextValue(&backgroundSpeedtextValue);

    //points
    sf::Text pointsText("Feathers:", font, 20);
    Text pointsTextObj;
    pointsTextObj.text = pointsText;
    pointsTextObj.relativePosition = sf::Vector2f(camera.getView().getSize().x - 190, 20);
    interface->addText(&pointsTextObj);
    sf::Text pointsValue("0", font, 20);
    Text pointsValueText;
    pointsValueText.text = pointsValue;
    pointsValueText.relativePosition = sf::Vector2f(camera.getView().getSize().x - 85, 20);
    interface->addTextValue(&pointsValueText);

    sf::Text slashText("/", font, 20);
    Text slashTextObj;
    slashTextObj.text = slashText;
    slashTextObj.relativePosition = sf::Vector2f(camera.getView().getSize().x - 55, 20);
    interface->addText(&slashTextObj);
    sf::Text maxPointsValue("0", font, 20);
    Text maxPointsValueText;
    maxPointsValueText.text = maxPointsValue;
    maxPointsValueText.relativePosition = sf::Vector2f(camera.getView().getSize().x - 30, 20);
    interface->addTextValue(&maxPointsValueText);

    Signals::SetMaxPoints.emit(gameManager.maxPoints);
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
        background->Update(delta, window);


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
        for (auto* text: interface->texts) {
            interface->updatePosition(camera.getView(), text);
        }
        for (auto* text: interface->textValues) {
            interface->updatePosition(camera.getView(), text);
        }
        guideArrow0->update({player1->getCenterX(), player1->getCenterY()}, worldGenerator->trapdoorPosition);
        // guideArrow1->update({player2->getCenterX(), player2->getCenterY()}, worldGenerator->trapdoorPosition);


        for (int i = 0; i < views; i++) {

            background->Draw(window);

            if (camera.isSplit()) {
                window.setView(i == 0 ? camera.getSplitViewLeft() : camera.getSplitViewRight());
            } else {
                window.setView(camera.getSplitViewLeft());
            }

            worldGenerator->drawWorld(window);

            window.draw(player1->sprite);
            // window.draw(player2->sprite);

            interface->draw(window);

            // guideArrow0->draw(window);
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
