#include <iostream>
 #include <SFML/Graphics.hpp>

enum directions { down, right, up, left };
int main()
{
    unsigned int windowWidth = 640;
    unsigned int windowHeight = 360;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({windowWidth, windowHeight}), "SFML Test");
    window->setFramerateLimit(60);

    sf::Texture texture;
    if(!texture.loadFromFile("Sprites/ExampleSprite.png")){
        std::cerr << "ERROR::COULD NOT LOAD FILE::Sprites/ExampleSprite.png" << std::endl;
		return -1;
    }
    sf::Sprite sprite(texture);

    sf::IntRect dir[4];
    for (int i = 0; i < 4; i++)
    {
        dir[i] = sf::IntRect({32 * i, 0}, {32, 32});
    }

    sprite.setTextureRect(dir[down]);
    sprite.setOrigin({16, 16});
    sprite.setPosition({windowWidth / 2.0f, windowHeight / 2.0f});

    


    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window->close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {

			sprite.move({ 0.0f, 1.0f });
			sprite.setTextureRect(dir[down]);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {

			sprite.move({ 0.0f, -1.0f });
			sprite.setTextureRect(dir[up]);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {

			sprite.move({ 1.0f, 0.0f });
			sprite.setTextureRect(dir[right]);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {

			sprite.move({ -1.0f, 0.0f });
			sprite.setTextureRect(dir[left]);
		}

        // render
        window->clear();

        // draw
        window->draw(sprite);
        
        window->display();
    }
}