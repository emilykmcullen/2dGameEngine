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

//SPLIT THE NODE INTO 4 SUBNODES
//Divides the nodes into 4 equal parts and initializes the subnodes with the new bounds
void Quadtree::split(){
    int subWidth = static_cast<int>(bounds.w/2);
    int subHeight = static_cast<int>(bounds.h/2);
    int x = static_cast<int>(bounds.x);
    int y = static_cast<int>(bounds.y);

    SDL_Rect rectZero{x + subWidth, y, subWidth, subHeight};
    SDL_Rect rectOne{x + subWidth, y, subWidth, subHeight};
    SDL_Rect rectTwo{x + subWidth, y, subWidth, subHeight};
    SDL_Rect rectThree{x + subWidth, y, subWidth, subHeight};
    
    nodes[0] = new Quadtree(level + 1,  rectZero);
    nodes[1] = new Quadtree(level + 1,  rectOne);
    nodes[2] = new Quadtree(level + 1,  rectTwo);
    nodes[3] = new Quadtree(level + 1,  rectThree);
}