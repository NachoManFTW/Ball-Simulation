#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ball.hpp"
#include "collision.hpp"

using std::cout, std::endl;

int main()
{
    //Window size


    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;

    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "Ball Simulation",sf::State::Windowed, settings);
    window.setFramerateLimit(60);
    float windowWidth  = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);

    sf::View view(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(window.getSize().x, window.getSize().y)));

    // Center of window vector coordinates
    sf::Vector2f windowCenter(windowWidth / 2, windowHeight / 2);

    //Booleans for keys pressed
    bool fPressedLastFrame = false;
    bool spacePressedLastFrame = false; // Controls 1 ball spawn per frame

    //Boolean for gravity mode
    bool fallDownMode = false; // Controls gravity(mouse centered or downward)
    //Booleans for pausing
    bool paused = false; //Check if game is paused
    bool pPressedLastFrame = false; //Check if p(pause) was pressed last frame
    bool nPressedLastFrame = false; //Check if n(frame by frame) was pressed last frame

    //Step only 1 frame
    const float fixedStep = 1.0f / 60.0f;


    //Hold all balls on screen
    std::vector<Ball> balls;
    sf::Clock clock;

    // Main Game Loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            //Close window if title bar X is hit
            if (event->is<sf::Event::Closed>())
                window.close();
            //Change visible area of window when resized
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                windowWidth = window.getSize().x;
                windowHeight = window.getSize().y;
                sf::FloatRect visibleArea({ 0.f, 0.f }, sf::Vector2f(resized->size));
                window.setView(sf::View(visibleArea));
            }
        }

        //Update realTime
        float realDT = clock.restart().asSeconds();

        //--Handle pause toggle (P)
        bool pNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
        if (pNow && !pPressedLastFrame)
        {
            paused = !paused;
            clock.restart(); // Discard dt from accumulating from current frame
        }
        pPressedLastFrame = pNow;

        //--Handle single-step(N)
        bool stepNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N);
        bool stepRequested = false;
        if (stepNow && !nPressedLastFrame)
        {
            if (paused) stepRequested = true; // Only steps when paused
        }
        nPressedLastFrame = stepNow;

        //Choose the simulation delta
        float simDT = 0.0f;
        if (!paused)
        {
            simDT = realDT; // Normal Run
        }
        else if (stepRequested)
        {
            simDT = fixedStep; // One step while paused
        }

        //Get mouse position (x,y)
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

        //--Update ball positions only when simDT > 0;
        if (simDT > 0.0f)
        {
            //--Update ball positions 
            for (auto& ball : balls)
            {
                ball.update(realDT, mousePos, windowHeight, windowWidth, fallDownMode);
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
        }

        //Clear window from last draw
        window.clear(sf::Color::Black);

        //Draw all balls in balls vector
        for (auto& ball : balls)
        {
            window.draw(ball.shape);
        }

        //--Create a green line from ball center to mouse position
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

        //Display to screen
        window.display();

        //Debug for window size change from resize
        //cout << "X: " << windowWidth << "Y: " << windowHeight << endl;

    }
}
