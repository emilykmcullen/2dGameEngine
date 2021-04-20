#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../FontManager.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Game.h"


class TextLabelComponent: public Component {
    private:
        SDL_Rect position;
        std::string text;
        std::string fontFamily;
        SDL_Color color;
        SDL_Texture* texture;
    public:
        TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color& color) {
            this->position.x = x;
            this->position.y = y;
            this->text = text;
            this->fontFamily = fontFamily;
            this->color = color;
            SetLabelText(text, fontFamily);
        }

        void SetLabelText(std::string text, std::string fontFamily) {
            //this will receive the font family, get the font family from the asset manager and then pass the label
            //pretty much all text with SDL must be converted to a c-style string  
            SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->GetFont(fontFamily), text.c_str(), color);
            texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
            //deletes/frees the memory where the surface is (prevents memory leak)
            SDL_FreeSurface(surface);
            //retrieves the basic settings of a texture (texture, &format, &access, &width, &height)
            SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
        }

        void Render() override {
            FontManager::Draw(texture, position);
        }
};