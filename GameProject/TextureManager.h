#pragma once
#include "ResourceManager.h"
#include "Utilities.h"
class TextureManager :
    public ResourceManager<TextureManager, sf::Texture>
{
public:
    TextureManager() : ResourceManager("textures.cfg") {}

    sf::Texture* load(const std::string& l_path)
    {
        sf::Texture* texture = new sf::Texture();
        if (!texture->loadFromFile(Utils::GetWorkingDirectory() + l_path))
        {
            delete texture;
            texture = nullptr;
            std::cerr << "!Failed to load image from: "
                << l_path << std::endl;
        }
        return texture;
    }
};

