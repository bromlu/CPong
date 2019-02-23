#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <ctime>
#include <string.h>
#include "paddle.hpp"
#include "ball.hpp"
#include "text.hpp"

#define WIDTH 1200
#define HEIGHT 1200
#define ONE_BILLION (double)1000000000.0

sf::Sound bounce;
sf::Sound score;

/* Return the current time. */
double now(void)
{
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  return current_time.tv_sec + (current_time.tv_nsec / ONE_BILLION);
}

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
        bounce.play();
    }
}

int main()
{
    sf::Font font;
    font.loadFromFile("assets/ConnectionII.otf");
    sf::Music music;
    music.openFromFile("assets/Off_Limits.wav");
    music.setVolume(50);
    music.setLoop(true);
    music.play();
    sf::SoundBuffer bounceBuffer;
    bounceBuffer.loadFromFile("assets/sfx_damage_hit1.wav");
    bounce.setBuffer(bounceBuffer);
    sf::SoundBuffer scoreBuffer;
    scoreBuffer.loadFromFile("assets/sfx_sounds_error7.wav");
    score.setBuffer(scoreBuffer);

    srand(now());
    
    Text playerOneScore("0", WIDTH - 100, 100, 150, &font, sf::Color::Red);
    Text playerTwoScore("0", 100, 100, 150, &font, sf::Color::Blue);


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
        playerOneScore.changeText(std::to_string(p1.getScore()));
        playerTwoScore.changeText(std::to_string(p2.getScore()));

        window.clear();
        window.draw(arena);
        window.draw(center);
        window.draw(rect);
        playerOneScore.draw(&window);
        playerTwoScore.draw(&window);

        p1.draw(&window);
        p2.draw(&window);

        if(now() - lastScored > (1.5)){
            ball.update();
        }
        ball.draw(&window);

        handleCollision(&ball, &p1, &window);
        handleCollision(&ball, &p2, &window);
        if(handleScoring(&ball, &p1, &p2)) {
            score.play();
            lastScored = now();
        }
        
        window.display();
    }

    return 0;
}
