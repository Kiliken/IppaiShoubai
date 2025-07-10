
 #include <SFML/Graphics.hpp>

int main()
{
    unsigned int windowWidth = 640;
    unsigned int windowHeight = 360;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({windowWidth, windowHeight}), "SFML Test");
    window->setFramerateLimit(60);

    // sf::CircleShape shape(200.f);
    // shape.setFillColor(sf::Color::Green);
    // shape.setOrigin(shape.getGeometricCenter());
    // //shape.setPointCount(5);
    // shape.setPosition({0.0f, 0.0f});

    // sf::CircleShape shape1(200.f);
    // shape1.setFillColor(sf::Color::Yellow);
    // shape1.setOrigin(shape1.getGeometricCenter());
    // //shape1.setPointCount(5);
    // shape1.setPosition({windowWidth * 1.0f, windowHeight * 1.0f});

    sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);

    // define the position of the triangle's points
    triangle[0].position = sf::Vector2f(50.f, 10.f);
    triangle[1].position = sf::Vector2f(10.f, 100.f);
    triangle[2].position = sf::Vector2f(100.f, 100.f);

    // define the color of the triangle's points
    triangle[0].color = sf::Color::Red;
    triangle[1].color = sf::Color::Blue;
    triangle[2].color = sf::Color::Green;

    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window->close();
        }

        // //shape.rotate(sf::degrees(1.0f));
        // shape.move({1.0f, 1.0f});
        // //shape1.rotate(sf::degrees(-1.0f));
        // shape1.move({-1.0f, -1.0f});

        // shape.setFillColor(sf::Color::Green);
        // shape1.setFillColor(sf::Color::Yellow);

        // if(shape.getGlobalBounds().findIntersection(shape1.getGlobalBounds())){
        //     shape.setFillColor(sf::Color::Red);
        //     shape1.setFillColor(sf::Color::Red);
        // }


        window->clear();
        // window->draw(shape);
        // window->draw(shape1);
        window->draw(triangle);
        window->display();
    }
}