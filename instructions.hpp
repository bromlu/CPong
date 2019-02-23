class Instructions {
    public:

    Instructions() {}

    Instructions(int width, int height, sf::Font *font) {
        loadAssets();
        this->width = width;
        this->height = height;
        this->selected = 0;
        this->lastMoved = now();
        this->lastSelected = now();

        playerOne = Text("Player 1 (Red): Left and Right arrows to move", width/2, height/5, 50, font, sf::Color::White);
        playerTwo = Text("Player 2 (Blue): A and D to move", width/2, height/5 + 60, 50, font, sf::Color::White);
        rule = Text("Last player to hit the ball scores", width/2, height/5 + 120, 50, font, sf::Color::White);

        back = Text("Back", width/2, height/2 + 110, 75, font, sf::Color::White);

        options[0] = Option(&back, 0, true);
    }

    int update() {
        if(now() - lastMoved > (0.20)){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                options[selected].unselect();
                selected--;
                if(selected < 0) {
                    selected = 0;
                }
                options[selected].select();
                move.play();
                lastMoved = now();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                options[selected].unselect();
                selected++;
                if(selected > 0) {
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

        return 2;
    }

    void draw(sf::RenderWindow *window) {
        playerOne.draw(window);
        playerTwo.draw(window);
        rule.draw(window);

        options[0].draw(window);
    }

    private:

    void loadAssets() {
        moveBuffer.loadFromFile("assets/sfx_menu_move1.wav");
        move.setBuffer(moveBuffer);
        selectBuffer.loadFromFile("assets/sfx_menu_select1.wav");
        select.setBuffer(selectBuffer);
    }

    Text playerOne;
    Text playerTwo;
    Text rule;
    Text back;
    Option options[1];
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