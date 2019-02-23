#include <SFML/Graphics.hpp>
#include <math.h>
#include <string.h>

class Ball {
    public:

    Ball() {}

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