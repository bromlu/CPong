class Text {
    public: 

    Text(std::string text, float x, float y, float size, sf::Font *font, sf::Color color) {
        this->text = sf::Text();
        this->text.setFont(*font);
        this->text.setString(text);
        this->text.setCharacterSize(size);
        this->text.setFillColor(color);
        this->text.setPosition(center(x, y, size, text));
        this->x = x;
        this->y = y;
        this->size = size;
    }

    void changeText(std::string text) {
        this->text.setString(text);
        this->text.setPosition(center(x, y, size, text));
    }

    void draw(sf::RenderWindow *window) {
        window->draw(this->text);
    }

    private:

    sf::Vector2f center(float x, float y, float size, std::string text) {
        size_t len = text.length();
        float width = len * size;
        return sf::Vector2f(x - (width/4), y - (size/3*2));
    }
    
    sf::Text text;
    float size;
    float x;
    float y;
};