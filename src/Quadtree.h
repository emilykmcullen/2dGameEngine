#ifndef QUADTREE_H
#define QUADTREE_H
#include <SDL2/SDL.h>
#include "Game.h"
#include<vector>
#include <list>
#include <memory>
#include "Entity.h"
using namespace std;

class Quadtree {
    private:
        int maxObjects = 10;
        int maxLevels = 5;

        int level; //current node level
        std::vector<Entity*> entities;
        SDL_Rect bounds;
        Quadtree* nodes[4];
        //using an array because I know how many elements I need
        //also remember vectors/arrays cannot hold references!
        void split();
        int getIndex(Entity* entity);
    
    public:
        Quadtree(int level, SDL_Rect bounds); //does bounds need to be a ref?
        void clearMe();
        void insert(Entity* entity, EntityManager& manager);
        std::vector<Entity*> retrieve(std::vector<Entity*> returnObjects, Entity* entity);
        
        

        


};

#endif

