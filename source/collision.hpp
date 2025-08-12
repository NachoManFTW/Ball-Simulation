#pragma once
#include "SFML/Graphics.hpp"
#include "ball.hpp"
#include <cmath>

void ballCollision(Ball& a, Ball& b)
{
	sf::Vector2f delta = b.getCenter() - a.getCenter(); //Mid point between 2 balls
	float dist = std::sqrt(pow(delta.x, 2) + pow(delta.y, 2)); //Distance from midpoint
	float minDist = a.shape.getRadius() + b.shape.getRadius(); //Min distance needed between both balls
	

	if (dist < minDist && dist != 0)
	{
		//Normalize collision axis
		sf::Vector2f normal = delta / dist;

		//Relative velocity
		sf::Vector2f relativeVelocity = b.velocity - a.velocity;

		//Velocity along the noraml
		float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

		//Skip if balls are moving away
		if (velocityAlongNormal > 0)
			return;

		//Simple elastic collision (equals mass)
		float restitution = 0.8f; // 1.0 = perfect bounce, <1 = energy loss
		float impulse = -(1 + restitution) * velocityAlongNormal / 2.0f;

		sf::Vector2f impulseVec = impulse * normal;

		a.velocity -= impulseVec;
		b.velocity += impulseVec;

		//Resolve overlap
		sf::Vector2f correction = normal * (minDist - dist) / 2.0f;
		a.shape.move(-correction);
		b.shape.move(correction);
	}

}