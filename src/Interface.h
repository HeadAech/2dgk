//
// Created by Hubert Klonowski on 21/11/2024.
//

#ifndef INTERFACE_H
#define INTERFACE_H
#include <iomanip>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Button.h"
#include "Signals.h"
#include "Util.h"

struct Text {
    sf::Text text;
    sf::Vector2f relativePosition;
};

class Interface {

    public:
    std::vector<Button*> buttons;
    std::vector<Text*> texts;
    std::vector<Text*> textValues;
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


        // player0Info = new sf::Text("Player 0", *font, 24);
        clock = new sf::Clock();

        Signals::UpdatePoints.connect([this] (int value) {onIncreasePoints(value);});
        Signals::SetMaxPoints.connect([this] (int value) {onSetMaxPoints(value);});
    }
    ~Interface(){}

    void addButton(Button* button) {
        buttons.push_back(button);
    }

    void addText(Text* text) {
        text->text.setOutlineColor(sf::Color::Black);
        text->text.setOutlineThickness(2);
        texts.push_back(text);
    }

    void addTextValue(Text* text) {
        text->text.setOutlineColor(sf::Color::Black);
        text->text.setOutlineThickness(2);
        textValues.push_back(text);
    }

    void setTextValue(int i, float value) {
        textValues.at(i)->text.setString(Util::toStringWithTwoDecimalPlaces(value));
    }



    void setClock(sf::Clock* clock) {
        this->clock = clock;
    }

    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < buttons.size(); i++) {
            buttons[i]->draw(window);
        }

        for (int i = 0; i < texts.size(); i++) {
            window.draw(texts[i]->text);
        }

        for (int i = 0; i < textValues.size(); i++) {
            window.draw(textValues.at(i)->text);
        }

    }

    void updatePosition(const sf::View& view, Text* text) {
        // Calculate absolute position based on the view.
        sf::Vector2f viewCenter = view.getCenter();
        sf::Vector2f viewSize = view.getSize();
        sf::Vector2f absolutePosition = viewCenter - viewSize / 2.0f + text->relativePosition;

        text->text.setPosition(absolutePosition);
    }

    void onShowMessage(std::string msg) {
        text->setString(msg);
        clock->restart();
    }

    void onIncreasePoints(int value) {
        textValues.at(textValues.size() - 2)->text.setString(std::to_string(value));
    }

    void onSetMaxPoints(int value) {
        textValues.at(textValues.size() - 1)->text.setString(std::to_string(value));
    }
};

#endif //INTERFACE_H
