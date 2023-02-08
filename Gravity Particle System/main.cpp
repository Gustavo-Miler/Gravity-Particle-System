#include <SFML/Graphics.hpp>
#include <stdlib.h>

float getMagnitude(sf::Vector2f vec) {
    float length = sqrt((vec.x * vec.x) + (vec.y * vec.y));
    return length;
}

sf::Vector2f getUnit(sf::Vector2f vec) {
    float magnitude = getMagnitude(vec);
    return sf::Vector2f(vec.x / magnitude, vec.y / magnitude);
}

int main() {
    const int particleAmount = 1000;
    int WIDTH = 1200, HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Main Window");

    struct Particle {
        sf::Vector2f pos, vel, acc;
    };

    //Initialize Particles
    sf::Vertex point;
    sf::VertexArray pointsArray(sf::Points, particleAmount);
    sf::Vector2f *pos, *vel, *acc;
    
    float maxSpeed = 0.1f;
    float attractionCoef = 2.f;

    Particle particles[particleAmount];
    for (int i = 0; i < particleAmount; i++) {
        particles[i].pos = sf::Vector2f(rand() % WIDTH, rand() % HEIGHT);
        particles[i].vel = getUnit(sf::Vector2f(rand() % 100 - 50, rand() % 100 - 50)) * maxSpeed;
        particles[i].acc = sf::Vector2f(0.f, 0.f);
        pointsArray[i].color = sf::Color::White;
    }

    //Mouse input variables
    bool mousePressed;
    sf::Vector2f mousePos;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: window.close(); break;
            
            }
        }

        window.clear(sf::Color::Black);

        //Get Mouse Input
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            mousePressed = true;
            mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        } else mousePressed = false;

        //Render Stuff
        for (int i = 0; i < particleAmount; i++) {
            pos = &particles[i].pos, vel = &particles[i].vel, acc = &particles[i].acc;
            if (mousePressed) {
                *acc += (mousePos - *pos) * attractionCoef;
            }

            *vel += *acc;

            //Clamp speed
            if (getMagnitude(*vel) > maxSpeed) *vel = getUnit(*vel) * maxSpeed;
            *pos += *vel;
            
            //Clamp position
            if (pos->x < 0.f) {
                vel->x *= -1.f;
                pos->x = 0.f;
            }
            else if (pos->x > WIDTH) {
                vel->x *= -1.f;
                pos->x = WIDTH;
            }
            if (pos->y < 0.f) {
                vel->y *= -1.f;
                pos->y = 0.f;
            }
            else if (pos->y > HEIGHT) {
                vel->y *= -1.f;
                pos->y = HEIGHT;
            }

            pointsArray[i].position = *pos;
        }

        window.draw(pointsArray);
        window.display();
        
    }
    
    return 0;
}