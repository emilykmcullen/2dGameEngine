#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"

Map::Map(std::string textureId, int scale, int tileSize){
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY){
    //TO DO READ THE MAP TILE DEFINITIONS FROM THE .map FILE
}

void Map::AddTile(int sourceX, int sourceY, int x, int y){
    //ADD NEW TILE ENTITY IN THE GAME SCENE
}