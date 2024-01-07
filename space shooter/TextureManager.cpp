#include "TextureManager.h"


void TextureManager::AddTexture(const string& fileName, const sf::IntRect& rect, const string& alias)
{
	// make sure the texture is loaded
	AddTextureFile(fileName);
	sprites[alias] = SpriteTexture();
	sprites[alias].fileName = fileName;
	sprites[alias].rect = rect;

}

void TextureManager::AddTextureFile(const string& fileName)
{
	if (textures.find(fileName) == textures.end())
	{
		// we haven't seen this before, load it and store a reference in the map
		textures[fileName] = sf::Texture();
		textures[fileName].loadFromFile(fileName);
	}
}
