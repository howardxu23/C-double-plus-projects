#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
using namespace std;

class Ship
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    float accleration = 10;
    sf::Vector2f worldSize;
    string textureAlias;
    string thrustFlame;
    float angle = 0.0f;
    bool isThrusting = false;
public:
    Ship(string textureAlias,string flametexture, sf::Vector2f position, sf::Vector2f worldSize) : position(position), velocity(sf::Vector2f(0.0f, 0.0f)), worldSize(worldSize), textureAlias(textureAlias), thrustFlame(flametexture) {}
    void Update(float dt)
    {
        position += velocity * dt;
        if (position.x > worldSize.x) {
            position.x -= worldSize.x;
        }
        if (position.y > worldSize.y) {
            position.y -= worldSize.y;
        }
        if (position.x < 0) {
            position.x += worldSize.x;
        }
        if (position.y < 0) {
            position.y += worldSize.y;
        }
        velocity *= 0.99f;
    }

    void Draw(sf::RenderWindow& window)
    {
        sf::Sprite Shipsprite;
        TextureManager::Instance().SetSprite(textureAlias, Shipsprite);//set ship sprite ship

        sf::Sprite flameSprite;
        TextureManager::Instance().SetSprite(thrustFlame, flameSprite);//set the flame sprite

        sf::Vector2f shiptexSize = sf::Vector2f(Shipsprite.getTextureRect().width, Shipsprite.getTextureRect().height);
        Shipsprite.setOrigin(sf::Vector2f(shiptexSize.x * 0.5f, shiptexSize.y * 0.5f));//set ship origin at center

        sf::Vector2f flametexSize = sf::Vector2f(flameSprite.getTextureRect().width, flameSprite.getTextureRect().height);
        flameSprite.setOrigin(sf::Vector2f(flametexSize.x * -1, flametexSize.y * 0.5f));//set flame origin at the left of the ship's sprite

        Shipsprite.setPosition(position);
        Shipsprite.setRotation(angle);

        flameSprite.setPosition(position);
        flameSprite.setRotation(angle+180);

        window.draw(Shipsprite);
        if (isThrusting == true) {//draw thrust flame if thrusting
            window.draw(flameSprite);
            isThrusting = false;
        }
    }
    void Thrust() {
        float x = cos(angle * 3.14159265358979323846 / 180.0f);
        float y = sin(angle * 3.14159265358979323846 / 180.0f);
        velocity += sf::Vector2f(accleration * x, accleration * y);
        isThrusting = true;
    }
    float getAngle() { return angle; }
    void setAngle(float newAngle) { angle = newAngle; }
    sf::Vector2f getPosition() { return position; }
};