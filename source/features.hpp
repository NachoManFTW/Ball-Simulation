#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

#include "Ball.hpp"

using std::vector;


// Spawn balls with space bar
bool spawnBall(bool& spaceNow, bool& spacePressLastFrame, vector<Ball>& balls, sf::Vector2i& mousePos, sf::RenderWindow& window) 
{
	if (spaceNow && !spacePressLastFrame)
	{
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
		balls.emplace_back((float)(rand() % 50 + 5), worldPos);
	}
	return spaceNow;
}


bool deleteBall(bool& dNow, bool& dPressedLastFrame, vector<Ball>& balls)
{
	if (dNow && !dPressedLastFrame && !balls.empty())
	{
		balls.pop_back();
	}
	return dNow;
}

// Toggle gravity force direction
bool toggleGravityDirection(bool& fNow, bool& fPressedLastFrame, bool& fallDownMode) 
{
	if (fNow && !fPressedLastFrame)
	{
		fallDownMode = !fallDownMode;
	}
	return fNow;
}

// Toggle collision type(currently 3 types)
bool toggleCollisionType(bool& cNow, bool cPressedLastFrame, size_t& currentMode, vector<std::function<void(vector<Ball>&, size_t, size_t)>>& collisionModes) 
{
	if (cNow && !cPressedLastFrame)
	{
		currentMode = (currentMode + 1) % collisionModes.size();
		std::cout << "Switched to collision mode: " << currentMode << std::endl;
	}
	return cNow;
}

// Pause ball simulation
bool pauseSimulation(bool& pNow, bool& pPressedLastFrame, bool& paused, sf::Clock& clock) 
{
	if (pNow && !pPressedLastFrame)
	{
		paused = !paused;
		clock.restart();
	}
	return pNow;
}

// Take 1 frame steps when simulation is paused
bool takeStep(bool& nNow, bool& stepRequested, bool& nPressedLastFrame, bool& paused) 
{
	if (nNow && !nPressedLastFrame)
	{
		if (paused) stepRequested = true;
	}
	return nNow;
}
