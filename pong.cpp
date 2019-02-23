#include <SFML/Graphics.hpp>
#include <math.h>
#include <ctime>
#include <string.h>

#define WIDTH 1200
#define HEIGHT 1200
#define ONE_BILLION (double)1000000000.0
#define TAU (double)6.28

/* Return the current time. */
double now(void)
{
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  return current_time.tv_sec + (current_time.tv_nsec / ONE_BILLION);
}

class Ball {
    public:

    Ball(int screenWidth, int screenHeight, sf::Color color, float radius = 10.0, float speed = 10.0) {
        this->circle = sf::CircleShape(radius);
        this->circle.setFillColor(color);
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        this->speed = speed;
        this->radius = radius;
        this->color = color;
        this->ownerId = 0;

        setupBall();
    }

    void update() {
        circle.move(vx, vy);
    }

    void draw(sf::RenderWindow *window) {
        window->draw(circle);
    }

    void bounce(float paddleX, float paddleY) {
        float relativeX = paddleX - screenWidth/2;
        float relativeY = paddleY - screenHeight/2;
        float normalize = sqrt(relativeX * relativeX + relativeY * relativeY);
        float centerX = (relativeX / normalize) * 80;
        float centerY = (relativeY / normalize) * 80;

        relativeX = getX() - (paddleX + centerX);
        relativeY = getY() - (paddleY + centerY);
        normalize = sqrt(relativeX * relativeX + relativeY * relativeY);
        vx = relativeX/normalize * speed;
        vy = relativeY/normalize * speed;
    }

    void setColor(sf::Color color) {
        circle.setFillColor(color);
    }

    void setOwnerId(int ownerId) {
        this->ownerId = ownerId;
    }

    int getOwnerId() {
        return ownerId;
    }

    float getX() {
        return circle.getPosition().x + radius;
    }

    float getY() {
        return circle.getPosition().y + radius;
    }

    float getRadius() {
        return radius;
    }

    void setupBall() {
        circle.setFillColor(color);
        circle.setPosition(screenHeight/2-radius, screenWidth/2-radius);
        angle = rand() % 360 * M_PI / 180.0;
        vx = speed * cos(angle);
        vy = speed * -sin(angle);
        ownerId = 0;
    }

    private:

    sf::CircleShape circle;
    sf::Color color;
    int ownerId;
    int screenWidth;
    int screenHeight;
    float angle;
    float vx;
    float vy;
    float radius;
    float speed;
};

class Paddle {
    public: 

    Paddle(std::string name, int id, int screenWidth, int screenHeight, int radius, sf::Color color, float angle = 0, int width = 20, int height = 200) {
        this->color = color;
        this->angle = angle;
        this->radius = radius;
        this->width = width;
        this->height = height;
        this->collision = false;
        this->hadCollision = false;
        this->collideUp = false;
        this->collideDown = false;
        this->rect = sf::RectangleShape(sf::Vector2f(width, height));
        this->rect.setFillColor(color);
        this->rect.setOrigin(sf::Vector2f(width/2, height/2));
        this->name = name;
        this->id = id;
        this->score = 0;

        move();
    }

    void draw(sf::RenderWindow *window) {
        window->draw(this->rect);
    }

    void moveUp(Paddle *paddle = NULL) {
        if(paddle == NULL) {
            angle -= 0.03;
        } else {
            bool collision = paddleCollision(paddle);
            if(collideDown) {
                moveUp();
            } else if(collision && !paddle->getCollideUp()) {
                paddle->moveUp();
                collideUp = true;
            } else {
                moveUp();
            }
        }
        move();
    }

    void moveDown(Paddle *paddle = NULL) {
        if(paddle == NULL) {
            angle += 0.03;
        } else {
            bool collision = paddleCollision(paddle);
            if(collideUp) {
                moveDown();
            } else if(collision && !paddle->getCollideDown()) {
                paddle->moveDown();
                collideDown = true;
            } else {
                moveDown();
            }
        }
        move();
    }

    void setCollision(float x, float y, float radius) {
        float closestX, closestY;
 
        if (x  < this->getX())
            closestX = this->getX();
        else if (x  > this->getX() + this->width)
            closestX = this->getX() + this->width;
        else
            closestX = x ;
        
        if (y < this->getY())
            closestY = this->getY();
        else if (y > this->getY() + this->height)
            closestY = this->getY() + this->height;
        else
            closestY = y;        
        
        float distance = sqrt(pow(abs(closestX - x),2.0) + pow(abs(closestY - y),2.0));
        if (distance < radius) {
            if(hadCollision == true) {
                collision = false;
            } else {
                collision = true;
            }
            hadCollision = true;
        }
        else {
            hadCollision = false;
            collision = false;
        }
    }

    void increaseScore() {
        this->score++;
    }

    int getScore() {
        return this->score;
    }

    float getAngle() {
        return rect.getRotation() * (M_PI / 180);
    }

    sf::Color getColor() {
        return color;
    }

    float getX() {
        return rect.getPosition().x - width/2;
    }

    float getY() {
        return rect.getPosition().y - height/2;
    }

    float getCenterX() {
        return rect.getPosition().x;
    }

    float getCenterY() {
        return rect.getPosition().y;
    }

    bool getCollision() {
        return collision;
    }

    bool getCollideDown() {
        return collideDown;
    }

    bool getCollideUp() {
        return collideUp;
    }

    int getId() {
        return id;
    }

    void resetCollision() {
        collideDown = false;
        collideUp = false;
    }

    private:

    bool collision;
    bool hadCollision;
    bool collideDown;
    bool collideUp;
    float angle;
    int radius;
    int width;
    int height;
    int score;
    int id;
    std::string name;
    sf::Color color;
    sf::RectangleShape rect;

    bool paddleCollision(Paddle *paddle) {
        float x1 = this->getCenterX();
        float y1 = this->getCenterY();
        float x2 = paddle->getCenterX();
        float y2 = paddle->getCenterY();
        float distance = sqrt(pow(abs(x1 - x2),2.0) + pow(abs(y1 - y2),2.0));
        if(distance <= 212.3) {
            return true;
        } else if (distance >= 700.0) {
            resetCollision();
            paddle->resetCollision();
        }
        return false;
    }

    void move() {
        angle = fmod(angle, TAU);
        float X = radius + cos(angle)*(radius-width*2);
        float Y = radius + sin(angle)*(radius-width*2);
        rect.setPosition(X, Y);
        rect.setRotation(angle*180/M_PI);
    }
};

bool handleScoring(Ball *ball, Paddle *p1, Paddle *p2) {
    float centerX = WIDTH/2.0;
    float centerY = HEIGHT/2.0;
    float x = ball->getX();
    float y = ball->getY();

    float distance = sqrt(pow(abs(centerX - x),2.0) + pow(abs(centerY - y),2.0));

    if(distance > WIDTH/2) {
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

void handleCollision(Ball *ball, Paddle *paddle, sf::RenderWindow *window) {
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
    }
}

int main()
{
    sf::Font font;
    if (!font.loadFromFile("assets/ConnectionII.otf"))
    {
        printf("Error loading fonts.\n");
        exit(1);
    }

    sf::Text PlayerOneScoreText;
    PlayerOneScoreText.setFont(font);
    PlayerOneScoreText.setString("0");
    PlayerOneScoreText.setCharacterSize(150);
    PlayerOneScoreText.setFillColor(sf::Color::Red);
    PlayerOneScoreText.setPosition(WIDTH - 180, 5);

    sf::Text PlayerTwoScoreText;
    PlayerTwoScoreText.setFont(font);
    PlayerTwoScoreText.setString("0");
    PlayerTwoScoreText.setCharacterSize(150);
    PlayerTwoScoreText.setFillColor(sf::Color::Blue);
    PlayerTwoScoreText.setPosition(50, 5);

    srand(now());

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pong");
    sf::CircleShape arena(WIDTH/2);
    arena.setOutlineColor(sf::Color::White);
    arena.setOutlineThickness(3.0);
    arena.setFillColor(sf::Color::Black);

    sf::CircleShape center(40.0);
    center.setPosition(WIDTH/2 - 40.0, HEIGHT/2 - 40.0);
    center.setOutlineColor(sf::Color::White);
    center.setOutlineThickness(3.0);
    center.setFillColor(sf::Color::Black);

    sf::RectangleShape rect(sf::Vector2f(40.0 * 2, 40.0 * 2));
    rect.setPosition(WIDTH/2 - 40.0, HEIGHT/2 - 40.0);
    rect.setFillColor(sf::Color::Black);

    Paddle p1 = Paddle("Player 1", 1, WIDTH, HEIGHT, WIDTH/2, sf::Color::Red, 0.0);
    Paddle p2 = Paddle("Player 2", 2, WIDTH, HEIGHT, WIDTH/2, sf::Color::Blue, M_PI);
    Ball ball = Ball(WIDTH, HEIGHT, sf::Color::White, 10.0, 8.0);

    double last = 0.0;
    double lastScored = now();
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            p1.moveDown(&p2);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            p2.moveDown(&p1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            p1.moveUp(&p2);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            p2.moveUp(&p1);
        }
        PlayerOneScoreText.setString(std::to_string(p1.getScore()));
        PlayerTwoScoreText.setString(std::to_string(p2.getScore()));

        window.clear();
        window.draw(PlayerOneScoreText);
        window.draw(PlayerTwoScoreText);
        window.draw(arena);
        window.draw(center);
        window.draw(rect);

        p1.draw(&window);
        p2.draw(&window);

        if(now() - lastScored > (1.5)){
            ball.update();
        }
        ball.draw(&window);

        handleCollision(&ball, &p1, &window);
        handleCollision(&ball, &p2, &window);
        if(handleScoring(&ball, &p1, &p2)) {
            lastScored = now();
        }
        
        window.display();
    }

    return 0;
}
