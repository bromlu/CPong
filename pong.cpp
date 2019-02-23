#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <string.h>
#include "utilities.hpp"
#include "paddle.hpp"
#include "ball.hpp"
#include "text.hpp"
#include "game.hpp"

#define WIDTH 1200
#define HEIGHT 1200

sf::Font font;
sf::Music music;

void loadAssets() {
    font.loadFromFile("assets/ConnectionII.otf");
    music.openFromFile("assets/Off_Limits.wav");
}

int main()
{
    loadAssets();
    srand(now());

    music.setVolume(50);
    music.setLoop(true);
    music.play();
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pong");

    Game game = Game(WIDTH, HEIGHT, &font);
    
    double last = 0.0;
    while (window.isOpen())
    {
        while(now() - last < (1.0/60.0)){
            continue;
        }
        last = now();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        game.update();
        
        window.clear();
        game.draw(&window);
        window.display();
    }

    return 0;
}
