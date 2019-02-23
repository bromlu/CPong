class Option {
    public: 

    Option() {}

    Option(Text *text, int state, bool selected) {
        this->text = text;
        this->state = state;
        if(selected) {
            select();
        }
    }

    void select() {
        text->changeColor(sf::Color::Green);
    }

    void unselect() {
        text->changeColor(sf::Color::White);
    }

    int getState() {
        return state;
    }

    void draw(sf::RenderWindow *window) {
        text->draw(window);
    }

    private:

    Text *text;
    int state;
};