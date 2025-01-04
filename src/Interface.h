//
// Created by Hubert Klonowski on 21/11/2024.
//

#ifndef INTERFACE_H
#define INTERFACE_H
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Button.h"
#include "Signals.h"

class Interface {

    public:
    std::vector<Button*> buttons;
    sf::Text* text;
    std::string message;
    sf::Font* font;
    sf::Clock* clock;

    sf::Text* player0Info;
    sf::Text* player1Info;

    Interface(sf::Font* font) {
        Signals::ShowMessage.connect([this] (std::string msg) {this->onShowMessage(msg);});
        text = new sf::Text("Find the trapdoor!", *font, 24);
        text->setPosition({10, 10});
        text->setOutlineColor(sf::Color::Black);
        text->setOutlineThickness(2);

        // player0Info = new sf::Text("Player 0", *font, 24);
        clock = new sf::Clock();
    }
    ~Interface(){}

    void addButton(Button* button) {
        buttons.push_back(button);
    }

    void setClock(sf::Clock* clock) {
        this->clock = clock;
    }

    void draw(sf::RenderWindow& window) {
        // for (int i = 0; i < buttons.size(); i++) {
        //     buttons[i]->draw(window);
        // }

        if(clock->getElapsedTime().asSeconds() < 2.0f) {
            window.draw(*text);
        }
    }

    void updatePosition(const sf::View& view, sf::RenderWindow& window) {
        // Calculate absolute position based on the view.
        sf::Vector2f viewCenter = view.getCenter();
        sf::Vector2f viewSize = view.getSize();
        sf::Vector2f absolutePosition = viewCenter - viewSize / 2.0f + sf::Vector2f(40, 40);

        text->setPosition(absolutePosition);
    }

    void onShowMessage(std::string msg) {
        text->setString(msg);
        clock->restart();
    }
};

#endif //INTERFACE_H
