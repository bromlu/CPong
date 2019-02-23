class Menu {
    public:

    Menu(int width, int height, sf::Font *font) {
        loadAssets();
        this->width = width;
        this->height = height;
        this->selected = 0;
        this->lastMoved = now();
        this->lastSelected = now();

        title = Text("Spike Pong", width/2, height/5, 150, font, sf::Color::White);
        by = Text("By Luke Brom", width/2, height/5 + 120, 40, font, sf::Color::White);

        play = Text("Play", width/2, height/2 - 110, 75, font, sf::Color::White);
        instructions = Text(" Instructions", width/2, height/2 - 35, 75, font, sf::Color::White);
        thanks = Text("Thanks ", width/2, height/2 + 35, 75, font, sf::Color::White);
        exit = Text("Exit", width/2, height/2 + 110, 75, font, sf::Color::White);

        options[0] = Option(&play, 1, true);
        options[1] = Option(&instructions, 2, false);
        options[2] = Option(&thanks, 3, false);
        options[3] = Option(&exit, 6, false);
    }

    void setLastSelected() {
        this->lastSelected = now();
    }

    int update() {
        if(now() - lastMoved > (0.2)){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                options[selected].unselect();
                selected--;
                if(selected < 0) {
                    selected = 3;
                }
                options[selected].select();
                move.play();
                lastMoved = now();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                options[selected].unselect();
                selected++;
                if(selected > 3) {
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

        return 0;
    }

    void draw(sf::RenderWindow *window) {
        title.draw(window);
        by.draw(window);

        for(int i = 0; i < 4; i++) {
            options[i].draw(window);
        }
    }

    private:

    void loadAssets() {
        moveBuffer.loadFromFile("assets/sfx_menu_move1.wav");
        move.setBuffer(moveBuffer);
        selectBuffer.loadFromFile("assets/sfx_menu_select1.wav");
        select.setBuffer(selectBuffer);
    }

    Text title;
    Text by;
    Text play;
    Text instructions;
    Text thanks;
    Text exit;
    Option options[4];
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