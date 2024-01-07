#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "TextureManager.h"
#include <iostream>
using namespace std;

class Rock
{
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float age;
    float alpha;
    Rock(sf::Vector2f position) : position(position), age(1.0f) ,alpha(1.0f) {
        velocity = sf::Vector2f(0, 0);
    }
};
class Rocks {
    sf::Vector2f worldSize;
    vector<Rock*> rocks;
    string textureAlias;
    int numrocks = 0;
    int MAXROCKS = 6;

public:
    Rocks(string textureAlias, sf::Vector2f worldSize) : textureAlias(textureAlias), worldSize(worldSize)
    {
        
    }
    void Add(sf::Vector2f pos)//adds rocks
    {
        Rock* pRock = new Rock(pos);
        rocks.push_back(pRock);
        
    }
    void Update(float dt) {
        if (numrocks < MAXROCKS) {
            //generate random coods for rock
            int xpos = rand() % (int)worldSize.x;
            int ypos = rand() % (int)worldSize.y;
            sf::Vector2f rockpos = sf::Vector2f(xpos, ypos);
            //adds rock
            Add(rockpos);
            numrocks++;
        }
        auto it = rocks.begin();
        while (it != rocks.end()) {
            Rock* pRock = *it;

            if (pRock->age <= 0.0f) {
                delete pRock;
                it = rocks.erase(it);
            }
            else {
                it++;
            }
        }
    }
    int length() {
        return rocks.size();
    }
    void rockDecrement() {
        numrocks--;
    }
    Rock* returnRock(int i) {
       return rocks[i];
    }
    void Draw(sf::RenderWindow& window)
    {
        sf::Sprite sprite;
        TextureManager::Instance().SetSprite(textureAlias, sprite);

        sf::Vector2f texSize = sf::Vector2f(sprite.getTextureRect().width, sprite.getTextureRect().height);
        sprite.setOrigin(sf::Vector2f(texSize.x * 0.5f, texSize.y * 0.5f));

        auto it = rocks.begin();
        while (it != rocks.end()) {
            Rock* pRock = *it;
            sprite.setPosition(pRock->position);
            float angle = (180.0f / 3.14159265358979323846) * atan2(pRock->velocity.y, pRock->velocity.x);
            sprite.setRotation(angle);
            sprite.setColor(sf::Color(255, 255, 255, 255 * pRock->alpha));
            window.draw(sprite);
            it++;
        }
    }
};