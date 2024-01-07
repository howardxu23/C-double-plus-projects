#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

using namespace std;

class SpriteTexture
{
public:
	string fileName;
	sf::IntRect rect;
};

class TextureManager
{
	// singleton which manages textures
	unordered_map< string, sf::Texture> textures;
	unordered_map< string, SpriteTexture> sprites;
	TextureManager(){} // private constructor
	void AddTextureFile(const string& fileName);
public:
	void AddTexture(const string& fileName, const sf::IntRect& rect, const string& alias);
	static TextureManager& Instance() 
	{
		static TextureManager instance; // static variable, only initialized once
		return instance; 
	}
	void SetSprite(const string& alias, sf::Sprite& spr)
	{
		// look for the alias
		if (sprites.find(alias) == sprites.end())
		{
			// can't find it, bail out
			return;
		}
		spr.setTexture(textures[sprites[alias].fileName]);
		spr.setTextureRect(sprites[alias].rect);
	}
};