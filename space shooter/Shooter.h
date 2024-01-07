#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
class Bullets;
class Ship;
class Rocks;

class Shooter
{
    // wrapper for the update and render methods for a simple shooter game
    sf::RenderWindow& window; // need a window reference for drawing things
    Bullets* bullets;
    Ship* ship;
    Rocks* rocks;
    sf::Font font;
    sf::Text text;
    int score=0;
public:
    Shooter(sf::RenderWindow& window);
    void Update(float dt);
    void Draw();
};