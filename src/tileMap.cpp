#include "tileMap.h"

#include <utility>

TileMap::TileMap() {
    // Init
    for (int y = 0; y < kMapHeight; y++) 
        for (int x = 0; x < kMapWidth; x++) 
            m_map[x][y] = nullptr;
    
    // Textures
    m_textures[TILE_BASE] = LoadTexture("assets/base.png");
    m_textures[TILE_TEST1] = LoadTexture("assets/test1.png");
    m_textures[TILE_TEST2] = LoadTexture("assets/test2.png");
    m_textures[TILE_TEST3] = LoadTexture("assets/test3.png");
    m_textures[TILE_LAMP] = LoadTexture("assets/lamp.png");
    m_textures[TILE_WATER] = LoadTexture("assets/water.png");
    m_textureMagCore = LoadTexture("assets/magCore.png");
}

TileMap::~TileMap() {
    clearMap();
    
    // Textures
    for(auto& t : m_textures)
        UnloadTexture(t.second);
    m_textures.clear();
    UnloadTexture(m_textureMagCore);
}

Tile* TileMap::getTile(int p_x, int p_y) {
    if (isWithinBounds(p_x, p_y))
        return m_map[p_x][p_y];
    return nullptr;
}
Tile* TileMap::getTileWorldPos(int p_x, int p_y) {
    int newX = p_x / kTileSize, newY = p_y / kTileSize;
    if (isWithinBounds(newX, newY))
        return m_map[newX][newY];
    return nullptr;
}

bool TileMap::isWithinBounds(int p_x, int p_y) const {
    return p_x >= 0 && p_x < kMapWidth && p_y >= 0 && p_y < kMapHeight;
}

int TileMap::getWidth() const {
    return kMapWidth;
}

int TileMap::getHeight() const {
    return kMapHeight;
}

int TileMap::getTileSize() const {
    return kTileSize;
}

const std::vector<Lamp*>& TileMap::getLamps() const{
    return m_lamps;
}

const std::vector<MagCore*>& TileMap::getMagCores() const{
    return m_magCores;
}

void TileMap::setTile(Tile* p_tile) {
    Vector2 pos = p_tile->getPosition();
    int posx = pos.x, posy = pos.y;
    if (isWithinBounds(posx, posy))
        m_map[posx][posy] = p_tile;
}

void TileMap::setLamp(Lamp* p_lamp){
    setTile(p_lamp);
    m_lamps.push_back(p_lamp);
}

void TileMap::setWater(Tile* p_tile) {
    Vector2 pos = p_tile->getPosition();
    int posx = pos.x, posy = pos.y;
    if (isWithinBounds(posx, posy))
        m_map[posx][posy] = p_tile;
    m_waterTiles.push_back(std::make_pair(posx, posy));
}

void TileMap::setMagCore(MagCore* p_magCore){
    m_magCores.push_back(p_magCore);
}

void TileMap::draw() {
    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            Tile* t = m_map[x][y];
            if (t != nullptr)
                t->draw(m_textures[t->getType()]);
        }
    }
    
    for(const auto& mc : m_magCores)
        mc->draw(m_textureMagCore);
}

void TileMap::drawWater(){
    for(const auto& pair : m_waterTiles){
        Tile* t = m_map[pair.first][pair.second];
        t->draw(m_textures[t->getType()]);
    }
}

void TileMap::loadMap(int mapNumber){
    if(mapNumber < 1 || mapNumber > 20) return;
    clearMap();
    if(mapNumber == 1){
        for(int i = 1; i < kMapWidth - 1; i++)
            setTile(new Tile({i*1.f,kMapHeight-2.f}, TILE_BASE, true));
        for(int i = 3; i < 7; i++)
            setTile(new Tile({i*1.f,kMapHeight-6.f}, TILE_BASE, true));
        for(int i = 7; i < 10; i++)
            setTile(new Tile({i*1.f,kMapHeight-5.f}, TILE_BASE, true));
        for(int i = 22; i < 29; i++)
            setTile(new Tile({i*1.f,kMapHeight-4.f}, TILE_BASE, true));
        for(int i = 22; i < 29; i++)
            setTile(new Tile({i*1.f,kMapHeight-5.f}, TILE_BASE, true));
        for(int i = 25; i < 28; i++)
            setTile(new Tile({i*1.f,kMapHeight-6.f}, TILE_BASE, true));
        setLamp(new Lamp({5.f,kMapHeight-5.f}, TILE_LAMP, true, 2));
    }
    else if(mapNumber == 2){
        for(int i = 1; i < kMapWidth - 1; i++)
            setTile(new Tile({i*1.f,kMapHeight-2.f}, TILE_BASE, true));
        for(int i = 22; i < 29; i++)
            setTile(new Tile({i*1.f,kMapHeight-4.f}, TILE_BASE, true));
        for(int i = 22; i < 29; i++)
            setTile(new Tile({i*1.f,kMapHeight-5.f}, TILE_BASE, false));
        for(int i = 25; i < 28; i++)
            setTile(new Tile({i*1.f,kMapHeight-6.f}, TILE_BASE, true));
        setMagCore(new MagCore( {4.f*kTileSize, (kMapHeight - 4.f)*kTileSize} ));
    }
    else if(mapNumber == 3){
        for(int i = 1; i < kMapWidth - 1; i++)
            setTile(new Tile({i*1.f,kMapHeight-2.f}, TILE_BASE, true));
        for(int i = 3; i < 10; i++)
            setTile(new Tile({i*1.f,kMapHeight-4.f}, TILE_BASE, true));
        for(int i = 4; i < 9; i++)
            setWater(new Tile({i*1.f,kMapHeight-5.f}, TILE_WATER, false));
        setTile(new Tile({3,kMapHeight-5.f}, TILE_BASE, true));
        setTile(new Tile({9,kMapHeight-5.f}, TILE_BASE, true));
        for(int i = 22; i < 29; i++)
            setTile(new Tile({i*1.f,kMapHeight-4.f}, TILE_BASE, true));
        for(int i = 22; i < 29; i++)
            setTile(new Tile({i*1.f,kMapHeight-5.f}, TILE_BASE, true));
        for(int i = 25; i < 28; i++)
            setTile(new Tile({i*1.f,kMapHeight-6.f}, TILE_BASE, true));
    }
}

void TileMap::clearMap(){
    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            Tile* t = m_map[x][y];
            if (t != nullptr){
                delete t;
                m_map[x][y] = nullptr;
            }
        }
    }
    
    m_lamps.clear();
    m_waterTiles.clear();
    
    for(auto& mc : m_magCores)
        delete mc;
    m_magCores.clear();
}