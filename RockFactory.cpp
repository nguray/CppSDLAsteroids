
#include "RockFactory.h"
#include <string>
#include <iostream>
#include <SDL_image.h>


RockFactory::RockFactory(SDL_Renderer* renderer, std::filesystem::path resourcesDir) : renderer(renderer), resourcesDir(resourcesDir)
{
}

RockFactory::~RockFactory()
{
    for (auto& [filename, texture] : textures) {
        delete texture;
    }
}

Rock* RockFactory::NewRock(RVector2D pos, RVector2D v, float m, std::string textureName)
{
    auto r = new Rock(pos, v, m);
    auto it = textures.find(textureName);
    if (it != textures.end()) {
        r->curTex = it->second;
    }
    else {
        r->curTex = NULL;
    }
    return r;
}

void RockFactory::AddTexture(std::string textureFileName)
{
    auto filePath = resourcesDir / std::filesystem::path(textureFileName);
    //std::cout << filePath.c_str() << std::endl;
    // Load image at specified path
    SDL_Surface* surface = IMG_Load(filePath.c_str());

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    std::pair<std::string, RTexture*> pair3 = std::make_pair(textureFileName, new RTexture(texture));
    textures.insert(pair3);

    SDL_FreeSurface(surface);


}

RTexture* RockFactory::GetTexture(std::string textureName)
{
    auto it = textures.find(textureName);
    if (it != textures.end()) {
        return it->second;
    }
    return NULL;
}