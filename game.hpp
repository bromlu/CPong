class Game {
    public:

    Game(int width, int height, sf::Font *font) {
        loadAssets();
        this->width = width;
        this->height = height;

        playerOneScore = Text("0", width - 100, 100, 150, font, sf::Color::Red);
        playerTwoScore = Text("0", 100, 100, 150, font, sf::Color::Blue);

        initArena(width, height);

        playerOne = Paddle("Player 1", 1, width, height, width/2, sf::Color::Red, 0.0);
        playerTwo = Paddle("Player 2", 2, width, height, width/2, sf::Color::Blue, M_PI);
        ball = Ball(width, height, sf::Color::White, 10.0, 8.0);

        lastScored = now();
    }

    void update() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            playerOne.moveDown(&playerTwo);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            playerTwo.moveDown(&playerOne);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            playerOne.moveUp(&playerTwo);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            playerTwo.moveUp(&playerOne);
        }
        playerOneScore.changeText(std::to_string(playerOne.getScore()));
        playerTwoScore.changeText(std::to_string(playerTwo.getScore()));

        if(now() - lastScored > (1.5)){
            ball.update();
        }


        handleCollision(&ball, &playerOne);
        handleCollision(&ball, &playerTwo);
        if(handleScoring(&ball, &playerOne, &playerTwo)) {
            this->score.play();
            lastScored = now();
        }
    }

    void draw(sf::RenderWindow *window) {
        window->draw(arena);
        window->draw(center);
        window->draw(rect);

        playerOneScore.draw(window);
        playerTwoScore.draw(window);

        playerOne.draw(window);
        playerTwo.draw(window);
        ball.draw(window);
    }

    private:

    void initArena(int width, int height) {
        arena = sf::CircleShape(width/2);
        arena.setOutlineColor(sf::Color::White);
        arena.setOutlineThickness(3.0);
        arena.setFillColor(sf::Color::Black);

        center = sf::CircleShape(40.0);
        center.setPosition(width/2 - 40.0, height/2 - 40.0);
        center.setOutlineColor(sf::Color::White);
        center.setOutlineThickness(3.0);
        center.setFillColor(sf::Color::Black);

        rect = sf::RectangleShape(sf::Vector2f(40.0 * 2, 40.0 * 2));
        rect.setPosition(width/2 - 40.0, height/2 - 40.0);
        rect.setFillColor(sf::Color::Black);
    }

    bool handleScoring(Ball *ball, Paddle *p1, Paddle *p2) {
        float centerX = width/2.0;
        float centerY = height/2.0;
        float x = ball->getX();
        float y = ball->getY();

        float distance = sqrt(pow(abs(centerX - x),2.0) + pow(abs(centerY - y),2.0));

        if(distance > width/2) {
            if(ball->getOwnerId() == p1->getId()) {
                p1->increaseScore();
            } else if(ball->getOwnerId() == p2->getId()) {
                p2->increaseScore();
            }
            ball->setupBall();
            return true;
        }
        return false;
    }

    void handleCollision(Ball *ball, Paddle *paddle) {
        float centerX = paddle->getCenterX();
        float centerY = paddle->getCenterY();
        float angle = -paddle->getAngle();
        float ballx = ball->getX();
        float bally = ball->getY();

        float unrotatedCircleX = cos(angle) * (ballx - centerX) - sin(angle) * (bally - centerY) + centerX;
        float unrotatedCircleY  = sin(angle) * (ballx - centerX) + cos(angle) * (bally - centerY) + centerY;
        paddle->setCollision(unrotatedCircleX, unrotatedCircleY, ball->getRadius());
        if(paddle->getCollision()) {
            ball->setColor(paddle->getColor());
            ball->setOwnerId(paddle->getId());
            ball->bounce(paddle->getCenterX(), paddle->getCenterY());
            this->bounce.play();
        }
    }

    void loadAssets() {
        bounceBuffer.loadFromFile("assets/sfx_damage_hit1.wav");
        bounce.setBuffer(bounceBuffer);
        scoreBuffer.loadFromFile("assets/sfx_sounds_error7.wav");
        score.setBuffer(scoreBuffer);
    }

    Text playerOneScore;
    Text playerTwoScore;
    sf::Sound bounce;
    sf::Sound score;
    sf::SoundBuffer scoreBuffer;
    sf::SoundBuffer bounceBuffer;
    sf::CircleShape arena;
    sf::CircleShape center;
    sf::RectangleShape rect;
    Paddle playerOne;
    Paddle playerTwo;
    Ball ball;

    int width;
    int height;
    double lastScored;
};