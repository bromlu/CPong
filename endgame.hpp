class Endgame { //We are in the endgame now - Dr. Strange
    public:

    Endgame() {}

    void init(int width, int height, sf::Font *font, std::string winningPlayer) {
        loadAssets();
        this->width = width;
        this->height = height;
        this->selected = 0;
        this->lastMoved = now();
        this->lastSelected = now();

        win = Text("Congratulations" + winningPlayer, width/2, height/5, 50, font, sf::Color::White);

        playAgain = Text("Play Again", width/2, height/2, 75, font, sf::Color::White);
        back = Text("Back to Menu", width/2, height/2 + 110, 75, font, sf::Color::White);

        options[0] = Option(&playAgain, 1, true);
        options[1] = Option(&back, 0, false);
    }

    int update() {
        if(now() - lastMoved > (0.20)){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                options[selected].unselect();
                selected--;
                if(selected < 0) {
                    selected = 1;
                }
                options[selected].select();
                move.play();
                lastMoved = now();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                options[selected].unselect();
                selected++;
                if(selected > 1) {
                    selected = 0;
                }
                options[selected].select();
                move.play();
                lastMoved = now();
            }
        }
        if(now() - lastSelected > (0.5)){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                lastSelected = now();
                select.play();
                return options[selected].getState();
            }
        }

        return 4;
    }

    void draw(sf::RenderWindow *window) {
        win.draw(window);

        options[0].draw(window);
        options[1].draw(window);
    }

    private:

    void loadAssets() {
        moveBuffer.loadFromFile("assets/sfx_menu_move1.wav");
        move.setBuffer(moveBuffer);
        selectBuffer.loadFromFile("assets/sfx_menu_select1.wav");
        select.setBuffer(selectBuffer);
    }

    Text win;
    Text playAgain;
    Text back;
    Option options[2];
    sf::Sound move;
    sf::Sound select;
    sf::SoundBuffer moveBuffer;
    sf::SoundBuffer selectBuffer;

    int width;
    int height;
    int selected;
    double lastMoved;
    double lastSelected;
};