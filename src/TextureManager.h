#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "./Game.h"

class TextureManager {
    public:
        static SDL_Texture* LoadTexture(const char* fileName);
        static void Draw(SDL_Texture* texture, SDL_Rect source_Rectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip);
};

#endif