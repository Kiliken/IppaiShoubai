#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <random>
#include <numeric>
#include <string>

#define ASSETS "../assets/"

// Item struct used for each item in the game
struct Item {
    sf::Texture *texture;
    sf::Sprite  *sprite;
    sf::String  name;
    int         id;
    int         price = 0;
    char        tag;    // tags : s(shopItem) i(inventoryItem)
    float       priceModifier;
    
    // generate a copy of the struct
    Item Clone() {
        Item newItem;
        newItem.texture = this->texture;
        newItem.sprite = new sf::Sprite(*newItem.texture);
        newItem.sprite->setScale(this->sprite->getScale());
        newItem.id = this->id;
        newItem.price = this->price;
        newItem.name = this->name;
        newItem.tag = this->tag;
        newItem.priceModifier = this->priceModifier;

        return newItem;
    }
};

// Shop struct for randomly generating new shops
struct Shop{
    sf::Texture *texture;
    sf::Sprite  *sprite;
    std::vector<Item> items;
    int currentHigh;    // item being bought at a high price
    int currentLow; // item being bought at a low price
    int forecastHigh;    // item being bought at a high price in next shop
    int forecastLow; // item being bought at a low price in next shop
    int lowestItemPrice;    // lowest item price in the shop
};


std::vector<Item> ALL_ITEMS;    // vector of all items in the game


// FORWARD DECLARATIONS
std::vector<Item> SetUpItems(); // set all default items in a vector
Shop SetUpShop(int highItem, int lowItem);
Item InteractWith(std::vector<Item> &items, sf::Sprite mousePos); // mouse interaction for items
int GetRandomIntInRange(int min, int max);
void ModifyPrices(int highItem, int lowItem);
void ModifyItemListPrice(std::vector<Item> &items);
void FindLowestPriceItem(Shop &shop);



int main(){
    srand(static_cast<unsigned int>(time(0)));  // random seed
    int money = 100;
    int moneyGoal = 1000;
    int scene = 0;  // current shop
    bool madeTransaction = false;   // bought or sold something
    bool gameClear = false;
    bool gameOver = false;

    // WINDOW
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Ippai Shoubai");
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    // money text
    const sf::Font font("../assets/arial.ttf");
    sf::Text moneyText(font, std::to_string(money) + "$");
    moneyText.setCharacterSize(30);
    moneyText.setStyle(sf::Text::Bold);
    moneyText.setFillColor(sf::Color::Black);
    moneyText.setPosition({1150.f,200.f});

    // text for item being hovered on
    sf::Text hoverText(font,"");
    hoverText.setCharacterSize(40);
    hoverText.setStyle(sf::Text::Bold);
    hoverText.setFillColor(sf::Color::Black);
    hoverText.setPosition({180.f,660.f});


    // text for stores left
    sf::Text storesLeftText(font,"");
    storesLeftText.setCharacterSize(35);
    storesLeftText.setStyle(sf::Text::Bold);
    storesLeftText.setFillColor(sf::Color::Black);
    storesLeftText.setPosition({500.f, 15.f});
    int storesCount = 10;

    //text for number of items the store will buy
    sf::Text willBuyText(font, "Will buy \n");
    willBuyText.setCharacterSize(25);
    willBuyText.setStyle(sf::Text::Bold);
    willBuyText.setFillColor(sf::Color::Black);
    willBuyText.setPosition({450.f, 325.f});
    int willBuy = 5;

    // game clear and game over texts
    sf::Text GameClearText(font,"GAME CLEAR");
    GameClearText.setCharacterSize(80);
    GameClearText.setStyle(sf::Text::Bold);
    GameClearText.setFillColor(sf::Color::Green);
    GameClearText.setPosition({350.f,300.f});
    sf::Text GameOverText(font,"GAME OVER");
    GameOverText.setCharacterSize(80);
    GameOverText.setStyle(sf::Text::Bold);
    GameOverText.setFillColor(sf::Color::Red);
    GameOverText.setPosition({350.f,300.f});

    // cursor
    const sf::Texture cursorText("../assets/cursor.png");
    sf::Sprite cursorSprite(cursorText);
    cursorSprite.setOrigin({cursorText.getSize().x / 2.f,cursorText.getSize().y/ 2.f});
    cursorSprite.setScale({0.06f,0.06f});

    // arrow for switching shops
    const sf::Texture arrowsText("../assets/arrow.png");
    sf::Sprite rightArrow(arrowsText);
    rightArrow.setOrigin({arrowsText.getSize().x / 2.f,arrowsText.getSize().y/ 2.f});
    rightArrow.setScale({0.25f,0.25f});
    rightArrow.setPosition({1200.f,100.f});

    // item positions
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

    ALL_ITEMS = SetUpItems();
    //std::cout << ALL_ITEMS[3].name.toAnsiString() << std::endl;
    Item itemClicked;
    Item itemHover;
    Item itemHigh;  // high price item forecast
    Item itemLow;   // low price item forecast

    // Set up first shop
    Shop currentShop = SetUpShop(-1, -1);
    itemHigh = ALL_ITEMS[currentShop.forecastHigh].Clone();
    itemHigh.sprite->setScale({0.07f,0.07f});
    itemHigh.sprite->setPosition({1080.f, 0.f});
    itemLow = ALL_ITEMS[currentShop.forecastLow].Clone();
    itemLow.sprite->setScale({0.07f,0.07f});
    itemLow.sprite->setPosition({1180.f, 0.f});

    // MAIN LOOP
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // mouse button press
            if (event->is<sf::Event::MouseButtonPressed>() && !gameClear && !gameOver){
                Item clickedItem = InteractWith(currentShop.items, cursorSprite);
                if (clickedItem.sprite != nullptr) {
                    itemClicked = clickedItem.Clone();

                    // buy item from shop
                    if(inventory.size() < 12 && money >= itemClicked.price && itemClicked.tag == 's') {
                        itemClicked.tag = 'i';
                        //itemClicked.sprite->setColor(sf::Color(rand() % 255,rand() % 255,rand() % 255));
                        money -= itemClicked.price;
                        moneyText.setString(std::to_string(money) + "$");
                        inventory.push_back(itemClicked);
                        madeTransaction = true;
                        break;
                        //std::cout << money << std::endl;
                    }
                }
                // sell item from inventory
                else if (willBuy > 0){
                    itemClicked = InteractWith(inventory,cursorSprite);
                    
                    if(itemClicked.sprite != nullptr){
                        
                        if(itemClicked.tag == 'i'){
                            for(int i = 0; i < inventory.size(); i++){
                                if(itemClicked.sprite->getPosition() == inventory[i].sprite->getPosition()){
                                    money += itemClicked.price;
                                    willBuy --;
                                    
                                    moneyText.setString(std::to_string(money) + "$");
                                    inventory.erase(inventory.begin() + i);

                                    madeTransaction = true;
                                    break;
                                }
                            }
                        }
                        
                    }
                }

                // go to next shop
                if (rightArrow.getGlobalBounds().findIntersection(cursorSprite.getGlobalBounds()) && madeTransaction){
                    currentShop = SetUpShop(currentShop.forecastHigh, currentShop.forecastLow);
                    ModifyPrices(currentShop.currentHigh, currentShop.currentLow);
                    std::cout << "shop prices:" << std::endl;
                    ModifyItemListPrice(currentShop.items);
                    FindLowestPriceItem(currentShop);
                    std::cout << "inventory prices:" << std::endl;
                    ModifyItemListPrice(inventory);
                    
                    willBuy = rand() % 4 + 4;

                    itemHigh = ALL_ITEMS[currentShop.forecastHigh].Clone();
                    itemHigh.sprite->setScale({0.07f,0.07f});
                    itemHigh.sprite->setPosition({1080.f, 0.f});
                    itemLow = ALL_ITEMS[currentShop.forecastLow].Clone();
                    itemLow.sprite->setScale({0.07f,0.07f});
                    itemLow.sprite->setPosition({1180.f, 0.f});

                    scene++;
                    storesCount = std::max(storesCount - 1, 0);
                    madeTransaction = false;
                    break;
                }
            }
        }

        // win lose condition
        if(money >= moneyGoal){
            gameClear = true;
        }
        else if((money < currentShop.lowestItemPrice && inventory.size() == 0) || storesCount <= 0){
            gameOver = true;
        }
        

        // set item positions
        for(int i = 0; i < inventory.size(); i++){
            inventory[i].sprite->setPosition(inventoryPos[i]);
            //inventory[i].sprite->setScale({0.10f,0.10f});
        }
        for(int i = 0; i < currentShop.items.size(); i++){
            currentShop.items[i].sprite->setPosition(storePos[i]);
            
        }

        // cursor
        cursorSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x) - window.getPosition().x,static_cast<float>(sf::Mouse::getPosition().y) - window.getPosition().y});
        
        // text
        hoverText.setString("");
        itemHover = InteractWith(currentShop.items, cursorSprite);
        if(itemHover.price != 0){
            hoverText.setString(itemHover.name + " : " + std::to_string(itemHover.price) + "$");
        }else{
            itemHover = InteractWith(inventory, cursorSprite);
            if(itemHover.price != 0){
                hoverText.setString(itemHover.name + " : " + std::to_string(itemHover.price) + "$");
                
            }
        }
        

        // RENDERING
        window.clear();
        window.draw(*currentShop.sprite);

        for(int i = 0; i < inventory.size(); i++)
            window.draw(*inventory[i].sprite);
        for(int i = 0; i < currentShop.items.size(); i++)
            window.draw(*currentShop.items[i].sprite);

        if(itemHigh.price != 0)
            window.draw(*itemHigh.sprite);
        if(itemLow.price != 0)
            window.draw(*itemLow.sprite);

        window.draw(hoverText);
        storesLeftText.setString("Stores Left: " + std::to_string(storesCount));
        window.draw(storesLeftText);
        willBuyText.setString("Will Buy\n" + std::to_string(willBuy) + " Items");
        window.draw(willBuyText);
        window.draw(moneyText);
        if(madeTransaction)
            window.draw(rightArrow);

        if(gameClear)
            window.draw(GameClearText);
        if(gameOver)
            window.draw(GameOverText);

        window.draw(cursorSprite);
        window.display();
    }
}


// set all default items in a vector
std::vector<Item> SetUpItems() {
    std::vector<Item> items;

    Item Bow;
    Bow.texture = new sf::Texture();
    Bow.texture->loadFromFile("../assets/bow.png");
    Bow.sprite = new sf::Sprite(*Bow.texture);
    Bow.sprite->setOrigin(sf::Vector2f(0.f, 5.f));
    Bow.name = "Bow";
    Bow.id = 0;
    Bow.price = 10;
    Bow.tag = 's';
    items.push_back(Bow);

    Item HpPotion;
    HpPotion.texture = new sf::Texture();
    HpPotion.texture->loadFromFile("../assets/hppotion.png");
    HpPotion.sprite = new sf::Sprite(*HpPotion.texture);
    HpPotion.name = "Heal Potion";
    HpPotion.id = 1;
    HpPotion.price = 20;
    HpPotion.tag = 's';
    items.push_back(HpPotion);

    Item ManaPotion;
    ManaPotion.texture = new sf::Texture();
    ManaPotion.texture->loadFromFile("../assets/manapotion.png");
    ManaPotion.sprite = new sf::Sprite(*ManaPotion.texture);
    ManaPotion.name = "Mana Potion";
    ManaPotion.id = 2;
    ManaPotion.price = 40;
    ManaPotion.tag = 's';
    items.push_back(ManaPotion);
    
    Item Sword;
    Sword.texture = new sf::Texture();
    Sword.texture->loadFromFile("../assets/psword.png");
    Sword.sprite = new sf::Sprite(*Sword.texture);
    Sword.name = "Paola Sword";
    Sword.id = 3;
    Sword.price = 100;
    Sword.tag = 's';
    items.push_back(Sword);

    Item Shield;
    Shield.texture = new sf::Texture();
    Shield.texture -> loadFromFile("../assets/pshield.png");
    Shield.sprite = new sf::Sprite(*Shield.texture);
    Shield.sprite->setOrigin(sf::Vector2f(0.f, 5.f));
    Shield.name = "Paolo Shield";
    Shield.id = 4;
    Shield.price = 80;
    Shield.tag = 's';
    items.push_back(Shield);
    
    return items;
}


// int for item index in all items vector, -1 for random
Shop SetUpShop(int highItem, int lowItem){
    Shop shop;

    // SHOP BACKGROUND
    shop.texture = new sf::Texture();

    int bg = rand() % 5;
    switch(bg){
        case 0:
            shop.texture->loadFromFile("../assets/background.png");
            break;
        case 1:
            shop.texture->loadFromFile("../assets/background1.png");
            break;
        case 2:
            shop.texture->loadFromFile("../assets/background2.png");
            break;
        case 3:
            shop.texture->loadFromFile("../assets/background3.png");
            break;
        case 4:
            shop.texture->loadFromFile("../assets/background4.png");
            break;
    }
    //shop.texture->loadFromFile("../assets/background.png");
    shop.sprite = new sf::Sprite(*shop.texture);

    shop.sprite->setOrigin({0.f,0.f});
    shop.sprite->setScale({1.25f,1.25f});


    // SHOP ITEMS
    std::vector<int> indices(ALL_ITEMS.size()); // Shuffle item indices to ensure uniqueness
    std::iota(indices.begin(), indices.end(), 0); // Fill with 0 to ALL_ITEMS.size()-1
    std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device{}()));


    
    shop.lowestItemPrice = 5000;
    // add random items to the vector
    // 3 ITEMS, INCRESE TO 4 LATER
    for(int i = 0; i < 4 && i < indices.size(); i++){
        int itemNo = indices[i];
        Item item = ALL_ITEMS[itemNo].Clone();

        if(item.price < shop.lowestItemPrice)
            shop.lowestItemPrice = item.price;

        item.sprite->setScale({0.10f, 0.10f});
        shop.items.push_back(item);
    }


    // SET PRICES
    if(highItem != -1){
        shop.currentHigh = highItem;
    }
    if(lowItem != -1){
        shop.currentLow = lowItem;
    }
    shop.forecastHigh = shop.forecastLow = rand() % ALL_ITEMS.size();
    std::cout << shop.forecastHigh << std::endl;
    while (shop.forecastHigh == shop.forecastLow)
    {
        shop.forecastLow = rand() % ALL_ITEMS.size();
    }
    std::cout << shop.forecastLow << std::endl;
    

    return shop;
}


// modify the price modifier in the main item list each round
void ModifyPrices(int highItem, int lowItem){
    for (int i = 0; i < ALL_ITEMS.size(); i++)
    {
        float modifier = GetRandomIntInRange(1, 8) * 0.1f;
        if(i == highItem){
            ALL_ITEMS[i].priceModifier = modifier;
        }
        else if (i == lowItem)
        {
            ALL_ITEMS[i].priceModifier = -modifier;
        }
        else{
            if(GetRandomIntInRange(0, 1) == 0){
                ALL_ITEMS[i].priceModifier = modifier;
            }
            else{
                ALL_ITEMS[i].priceModifier = -modifier;
            }
        }
        std::cout << ALL_ITEMS[i].name.toAnsiString() + " " + std::to_string(ALL_ITEMS[i].priceModifier) << std::endl;
    }
}


// modify the prices of items in store and inventory
void ModifyItemListPrice(std::vector<Item> &items){
    for(int i = 0; i < items.size(); i++){
        items[i].price = ALL_ITEMS[items[i].id].price + ALL_ITEMS[items[i].id].price * ALL_ITEMS[items[i].id].priceModifier;
        std::cout << items[i].name.toAnsiString() + " " + std::to_string(items[i].price) << std::endl;
    }
}


void FindLowestPriceItem(Shop &shop){
    shop.lowestItemPrice = 5000;
    // add random items to the vector
    // 3 ITEMS, INCRESE TO 4 LATER
    for(int i = 0; i < shop.items.size(); i++){
        if(shop.items[i].price < shop.lowestItemPrice)
            shop.lowestItemPrice = shop.items[i].price;
    }
}


// get the item being clicked on
Item InteractWith(std::vector<Item> &items, sf::Sprite mousePos){
    Item empty;
    empty.sprite = nullptr;

    for(int i = 0; i < items.size(); i++){
        if (items[i].sprite->getGlobalBounds().findIntersection(mousePos.getGlobalBounds())){
        return items[i];
        }    
    }

    return empty;
}


int GetRandomIntInRange(int min, int max) {
    static std::random_device rd;  // only used once to initialize (seed) engine
    static std::mt19937 gen(rd()); // static to keep the same generator between calls

    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}