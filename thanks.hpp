class Thanks {
    public:

    Thanks() {}

    Thanks(int width, int height, sf::Font *font) {
        loadAssets();
        this->width = width;
        this->height = height;
        this->selected = 0;
        this->lastMoved = now();
        this->lastSelected = now();

        text1 = Text("To dl-sounds and Subspace Audio", width/2, height/5, 40, font, sf::Color::White);
        text2 = Text("for their awesome public domain music and effects!   ", width/2, height/5 + 120, 40, font, sf::Color::White);

        back = Text("Back", width/2, height/2 + 110, 75, font, sf::Color::White);

        options[0] = Option(&back, 0, true);
    }

    int update() {
        if(now() - lastMoved > (0.2)){
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

        return 3;
    }

    void draw(sf::RenderWindow *window) {
        text1.draw(window);
        text2.draw(window);

        options[0].draw(window);
    }

    private:

    void loadAssets() {
        moveBuffer.loadFromFile("assets/sfx_menu_move1.wav");
        move.setBuffer(moveBuffer);
        selectBuffer.loadFromFile("assets/sfx_menu_select1.wav");
        select.setBuffer(selectBuffer);
    }

    Text text1;
    Text text2;
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