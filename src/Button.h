//
// Created by Hubert Klonowski on 21/11/2024.
//

#ifndef BUTTON_H
#define BUTTON_H

enum Type {
    ONE_SHOT,
    TOGGLE
};

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Vector2f relativePosition; // Position relative to the view.
    std::function<void()> onClick;
    bool pressed = false;
    Type type = ONE_SHOT;

    bool toggled = false;

public:
    Button(Type type, float x, float y, float width, float height, const std::string& label, const sf::Font& font, const std::function<void()>& callback)
        : relativePosition(x, y), onClick(callback) {
        this->type = type;
        // Set up the shape.
        shape.setSize({width, height});
        shape.setFillColor(sf::Color::White);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(2);

        // Set up the text.
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::Black);

        // Center text within the button.
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
        shape.setOrigin(width / 2, height / 2);
    }

    void updatePosition(const sf::View& view) {
        // Calculate absolute position based on the view.
        sf::Vector2f viewCenter = view.getCenter();
        sf::Vector2f viewSize = view.getSize();
        sf::Vector2f absolutePosition = viewCenter - viewSize / 2.0f + relativePosition;

        shape.setPosition(absolutePosition);
        text.setPosition(absolutePosition);
    }

    void draw(sf::RenderWindow& window) {
        if (type == TOGGLE) {
            if (toggled) shape.setFillColor(sf::Color::Green);
            else shape.setFillColor(sf::Color::White);
        }
        window.draw(shape);
        window.draw(text);
    }

    void setColor(sf::Color color) {
        shape.setFillColor(color);
    }

    void setToggled(bool v) {
        toggled = v;
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (pressed) return;
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (shape.getGlobalBounds().contains(mousePos)) {
                onClick();
                pressed = true;
                if (type == TOGGLE) {
                    toggled = !toggled;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (pressed) {
                pressed = false;
            }
        }
    }
};

#endif //BUTTON_H
