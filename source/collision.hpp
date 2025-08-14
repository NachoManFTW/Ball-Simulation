#pragma once
#include "SFML/Graphics.hpp"
#include "ball.hpp"
#include <cmath>

void ballCollisionElastic(Ball& a, Ball& b)
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

//Wrapper for elastic collision, so parameters match other collision
void ballCollisionElasticWrapper(std::vector<Ball>& balls, size_t i, size_t j)
{
	ballCollisionElastic(balls[i], balls[j]);
}


void ballCollisionPerfectlyElastic(Ball& a, Ball& b)
{
	sf::Vector2f delta = b.getCenter() - a.getCenter();
	float dist = sqrt(pow(delta.x, 2) + pow(delta.y, 2));
	float minDist = a.shape.getRadius() + b.shape.getRadius();

	if (dist < minDist && dist != 0)
	{
		//Normal collision axis
		sf::Vector2f normal = delta / dist;

		//Project velocitiews onto the normal
		float aNormal = a.velocity.x * normal.x + a.velocity.y * normal.y;
		float bNormal = b.velocity.x * normal.x + b.velocity.y * normal.y;

		//Swap normal components (equal mass)
		float temp = aNormal;
		aNormal = bNormal;
		bNormal = temp;

		//Compute tangential components
		sf::Vector2f aTangent = a.velocity - normal * temp;
		sf::Vector2f bTangent = b.velocity - normal * bNormal;

		//Reconstruct final velocities
		a.velocity = aTangent + normal * aNormal;
		b.velocity = bTangent + normal * bNormal;

		//Resolve overlap
		sf::Vector2 correction = normal * (minDist - dist) / 2.0f;
		a.shape.move(-correction);
		b.shape.move(correction);
	}
}

void ballCollisionPerfectlyElasticWrapper(std::vector<Ball>& balls, size_t i, size_t j)
{
	ballCollisionPerfectlyElastic(balls[i], balls[j]);
}

void ballCollisionMerge(std::vector<Ball>& balls, size_t i, size_t j)
{
	Ball& a = balls[i];
	Ball& b = balls[j];

	sf::Vector2f delta = b.getCenter() - a.getCenter();
	float dist = sqrt(pow(delta.x, 2) + pow(delta.y, 2));
	float minDist = a.shape.getRadius() + b.shape.getRadius();

	if (dist < minDist && dist != 0)
	{
		//Compute new radius based on area
		float r1 = a.shape.getRadius();
		float r2 = b.shape.getRadius();
		float newRadius = sqrt(pow(r1, 2) + pow(r2, 2));//Perserves area

		//Compute new velocity (momentum)
		sf::Vector2f newVelocity = (a.velocity + b.velocity) / 2.0f;

		//Compute new position (ball-center)
		sf::Vector2f newPos = a.shape.getRadius() > b.shape.getRadius() ?  a.getCenter() : b.getCenter();


		//Create merged ball
		Ball merged(newRadius, newPos);
		merged.velocity = newVelocity;

		//Replace ball with merged, and remove ball b
		balls[i] = merged;
		balls.erase(balls.begin() + j);
	}
}
