#pragma once
#include "SFML/Graphics.hpp"
#include <math.h>

using std::sqrt, std::fabs, std::fmod;

class Ball
{
public:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	float gravity = 980.0f;
	float damping = 0.8f;
	float gravityStrength = 500.0f;

	//Color Fade 
	sf::Color HSLtoRGB(float h, float s, float l) //Hue, saturation, lightness
	{
		float c = (1.f - fabs(2 * l - 1)) * s;
		float x = c * (1 - fabs(fmod(h / 60.f, 2) - 1));
		float m = l - c / 2;

		float r, g, b;
		if (h < 60) { r = c; g = x; b = 0; }
		else if (h < 120) { r = x; g = c; b = 0; }
		else if (h < 180) { r = 0; g = c; b = x; }
		else if (h < 240) { r = 0; g = x; b = c; }
		else if (h < 300) { r = x; g = 0; b = c; }
		else { r = c; g = 0; b = x; }

		return sf::Color(
			static_cast<uint8_t>((r + m) * 255),
			static_cast<uint8_t>((g + m) * 255),
			static_cast<uint8_t>((b + m) * 255)
		);
	}


	Ball(float radius, sf::Vector2f startPos) 
	{
		shape.setRadius(radius);
		shape.setPosition(startPos);
		shape.setFillColor(sf::Color::Red);
		
		shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
		
	}


	//Main update function for ball
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

		updateColorBySpeed();
		checkWindowCollision(windowHeight, windowWidth);
		
	}

	sf::Vector2f getCenter() const
	{
		return sf::Vector2f(shape.getPosition().x, shape.getPosition().y);
	}

	//Edit how strong the pull is to the mouse
	void setGravityStrength()
	{
		//Increase
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && gravityStrength < 2500)
		{
			gravityStrength += 50.0f;
		}
		//Lessen
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && gravityStrength > 0)
		{
			gravityStrength -= 50.0f;
		}
	}

	//Set ball color based on speed of ball
	void updateColorBySpeed()
	{
		float speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
		float maxSpeed = 100.f;
		
		float t = std::clamp((speed / maxSpeed) / 45.0f, 0.0f, 1.0f);
		
		//Interpolate hue: 240(blue) to 0(red)
		float hue = 240.f * (1.f - t);

		//Full saturation, medium light
		sf::Color smoothColor = HSLtoRGB(hue, 1.f, 0.5f);
		shape.setFillColor(sf::Color(smoothColor));
	}

	//Check collision of ball with window
	void checkWindowCollision(float windowHeight, float windowWidth)
	{
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
};