#include "Quadtree.h"
#include <SDL2/SDL.h>
#include<vector>
#include <list>

Quadtree::Quadtree(int level, SDL_Rect& bounds) : level(level), bounds(bounds) {
    
}

void Quadtree::clear(){
    objects.clear();

    for (int i=0; i< sizeof(nodes); i++) {
        if (nodes[i] != nullptr) {
            nodes[i]->clear();
            nodes[i] = nullptr;
        }
    }
}