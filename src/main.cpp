
 #include <SFML/Graphics.hpp>
 #include <iostream>
 #include <vector>
 #include <algorithm>
 #include <stdlib.h>

 #define ASSETS "../assets/"

struct Item {
    sf::Texture *texture;
    sf::Sprite  *sprite;
    sf::String  name;
    int         price = 0;
    char        tag;
    // tags : s(shopItem) i(inventoryItem)

    Item Clone() {
        Item newItem;
        newItem.texture = this->texture;
        newItem.sprite = new sf::Sprite(*newItem.texture);
        newItem.sprite->setScale(this->sprite->getScale());
        newItem.price = this->price;
        newItem.name = this->name;
        newItem.tag = this->tag;

        return newItem;
    }
};

std::vector<Item> DeclareItems() {
    std::vector<Item> items;

    Item item;


    item.texture = new sf::Texture();
    item.texture->loadFromFile("../assets/bow.png");
    item.sprite = new sf::Sprite(*item.texture);
    item.name = "Bow";
    item.price = 10;
    item.tag = 's';

    items.push_back(item);

    item.texture = new sf::Texture();
    item.texture->loadFromFile("../assets/hppotion.png");
    item.sprite = new sf::Sprite(*item.texture);
    item.name = "Heal Potion";
    item.price = 20;
    item.tag = 's';

    items.push_back(item);

    item.texture = new sf::Texture();
    item.texture->loadFromFile("../assets/manapotion.png");
    item.sprite = new sf::Sprite(*item.texture);
    item.name = "Mana Potion";
    item.price = 40;
    item.tag = 's';

    items.push_back(item);
    item.texture = new sf::Texture();
    item.texture->loadFromFile("../assets/manapotion.png");
    item.sprite = new sf::Sprite(*item.texture);
    item.name = "Mana Potion";
    item.price = 40;
    item.tag = 's';

    items.push_back(item);
    item.texture = new sf::Texture();
    item.texture->loadFromFile("../assets/manapotion.png");
    item.sprite = new sf::Sprite(*item.texture);
    item.name = "Mana Potion";
    item.price = 40;
    item.tag = 's';

    items.push_back(item);
    item.texture = new sf::Texture();
    item.texture->loadFromFile("../assets/manapotion.png");
    item.sprite = new sf::Sprite(*item.texture);
    item.name = "Mana Potion";
    item.price = 40;
    item.tag = 's';

    items.push_back(item);
    item.texture = new sf::Texture();
    item.texture->loadFromFile("../assets/manapotion.png");
    item.sprite = new sf::Sprite(*item.texture);
    item.name = "Mana Potion";
    item.price = 40;
    item.tag = 's';

    items.push_back(item);
    item.texture = new sf::Texture();
    item.texture->loadFromFile("../assets/manapotion.png");
    item.sprite = new sf::Sprite(*item.texture);
    item.name = "Mana Potion";
    item.price = 40;
    item.tag = 's';

    items.push_back(item);
    

    for(int i = 0; i < items.size(); i++){
            items[i].sprite->setScale({0.10f,0.10f});
    }

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

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Ippai Shoubai");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);
    //const sf::Image gameIcon("../assets/cursor.png");
    //window.setIcon(gameIcon);

    const sf::Font font("../assets/arial.ttf");
    sf::Text moneyText(font, std::to_string(money) + "$");
    moneyText.setCharacterSize(30);
    moneyText.setStyle(sf::Text::Bold);
    moneyText.setFillColor(sf::Color::Black);
    moneyText.setPosition({1150.f,200.f});

    sf::Text hoverText(font,"");
    hoverText.setCharacterSize(40);
    hoverText.setStyle(sf::Text::Bold);
    hoverText.setFillColor(sf::Color::Black);
    hoverText.setPosition({200.f,650.f});

    std::vector<Item> inventory;
    sf::Vector2f inventoryPos[12] = {
        {925.f,275.f},{1025.f,275.f},{1125.f,275.f},
        {925.f,375.f},{1025.f,375.f},{1125.f,375.f},
        {925.f,475.f},{1025.f,475.f},{1125.f,475.f},
        {925.f,575.f},{1025.f,575.f},{1125.f,575.f},
    };

    sf::Vector2f storePos[8] = {
        {50.f,50.f},{200.f,50.f},
        {50.f,200.f},{200.f,200.f},
        {50.f,350.f},{200.f,350.f},
        {50.f,500.f},{200.f,500.f},
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

    /*for(int i = 0; i < items.size(); i++){
        items[i].sprite->setPosition({200.f,200.f});
        items[i].sprite->setScale({0.10f,0.10f});
        
    }*/
            
    Item itemClicked;
    Item itemHover;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()){
                itemClicked = InteractWith(items,cursorSprite).Clone();

                if(inventory.size() < 12 && money >= itemClicked.price && itemClicked.tag == 's') {
                    itemClicked.tag = 'i';
                    //itemClicked.sprite->setColor(sf::Color(rand() % 255,rand() % 255,rand() % 255));
                    money -= itemClicked.price;
                    moneyText.setString(std::to_string(money) + "$");
                    inventory.push_back(itemClicked);
                    //std::cout << money << std::endl;
                }else {
                    itemClicked = InteractWith(inventory,cursorSprite);
                
                    if(itemClicked.tag == 'i'){
                        for(int i=0; i < inventory.size(); i++){
                            if(itemClicked.sprite->getPosition() == inventory[i].sprite->getPosition()){
                                money += itemClicked.price;
                                moneyText.setString(std::to_string(money) + "$");
                                inventory.erase(inventory.begin() + i);
                                break;
                            }
                        }
                    }
                }
            }
        }

        cursorSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x) - window.getPosition().x,static_cast<float>(sf::Mouse::getPosition().y) - window.getPosition().y});
        
        
       for(int i = 0; i < inventory.size(); i++){
            inventory[i].sprite->setPosition(inventoryPos[i]);
            //inventory[i].sprite->setScale({0.10f,0.10f});
        }

        for(int i = 0; i < items.size(); i++){
            items[i].sprite->setPosition(storePos[i]);
            
        }
        
        {
            hoverText.setString("");
            itemHover = InteractWith(items,cursorSprite);
            if(itemHover.price != 0){
                hoverText.setString(itemHover.name + " " + std::to_string(itemHover.price) + "$");
            }else{
                itemHover = InteractWith(inventory,cursorSprite);
                if(itemHover.price != 0)
                    hoverText.setString(itemHover.name + " " + std::to_string(itemHover.price) + "$");
            }
        }

        window.clear();
        window.draw(bgSprite);
        //window.draw(shape);
        

        for(int i = 0; i < items.size(); i++)
            window.draw(*items[i].sprite);

        for(int i = 0; i < inventory.size(); i++)
            window.draw(*inventory[i].sprite);


        window.draw(hoverText);
        window.draw(moneyText);
        window.draw(cursorSprite);
        
        window.display();
    }
}

