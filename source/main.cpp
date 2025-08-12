#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ball.hpp"
#include "collision.hpp"

using std::cout, std::endl;

int main()
{
    //Window size
    unsigned int windowWidth = 1080;
    unsigned int windowHeight = 720;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;

    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "SFML works!");
    window.setFramerateLimit(60);

    // Center of window vector coordinates
    sf::Vector2f windowCenter(windowWidth / 2, windowHeight / 2);


    bool fallDownMode = false;
    bool fPressedLastFrame = false;
    bool spacePressedLastFrame = false;

    //Hold all balls on screen
    std::vector<Ball> balls;
    sf::Clock clock;

    // Main Game Loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        //Update deltaTime
        float deltaTime = clock.restart().asSeconds();
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        //--Spawn/Delete Balls
        bool spaceNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
        if (spaceNow && !spacePressedLastFrame)
        {
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
            balls.emplace_back((float)((rand() % 50) + 5), worldPos);
        }
        spacePressedLastFrame = spaceNow;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && !balls.empty())
        {
            balls.pop_back();
        }

        //--Toggle gravity 
        bool fNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
        if (fNow && !fPressedLastFrame)
        {
            fallDownMode = !fallDownMode;
        }
        fPressedLastFrame = fNow;


        //--Update ball positions 
        for (auto& ball : balls)
        {
            ball.update(deltaTime, mousePos, windowHeight, windowWidth, fallDownMode);
            ball.setGravityStrength();
        }

        //Check if balls are colliding
        for (size_t i = 0; i < balls.size(); ++i)
        {
            for (size_t j = i + 1; j < balls.size(); ++j)
            {
                ballCollision(balls[i], balls[j]);
            }
        }




        window.clear(sf::Color::Black);



        for (auto& ball : balls)
        {
            window.draw(ball.shape);
        }

        //Create a green line from ball center to mouse position
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            for (auto& ball : balls)
            {
                sf::Vector2f ballCenter = ball.getCenter();
                sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                sf::Vertex gravityLine[] =
                {
                    {{ballCenter}, sf::Color::Green},
                    {{mouseWorldPos}, sf::Color::Green}
                };
                window.draw(gravityLine, 2, sf::PrimitiveType::Lines);
            }
        }

        window.display();

    }
}
