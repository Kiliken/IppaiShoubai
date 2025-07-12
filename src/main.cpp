
 #include <SFML/Graphics.hpp>
 #include <iostream>
 #include <vector>

 #define ASSETS "../assets/"

struct Item {
    sf::Texture *texture;
    sf::Sprite  *sprite;
    sf::String  name;
    int         price;
};

std::vector<Item> DeclareItems() {
    std::vector<Item> items;

    Item bow;


    bow.texture = new sf::Texture("../assets/bow.png");
    bow.sprite = new sf::Sprite(*bow.texture);
    bow.name = "Bow";
    bow.price = 10;

    items.push_back(bow);

    return items;
}

Item InteractWith(std::vector<Item> items, sf::Sprite mousePos){
    Item empty;


    for(int i = 0; i < items.size(); i++){
         if (items[i].sprite->getGlobalBounds().findIntersection(mousePos.getGlobalBounds())){
            return items[i];
         }    
    }

    return empty;
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

    std::vector<Item> inventory;
    sf::Vector2f inventoryPos[12] = {
        {900.f,300.f},{500.f,400.f},{0.f,0.f},
        {0.f,0.f},{0.f,0.f},{0.f,0.f},
        {0.f,0.f},{0.f,0.f},{0.f,0.f},
        {0.f,0.f},{0.f,0.f},{0.f,0.f},
    };

    const sf::Texture cursorText("../assets/cursor.png");
    sf::Sprite cursorSprite(cursorText);
    cursorSprite.setOrigin({cursorText.getSize().x / 2.f,cursorText.getSize().y/ 2.f});
    cursorSprite.setScale({0.06f,0.06f});

    const sf::Texture bgText("../assets/background.png");
    sf::Sprite bgSprite(bgText);
    bgSprite.setOrigin({0.f,0.f});
    bgSprite.setScale({1.25f,1.25f});
    
    std::vector<Item> items = DeclareItems();

    for(int i = 0; i < items.size(); i++){
        items[i].sprite->setPosition({200.f,200.f});
        items[i].sprite->setScale({0.10f,0.10f});
        
    }
            
    


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()){
                Item itemClicked = InteractWith(items,cursorSprite);
                inventory.push_back(itemClicked);
            }
        }

        cursorSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x) - window.getPosition().x,static_cast<float>(sf::Mouse::getPosition().y) - window.getPosition().y});
        
        
       for(int i = 0; i < inventory.size(); i++){
            inventory[i].sprite->setPosition(inventoryPos[i]);
            inventory[i].sprite->setScale({0.10f,0.10f});
        }

        window.clear();
        window.draw(bgSprite);
        window.draw(shape);
        

        for(int i = 0; i < items.size(); i++)
            window.draw(*items[i].sprite);

        window.draw(cursorSprite);
        
        window.display();
    }
}

