#pragma once
#include "SDL.h"
#include <vector>
#include "Rock.h"
#include <unordered_map>

class RockFactory {

public:
    RockFactory();
    ~RockFactory();

    Rock* NewRock(RVector2D pos, RVector2D v, float m, std::string textureName);

    void AddTexture(std::string textureName, SDL_Texture* texture);

    SDL_Texture* GetTexture(std::string textureName);

private:
    std::unordered_map<std::string, SDL_Texture*> textures;

};
