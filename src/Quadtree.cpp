#include "Quadtree.h"
#include <SDL2/SDL.h>
#include<vector>
#include <list>
using namespace std;

Quadtree::Quadtree(int level, SDL_Rect& bounds) : level(level), bounds(bounds) {
    
}

void Quadtree::clear(){
    for (auto obj : objects){
        delete obj;
    }
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
int Quadtree::getIndex(SDL_Rect* theRect){
    int index = -1;
    double verticalMidpoint = bounds.x + (bounds.w/2);
    double horizontalMidpoint = bounds.y + (bounds.h/2);
    //dereference
    SDL_Rect &pRect = *theRect;

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

//INSERT THE OBJECT INTO THE QUADTREE
//If the node exceeds the capacity it will split and add all the objects to their corresponding nodes

void Quadtree::insert(SDL_Rect* pRect) {
    //do this current node have nodes? if yes:
    if (nodes[0] != nullptr) {
        int index = getIndex(pRect);
        //get appropriate index using the position of the rect
        if (index != -1) {
            nodes[index]->insert(pRect);
            //inserts the object in the node at this index, so it's now in the node tree in the correct node
            return;
        }
    }

    //if there are no child nodes or the object doesn't fit into a child node (goes over the bounds borders)
    //adds pRect into the objects list for the current (parent) node
    objects.emplace_back(pRect);

    //has the node overflown with objects?
    if (objects.size() > maxObjects && level < maxLevels) {
        if (nodes[0] == nullptr) {
            //does this node have subnodes? if not split it into subnodes
            split();
        }

        int i=0;
        while (i < objects.size()) {
            //loop through all the objects in the list and get the index of each
            
            int index = getIndex(objects.at(i));
            //if index is not -1, insert the object into the new node and remove form objects list in current node
            //so any objects that can fit completely into the child nodes are put there
            //and any that cant (they cross over the bounds borders) stay in the current (parent) node
            if (index != 1) {
                //to be checked
                SDL_Rect* copiedRect = objects.at(i);
                nodes[index]->insert(copiedRect);
                delete objects.at(i);
                objects.at(i) = nullptr;
                objects.erase(objects.begin() + i);
                
            }
            else {
                i++;
            }
        }
    }
}

//RETURN ALL OBJECTS IN ALL NODES THAT COULD COLLIDE WITH THE GIVEN OBJECT
std::vector<SDL_Rect*> Quadtree::retrieve(std::vector<SDL_Rect*> returnObjects, SDL_Rect* pRect){
    int index = getIndex(pRect);
    //if we have child nodes and the pRect can fit completely into one of them:
    if ( index != -1 && nodes[0] != nullptr) {
        //go to the node at that index and call the function again
        //this keeps going until there are no more nodes or the index is -1 (so it is overlapping the bounds)
        nodes[index]->retrieve(returnObjects, pRect);
    }

    //when index is -1 or there are no child nodes, all the objects at this node are added to returnObjects
    returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());

    return returnObjects;
}