#define TAU (double)6.28

class Paddle {
    public: 

    Paddle() {}

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