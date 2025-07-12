
 #include <SFML/Graphics.hpp>
 #include <iostream>
 #include <vector>

 #define ASSETS "../assets/"

struct Item {
    sf::Texture texture;
    sf::Sprite  *sprite;
    sf::String  name;
    int         price;
};

std::vector<Item> DeclareItems() {
    std::vector<Item> items;

    Item bow;

    bow.texture = sf::Texture("../assets/bow.png");
    bow.sprite = new sf::Sprite(bow.texture);
    bow.name = "Bow";
    bow.price = 10;


    return items;
}

int main()
{
    int money = 100;
    bool mouseLeftDown = false;

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "SFML works!");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Red);

    

    const sf::Texture cursorText("../assets/cursor.png");
    sf::Sprite cursorSprite(cursorText);
    cursorSprite.setOrigin({cursorText.getSize().x / 2.f,cursorText.getSize().y/ 2.f});
    cursorSprite.setScale({0.06f,0.06f});
    
    std::vector<Item> items = DeclareItems();

    for(int i = 0; i < items.size(); i++){
        items[i].sprite->setPosition({200.f,200.f});
        items[i].sprite->setScale({0.25f,0.25f});
        std::cout << items[i].name.toAnsiString() << std::endl;
    }
            
    


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()){
                if (cursorSprite.getGlobalBounds().findIntersection(shape.getGlobalBounds()))
                std::cout << "WORKS" << std::endl;
            }
        }

        cursorSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x) - window.getPosition().x,static_cast<float>(sf::Mouse::getPosition().y) - window.getPosition().y});
        
        



        window.clear();
        window.draw(shape);
        window.draw(cursorSprite);

        for(int i = 0; i < items.size(); i++)
            window.draw(*items[i].sprite);
        
        window.display();
    }
}

