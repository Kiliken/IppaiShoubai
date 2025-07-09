
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <SFML/Window/Event.hpp>
#include "Paolo.h"

void playerInput()
{
}

void renderScene() {

    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "SFML works!");
    window.setFramerateLimit(60);
    sf::Vector2u windowSize = window.getSize();

    sf::RectangleShape backGrouond(sf::Vector2f(100.f, 100.f));
    backGrouond.setFillColor(sf::Color::White);

    sf::Clock deltaClock;

    const sf::Font font("../Assets/Fonts/arial.ttf");
    sf::Text text(font, "hello");
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Black);
    sf::Texture texture;
    if (!texture.loadFromFile("../Assets/Images/Ghost.png"))
    {
        std::cout << "Failed to load texture" << std::endl;
    }


    sf::Sprite sprite(texture);
    sprite.setScale(sf::Vector2f(0.5f, 0.5f));
    //sprite.setPosition(sf::Vector2f((windowSize.x/2) - sprite.getTextureRect().size.x/2, (windowSize.y / 2) - sprite.getTextureRect().size.y/2));
    sprite.setOrigin(sf::Vector2f(sprite.getTextureRect().size.x / 2, sprite.getTextureRect().size.y / 2));
    sprite.setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));


    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        float speed = 300.f;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            /*if ((event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Left)){
                sprite.setPosition(sf::Vector2f(sprite.getPosition().x - (speed * deltaTime), sprite.getPosition().y));
            }

            if ((event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Right)) {
                sprite.setPosition(sf::Vector2f(sprite.getPosition().x + (speed * deltaTime), sprite.getPosition().y));
            }

            if ((event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up)) {
                sprite.setPosition(sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y - (speed * deltaTime)));
            }

            if ((event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down)) {
                sprite.setPosition(sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y + (speed * deltaTime)));
            }*/

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            sprite.setPosition(sf::Vector2f(sprite.getPosition().x - (speed * deltaTime), sprite.getPosition().y));
            sprite.setScale(sf::Vector2f(0.5f, 0.5f)); // Flip sprite back horizontally
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            sprite.setPosition(sf::Vector2f(sprite.getPosition().x + (speed * deltaTime), sprite.getPosition().y));
            sprite.setScale(sf::Vector2f(-0.5f, 0.5f)); // Flip sprite horizontally
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            sprite.setPosition(sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y - (speed * deltaTime)));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            sprite.setPosition(sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y + (speed * deltaTime)));
        }

        backGrouond.setSize(sf::Vector2f(windowSize.x, windowSize.y));
        window.clear();
        window.draw(backGrouond);
        window.draw(text);
        window.draw(sprite);
        window.display();
    }
}


int main()
{
    Paolo::test();
    playerInput();
    renderScene();
	return 0;
}


