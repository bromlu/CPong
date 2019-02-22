#include <SFML/Graphics.hpp>
#include <math.h>
#include <ctime>

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

        setupBall();
    }

    void update() {
        float centerX = screenWidth/2.0;
        float centerY = screenHeight/2.0;
        float x = circle.getPosition().x;
        float y = circle.getPosition().y;

        float distance = sqrt(pow(abs(centerX - x),2.0) + pow(abs(centerY - y),2.0));

        if(distance > screenWidth/2) {
            setupBall();
        } else {
            circle.move(vx, vy);
        }
    }

    void draw(sf::RenderWindow *window) {
        window->draw(circle);
    }

    void bounce() {
        vx = vx * -1;
        vy = vy * -1;
    }

    float getX() {
        return circle.getPosition().x;
    }

    float getY() {
        return circle.getPosition().y;
    }

    float getRadius() {
        return radius;
    }

    private:

    void setupBall() {
        circle.setPosition(screenHeight/2, screenWidth/2);
        int angle = rand() % 360;
        vx = cos(angle * M_PI / 180.0) * speed;
        vy = sin(angle * M_PI / 180.0) * speed;
    }

    sf::CircleShape circle;
    sf::Color color;
    int vx;
    int vy;
    int screenWidth;
    int screenHeight;
    float radius;
    float speed;
};

class Paddle {
    public: 

    Paddle(int screenWidth, int screenHeight, int radius, sf::Color color, double angle = 0, int width = 100, int height = 20) {
        this->color = color;
        this->angle = angle;
        this->radius = radius;
        this->width = width;
        this->height = height;
        this->rect = sf::RectangleShape(sf::Vector2f(height, width));
        this->rect.setFillColor(color);

        double X = radius + cos(angle)*(radius-height*2);
        double Y = radius + sin(angle)*(radius-height*2);
        this->rect.setPosition(X, Y);
        this->rect.setOrigin(sf::Vector2f(height/2, width/2));
        this->rect.setRotation(angle*180/M_PI);
    }

    void draw(sf::RenderWindow *window) {
        window->draw(this->rect);
    }

    void moveUp() {
            angle-= 0.05;
            move();
    }

    void moveDown() {
            angle+= 0.05;
            move();
    }

    bool isInside(float x, float y, float radius) {
        double closestX, closestY;
 
        // Find the unrotated closest x point from center of unrotated circle
        if (x  < this->getX())
            closestX = this->getX();
        else if (x  > this->getX() + this->width)
            closestX = this->getX() + this->width;
        else
            closestX = x ;
        
        // Find the unrotated closest y point from center of unrotated circle
        if (y < this->getY())
            closestY = this->getY();
        else if (y > this->getY() + this->height)
            closestY = this->getY() + this->height;
        else
            closestY = y;
        
        
        float distance = sqrt(pow(abs(closestX - x),2.0) + pow(abs(closestY - y),2.0));
        if (distance < radius)
            return true;
        else
            return false;
    }

    double getAngle() {
        return angle;
    }

    float getX() {
        return rect.getPosition().x;
    }

    float getY() {
        return rect.getPosition().y;
    }

    float getCenterX() {
        return rect.getPosition().x + (width / 2);
    }

    float getCenterY() {
        return rect.getPosition().y + (height / 2);
    }

    private:

    double angle;
    int radius;
    int height;
    int width;
    sf::Color color;
    sf::RectangleShape rect;

    void move() {
        angle = fmod(angle, TAU);
        float X = radius + cos(angle)*(radius-height*2);
        float Y = radius + sin(angle)*(radius-height*2);
        rect.setPosition(X, Y);
        rect.setRotation(angle*180/M_PI);
    }
};

bool checkCollision(Ball *ball, Paddle *paddle, sf::RenderWindow *window) {
    float centerX = paddle->getCenterX();
    float centerY = paddle->getCenterY();
    float angle = paddle->getAngle() * -1;
    float ballx = ball->getX();
    float bally = ball->getY();
    // printf("%f", angle);

    float unrotatedCircleX;
    float unrotatedCircleY;

    unrotatedCircleX = cos(angle) * (ballx - centerX) - sin(angle) * (bally - centerY) + centerX;
    unrotatedCircleY  = sin(angle) * (ballx - centerX) + cos(angle) * (bally - centerY) + centerY;

    sf::CircleShape test(10.0f);
    test.setPosition(unrotatedCircleX, unrotatedCircleY);
    test.setFillColor(sf::Color::Yellow);
    window->draw(test);

    // Axis1.x = A.UR.x - A.UL.x 
    // Axis1.y = A.UR.y - A.UL.y 
    // Axis2.x = A.UR.x - A.LR.x 
    // Axis2.y = A.UR.y - A.LR.y 
    // Axis3.x = B.UL.x - B.LL.x 
    // Axis3.y = B.UL.y - B.LL.y 
    // Axis4.x = B.UL.x - B.UR.x 
    // Axis4.y = B.UL.y - B.UR.y


    return paddle->isInside(unrotatedCircleX, unrotatedCircleY, ball->getRadius());
}

int main()
{
    srand(now());

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pong");
    sf::CircleShape arena(WIDTH/2);
    arena.setOutlineColor(sf::Color::White);
    arena.setOutlineThickness(3.0);
    arena.setFillColor(sf::Color::Black);

    Paddle p1 = Paddle(WIDTH, HEIGHT, WIDTH/2, sf::Color::Red, 0.0, 200);
    Paddle p2 = Paddle(WIDTH, HEIGHT, WIDTH/2, sf::Color::Blue, 3.14159, 200);
    Ball ball = Ball(WIDTH, HEIGHT, sf::Color::White, 10.0, 5.0);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            p1.moveDown();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            p1.moveUp();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            p2.moveDown();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            p2.moveUp();
        }

        window.clear();
        window.draw(arena);
        p1.draw(&window);
        p2.draw(&window);
        ball.update();
        ball.draw(&window);

        if(checkCollision(&ball, &p1, &window)) {
            ball.bounce();
        }
        // if(checkCollision(&ball, &p2, &window)) {
        //     ball.bounce();
        // }

        window.display();
    }

    return 0;
}
