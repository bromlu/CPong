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

class paddle {
    public: 

    double angle;
    int radius;
    int height;
    sf::Color color;
    sf::RectangleShape rect;

    paddle(int screenWidth, int screenHeight, int radius, sf::Color color, double angle = 0, int width = 100, int height = 20) {
        this->color = color;
        this->angle = angle;
        this->radius = radius;
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
        window->draw(rect);
    }

    void moveUp() {
            angle-= 0.07;
            move();
    }

    void moveDown() {
            angle+= 0.07;
            move();
    }

    private:

    void move() {
        angle = fmod(angle, TAU);
        float X = radius + cos(angle)*(radius-height*2);
        float Y = radius + sin(angle)*(radius-height*2);
        rect.setPosition(X, Y);
        rect.setRotation(angle*180/M_PI);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 1200), "Pong");
    sf::CircleShape arena(WIDTH/2);
    arena.setFillColor(sf::Color::White);

    paddle p1 = paddle(WIDTH, HEIGHT, WIDTH/2, sf::Color::Red, 0.0);
    paddle p2 = paddle(WIDTH, HEIGHT, WIDTH/2, sf::Color::Blue, 3.14159);

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
        window.display();
    }

    return 0;
}
