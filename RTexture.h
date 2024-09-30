#pragma once
#include <SDL.h>

class RTexture {
public:
    RTexture(SDL_Texture* texture);
    ~RTexture();
    int width;
    int height;
    SDL_Texture* texture;


};