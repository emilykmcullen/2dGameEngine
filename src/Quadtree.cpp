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


//DETERMINE WHICH NODE WITHIN THE BOUNDS THE OBJECT BELONGS TO
//-1 means the object cannot completely fit within a child node and is part of the parent node
int Quadtree::getIndex(SDL_Rect& pRect){
    int index = -1;
    double verticalMidpoint = bounds.x + (bounds.w/2);
    double horizontalMidpoint = bounds.y + (bounds.h/2);

    //Object can fit completely within the top quadrants
    bool topQuadrant = (pRect.y < horizontalMidpoint && pRect.y + pRect.h < horizontalMidpoint);
    //Object can fit completely within the bottom quadrants
    bool bottomQuadrant = (pRect.y > horizontalMidpoint);

    //Object can fit within the left quadrants
    if (pRect.x < verticalMidpoint && pRect.x + pRect.w < verticalMidpoint){
        if (topQuadrant){
            index = 1; //top left quadrant
        }
        else if (bottomQuadrant){
            index = 2; //bottom left quadrant
        }
    }

    //Object can fit completely within the right quadrants
    if (pRect.x > verticalMidpoint){
        if (topQuadrant){
            index = 0; //top right quadrant
        }
        else if (bottomQuadrant) {
            index = 3; //bottom right quadrant
        }
    }

    return index;
    


}