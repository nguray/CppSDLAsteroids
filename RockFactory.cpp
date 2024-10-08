/*---------------------------------------------------------------------------*\


\*---------------------------------------------------------------------------*/

#include "RockFactory.h"
#include <string>
#include <iostream>
#include <SDL_image.h>

extern void Path2Str(const std::filesystem::path& filePath, char* strFilePath, int strSize);

RockFactory::RockFactory(SDL_Renderer* renderer, std::filesystem::path resourcesDir) : renderer(renderer), resourcesDir(resourcesDir)
{
    AddTexture("rock00.png");
    AddTexture("rock10.png");

    AddTexture("rock20.png");
    AddTexture("rock21.png");
    AddTexture("rock22.png");
    AddTexture("rock23.png");

    AddTexture("rock30.png");
    AddTexture("rock31.png");
    AddTexture("rock32.png");
    AddTexture("rock33.png");

}

RockFactory::~RockFactory()
{
    for (auto& [filename, texture] : textures) {
        delete texture;
    }
}

int RockFactory::RandomInt(int a, int b)
{
    int diff = b - a + 1;
    int r = rand() % diff;
    return a + r;
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

Rock* RockFactory::NewRock()
{

    int winWidth, winHeight;
    SDL_Window* window = SDL_RenderGetWindow(renderer);
    SDL_GetWindowSizeInPixels(window, &winWidth, &winHeight);

    auto m = static_cast<float>(RandomInt(1, 2));
    auto ri = 10.0 * m;

    auto px = static_cast<float>(RandomInt(0, winWidth));
    auto py = static_cast<float>(RandomInt(0, winHeight));

    auto ra = static_cast<double>(RandomInt(0, 360) * M_PI / 180.0);

    std::string texName = (m == 2) ? "rock00.png" : "rock10.png";

    auto r = new Rock(
        RVector2D{ px,py },
        RVector2D{ static_cast<float>(1.35f * cos(ra)),static_cast<float>(1.35f * sin(ra)) },
        m);
    r->direction = static_cast<float>(RandomInt(0, 359));
    auto it = textures.find(texName);
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
    char myStr[128];
    // Load image at specified path
    auto filePath = resourcesDir / std::filesystem::path(textureFileName);
    Path2Str(filePath, myStr, sizeof(myStr));
    SDL_Surface* surface = IMG_Load(myStr);

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