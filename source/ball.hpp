#pragma once
#include "SFML/Graphics.hpp"
#include <math.h>

class Ball
{
public:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	float gravity = 980.0f;
	float damping = 0.8f;
	float gravityStrength = 500.0f;

	Ball(float radius, sf::Vector2f startPos) 
	{
		shape.setRadius(radius);
		shape.setPosition(startPos);
		shape.setFillColor(sf::Color::Red);
		
		shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
		shape.setFillColor(setFillColor());
		
	}

	void update(float deltaTime, sf::Vector2i mousePos, float windowHeight, float windowWidth, bool fallDownMode)
	{
		sf::Vector2f ballPos = shape.getPosition() + sf::Vector2f(shape.getRadius(), shape.getRadius());
		sf::Vector2f direction = sf::Vector2f(mousePos) - ballPos;

		//Normalize direction
		float length = std::sqrt(pow(direction.x, 2) + pow(direction.y, 2));
		if (length != 0)
		{
			direction /= length;
		}

		sf::Vector2f acceleration = direction * gravityStrength;

		if (fallDownMode) {
			velocity.y += gravityStrength * deltaTime;
		}
		else {
			// Original gravity toward mouse
			sf::Vector2f direction = sf::Vector2f(mousePos) - getCenter();
			float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			if (length != 0) {
				direction /= length;
				velocity += direction * gravityStrength * deltaTime;
			}
		}

		shape.move(velocity * deltaTime);



		float y = shape.getPosition().y;
		float x = shape.getPosition().x;
		float radius = shape.getRadius();

		//Check collision bottom
		if (y + radius >= windowHeight)
		{
			shape.setPosition(sf::Vector2f(x, windowHeight - radius));
			velocity.y = -velocity.y * damping;
		}
		//Check collision top
		if (y - radius <= 0)
		{
			shape.setPosition(sf::Vector2f(x, radius));
			velocity.y = -velocity.y * damping;
		}
		//Check collision right
		if (x + radius >= windowWidth)
		{
			shape.setPosition(sf::Vector2f(windowWidth - radius, y));
			velocity.x = -velocity.x * damping;
		}
		//Check collision left
		if (x - radius <= 0)
		{
			shape.setPosition(sf::Vector2f(radius, y));
			velocity.x = -velocity.x * damping;
		}
	}

	sf::Vector2f getCenter() const
	{
		return sf::Vector2f(shape.getPosition().x, shape.getPosition().y);
	}

	//Edit how strong the pull is to the mouse
	void setGravityStrength()
	{
		//Increase
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && gravityStrength < 10000)
		{
			gravityStrength += 50.0f;
		}
		//Lessen
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && gravityStrength > 0)
		{
			gravityStrength -= 50.0f;
		}
	}

	//Set fill color based on radius of ball
	sf::Color setFillColor()
	{
		float radius = shape.getRadius();
		float t = std::clamp((radius - 5.0f) / 45.0f, 0.0f, 1.0f);
		sf::Color startColor = sf::Color::Blue;
		sf::Color endColor = sf::Color::Red;

		return sf::Color::Color(
			(startColor.r + t) * (endColor.r - startColor.r),
			(startColor.g + t) * (endColor.g - startColor.g),
			(startColor.b + t) * (endColor.b - startColor.b),
			255
		);
	}

};