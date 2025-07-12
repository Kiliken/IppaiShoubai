
 #include <SFML/Graphics.hpp>

int main()
{
    int money = 0;

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    const sf::Texture cursorText("../assets/cursor.png");
    sf::Sprite cursorSprite(cursorText);
    cursorSprite.setOrigin({cursorText.getSize().x / 2.f,cursorText.getSize().y/ 2.f});
    


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        
        cursorSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x),static_cast<float>(sf::Mouse::getPosition().y)});

        window.clear();
        window.draw(shape);
        window.draw(cursorSprite);
        window.display();
    }
}