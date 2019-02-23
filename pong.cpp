#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <string.h>
#include "utilities.hpp"
#include "paddle.hpp"
#include "ball.hpp"
#include "text.hpp"
#include "option.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "instructions.hpp"
#include "thanks.hpp"
#include "endgame.hpp"

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

    Menu menu = Menu(WIDTH, HEIGHT, &font);
    Instructions instructions;
    Thanks thanks;
    Endgame endgame;
    Game game;
    
    int state = 0;
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
        window.clear();

        switch (state)
        {
            case 0:
                state = menu.update();
                if(state == 1) {
                    game.init(WIDTH, HEIGHT, &font);
                } else if(state == 2) {
                    instructions = Instructions(WIDTH, HEIGHT, &font);
                } else if(state == 3) {
                    thanks = Thanks(WIDTH, HEIGHT, &font);;
                }
                menu.draw(&window);
                break;
            case 1:
                game.draw(&window);
                state = game.update();
                if(state == 4) {
                    endgame.init(WIDTH, HEIGHT, &font, "Player 1");
                } else if(state == 5) {
                    endgame.init(WIDTH, HEIGHT, &font, "Player 2");
                } 
                break;
            case 2:
                instructions.draw(&window);
                state = instructions.update();
                if(state == 0) {
                    menu.setLastSelected();
                }
                break;
            case 3:
                thanks.draw(&window);
                state = thanks.update();
                if(state == 0) {
                    menu.setLastSelected();
                }
                break;
            case 4:
            case 5:
                endgame.draw(&window);
                state = endgame.update();
                if(state == 0) {
                    menu.setLastSelected();
                } else if(state == 1) {
                    game.init(WIDTH, HEIGHT, &font);
                }
                break;
            default:
                exit(0);
                break;
        }
        window.display();
    }

    return 0;
}
