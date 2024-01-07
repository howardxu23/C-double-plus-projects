#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace std;

class Bullet
{
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float age;
    float  alpha;
    Bullet(sf::Vector2f position, sf::Vector2f velocity) : position(position), velocity(velocity), age(0.0f),alpha(1.0f) {}
};

class Bullets
{
    sf::Vector2f worldSize;
    vector<Bullet*> bullets;
    string textureAlias;
    float coolDownTimer = 0.0f;
    float coolDownTime = 0.2f;
public:
    Bullets(string textureAlias, sf::Vector2f worldSize) : textureAlias(textureAlias), worldSize(worldSize)
    {
    }
    void Add(sf::Vector2f pos, sf::Vector2f vel)
    {
        if (coolDownTimer > 0.0f) {
            return;
        }
        coolDownTimer = coolDownTime;
        Bullet* pBullet = new Bullet(pos, vel);
        bullets.push_back(pBullet);
    }

    void Update(float dt)
    {
        coolDownTimer -= dt;
        auto it = bullets.begin();
        while (it != bullets.end()) {
            Bullet* pBullet = *it;
            pBullet->age += dt;

            if (pBullet->age>1.0f) {
                pBullet->alpha -= 3.0f*dt;
            }
            pBullet->position += pBullet->velocity * dt;
            if (pBullet->position.x > worldSize.x) {
                pBullet->position.x -= worldSize.x;
            }
            if (pBullet->position.y > worldSize.y) {
                pBullet->position.y -= worldSize.y;
            }
            if (pBullet->position.x <0) {
                pBullet->position.x += worldSize.x;
            }
            if (pBullet->position.y <0) {
                pBullet->position.y += worldSize.y;
            }
            if (pBullet->alpha < 0.0f) {
                delete pBullet;
                it=bullets.erase(it);
            }
            else {
                it++;
            }
        }
    }
    int length() {
        return bullets.size();
    }
    Bullet* returnBullet(int i) {
        return bullets[i];
    }
    void Draw(sf::RenderWindow& window)
    {
        sf::Sprite sprite;
        TextureManager::Instance().SetSprite(textureAlias, sprite);

        sf::Vector2f texSize = sf::Vector2f(sprite.getTextureRect().width, sprite.getTextureRect().height);
        sprite.setOrigin(sf::Vector2f(texSize.x * 0.5f, texSize.y * 0.5f));

        auto it = bullets.begin();
        while (it != bullets.end()) {
            Bullet* pBullet = *it;
            sprite.setPosition(pBullet->position);
            float angle = (180.0f / 3.14159265358979323846) * atan2(pBullet->velocity.y, pBullet->velocity.x);
            sprite.setRotation(angle);
            sprite.setColor(sf::Color(255, 255, 255, 255 * pBullet->alpha));
            window.draw(sprite);
            it++;
        }
    }
};