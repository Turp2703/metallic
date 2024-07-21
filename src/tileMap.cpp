#include "tileMap.h"

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
}

TileMap::~TileMap() {
    // Tiles
    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            Tile* t = m_map[x][y];
            if (t != nullptr)
                delete t;
        }
    }
    
    // Textures
    for(auto& t : m_textures)
        UnloadTexture(t.second);
    m_textures.clear();
}

Tile* TileMap::getTile(int p_x, int p_y) {
    if (isWithinBounds(p_x, p_y))
        return m_map[p_x][p_y];
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

void TileMap::setTile(Tile* p_tile) {
    Vector2 pos = p_tile->getPosition();
    int posx = pos.x, posy = pos.y;
    if (isWithinBounds(posx, posy))
        m_map[posx][posy] = p_tile;
}

void TileMap::draw() {
    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            Tile* t = m_map[x][y];
            if (t != nullptr)
                t->draw(m_textures[t->getType()]);
        }
    }
}