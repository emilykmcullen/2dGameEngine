#ifndef QUADTREE_H
#define QUADTREE_H
#include <SDL2/SDL.h>
#include<vector>
#include <list>
#include <memory>
using namespace std;

class Quadtree {
    private:
        int maxObjects = 10;
        int maxLevels = 5;

        int level; //current node level
        std::vector<SDL_Rect*> objects;
        SDL_Rect bounds;
        Quadtree* nodes[4];
        //using an array because I know how many elements I need
        //also remember vectors/arrays cannot hold references!
        void split();
        int getIndex(SDL_Rect* pRect);
    
    public:
        Quadtree(int level, SDL_Rect& bounds);
        void clear();
        void insert(SDL_Rect* pRect);
        std::vector<SDL_Rect*> retrieve;
        
        

        


};

#endif

