#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Shooter.h"
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    Shooter game(window);
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Update the window
        game.Update(1.0f / 60.0f);
        game.Draw();
        window.display();
    }
    return EXIT_SUCCESS;
}