#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>

float getMagnitude(sf::Vector2f vec) {
    float length = sqrt((vec.x * vec.x) + (vec.y * vec.y));
    return length;
}

sf::Vector2f getUnit(sf::Vector2f vec) {
    float magnitude = getMagnitude(vec);
    return sf::Vector2f(vec.x / magnitude, vec.y / magnitude);
}

int main() {
    const int particleAmount = 25000;
    int WIDTH = 1600, HEIGHT = 900;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Main Window");

    struct Particle {
        sf::Vector2f pos, vel, acc;
    };

    //Initialize Particles
    sf::Vertex point;
    sf::VertexArray pointsArray(sf::Points, particleAmount);
    sf::Vector2f *pos, *vel, *acc;
    
    float maxSpeed = 0.4f;
    float maxAcc = 0.1f;
    float attractionCoef = 4.f;
    float dist;
    float maxDist = 200.f;
    float minDist = 100.f;

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

    //Keyboard variables
    int current_var = 0;
    float delta = 0.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: window.close(); break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                    case sf::Keyboard::Num1: current_var = 0; std::cout << "Max Speed" << std::endl; break;
                    case sf::Keyboard::Num2: current_var = 1; std::cout << "Attraction" << std::endl; break;
                    case sf::Keyboard::Num3: current_var = 2; std::cout << "Min Dist" << std::endl; break;
                    case sf::Keyboard::Num4: current_var = 3; std::cout << "Max Dist" << std::endl; break;
                    case sf::Keyboard::Num5: current_var = 4; std::cout << "Max Acc" << std::endl; break;
                    case sf::Keyboard::R:
                        for (int i = 0; i < particleAmount; i++) {
                            particles[i].pos = sf::Vector2f(rand() % WIDTH, rand() % HEIGHT);
                            particles[i].vel = getUnit(sf::Vector2f(rand() % 100 - 50, rand() % 100 - 50)) * maxSpeed;
                            particles[i].acc = sf::Vector2f(0.f, 0.f);
                            pointsArray[i].color = sf::Color::White;
                        }; break;
                    }; break;
                case sf::Event::MouseWheelScrolled:
                    delta = (0 < event.mouseWheel.x) - (event.mouseWheel.x < 0);
                    switch (current_var) {
                        case 0: maxSpeed += delta * 0.01f; std::cout << "Max Speed: " << maxSpeed << std::endl; break;
                        case 1: attractionCoef += delta * 1.f; std::cout << "Attraction: " << attractionCoef << std::endl; break;
                        case 2: minDist += delta * 5.f; std::cout << "Min Dist: " << minDist << std::endl; break;
                        case 3: maxDist += delta * 5.f; std::cout << "Max Dist: " << maxDist << std::endl; break;
                        case 4: maxAcc += delta * 0.01f; std::cout << "Max Acc: " << maxAcc << std::endl; break;
                    }; break;
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
                dist = getMagnitude(mousePos - *pos);
                if (dist > maxDist) {
                    dist = maxDist;
                }
                else if (dist < minDist) dist = minDist;
                *acc += getUnit(mousePos - *pos) * (attractionCoef / (dist * dist));
            }

            //Clamp acc
            if (getMagnitude(*acc) > maxAcc) {
                *acc = getUnit(*acc) * maxAcc;
            }

            *vel += *acc;

            //Clamp speed
            if (getMagnitude(*vel) > maxSpeed) *vel = getUnit(*vel) * maxSpeed;
            *pos += *vel;
            
            //Clamp position
            if (pos->x < 0 || pos->x > WIDTH) {
                vel->x *= -1;
                acc->x *= -1;
            }
            if (pos->y < 0 || pos->y > HEIGHT) {
                vel->y *= -1;
                acc->y *= -1;
            }

            pointsArray[i].position = *pos;
        }

        window.draw(pointsArray);
        window.display();
        
    }

    return 0;
}