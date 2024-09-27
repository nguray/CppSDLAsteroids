
#include "RockFactory.h"
#include <string>
#include <iostream>
#include <SDL_image.h>


RockFactory::RockFactory()
{
}

RockFactory::~RockFactory()
{
    for (auto& [filename, texture] : textures) {
        SDL_DestroyTexture(texture);
    }
}

Rock* RockFactory::NewRock(RVector2D pos, RVector2D v, float m, std::string textureName)
{
    auto r = new Rock(pos, v, m);
    auto it = textures.find(textureName);
    if (it != textures.end()) {
        r->curTex = it->second;
    }
    return r;
}

void RockFactory::AddTexture(std::string textureName, SDL_Texture* texture)
{
    std::pair<std::string, SDL_Texture*> pair3 = std::make_pair(textureName, texture);
    textures.insert(pair3);

}

SDL_Texture* RockFactory::GetTexture(std::string textureName)
{
    auto it = textures.find(textureName);
    if (it != textures.end()) {
        return it->second;
    }
    return NULL;
}