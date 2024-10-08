#pragma once
#include "SDL.h"
#include <vector>
#include "Rock.h"
#include "RTexture.h"
#include <unordered_map>
#include <filesystem>

class RockFactory {
    std::filesystem::path resourcesDir;
    SDL_Renderer* renderer;
public:
    RockFactory(SDL_Renderer* renderer, std::filesystem::path resourcesDir);
    ~RockFactory();

    int RandomInt(int a, int b);

    Rock* NewRock();
    Rock* NewRock(RVector2D pos, RVector2D v, float m, std::string textureName);

    void AddTexture(std::string textureFileName);

    RTexture* GetTexture(std::string textureName);

private:
    std::unordered_map<std::string, RTexture*> textures;

};
