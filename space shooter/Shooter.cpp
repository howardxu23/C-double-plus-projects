#include "Shooter.h"
#include "Bullets.h"
#include "Ship.h"
#include "Rocks.h"
using namespace std;

Shooter::Shooter(sf::RenderWindow& window) : window(window)
{
    TextureManager::Instance().AddTexture("assets/Asset-Sheet-With-Transparency.png", sf::IntRect(64, 388, 16, 7), "bullet");
    bullets = new Bullets("bullet", (sf::Vector2f)(window.getSize()));
    TextureManager::Instance().AddTexture("assets/Asset-Sheet-With-Transparency.png", sf::IntRect(201, 211, 30, 42), "ship");
    TextureManager::Instance().AddTexture("assets/Asset-Sheet-With-Transparency.png", sf::IntRect(143, 95, 16, 17), "thrust flame");


    ship = new Ship("ship","thrust flame", sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), (sf::Vector2f)(window.getSize()));

    TextureManager::Instance().AddTexture("assets/Asset-Sheet-With-Transparency.png", sf::IntRect(360, 6, 51, 45), "rocks");
    rocks = new Rocks("rocks",(sf::Vector2f)(window.getSize()));

    if (!font.loadFromFile("assets/Calibri.ttf")) {//loads font
        cout << "error loading font"<<endl;
    }
    //set text
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
}
void Shooter::Update(float dt)
{
    float angle = ship->getAngle();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        angle -= dt * 90.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        angle += dt * 90.0f;
    ship->setAngle(angle);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        float x = cos(angle * 3.14159265358979323846 / 180.0f);
        float y = sin(angle * 3.14159265358979323846 / 180.0f);
        sf::Vector2f velocity = sf::Vector2f(500.0 * x, 500.0 * y);
        bullets->Add(ship->getPosition(), velocity);
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        ship->Thrust();
    }
    
    for (int b = 0; b < bullets->length(); b++) {
        for (int r = 0; r < rocks->length(); r++) {
            //check each specifc bullet and rock distance
            //get bullet and rock
            auto pbullet = bullets->returnBullet(b);
            auto prock = rocks->returnRock(r);
            //get their positions
            sf::Vector2f bulletPos = pbullet->position;
            sf::Vector2f rockPos = prock->position;
            //get distance from each other
            sf::Vector2f diff = bulletPos - rockPos;
            float squaredDistance = diff.x * diff.x + diff.y * diff.y;
            //cout << squaredDistance<<endl;//DEBUG
            //remove rock and bullet when impact
            if (squaredDistance<1000.0f) {
                prock->age = 0;
                pbullet->alpha = 0;
                rocks->rockDecrement();
                score++;
                
            }
        }
    }
    //rocks->Add(sf::Vector2f(window.getSize().x/2, window.getSize().y / 2));//ADD ROCK test
    
    bullets->Update(dt);
    ship->Update(dt);
    rocks->Update(dt);

    //displays rocks destroyed
    ostringstream stream;
    stream << "rocks destroyed: " << score;
    string scoreText = stream.str();
    text.setString(scoreText);

}

void Shooter::Draw()
{
    bullets->Draw(window);
    ship->Draw(window);
    rocks->Draw(window);
    window.draw(text);
}