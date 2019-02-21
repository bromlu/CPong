#include <SFML/Graphics.hpp>

#define SQUARES 100

int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "Pong");
    sf::CircleShape shape(200.f);
    sf::CircleShape dot(20.f);
    shape.setFillColor(sf::Color::Green);
    dot.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.draw(dot);
        window.display();
    }

    return 0;
}
