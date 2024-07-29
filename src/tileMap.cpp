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
    m_textures[TILE_LAMPBROKEN] = LoadTexture("assets/lampBroken.png");
    m_textures[TILE_WATER] = LoadTexture("assets/water.png");
    m_textures[TILE_MAGSLOT] = LoadTexture("assets/magSlot.png");
    m_textures[TILE_MAGSLOTACTIVE] = LoadTexture("assets/magSlotActive.png");
    m_textures[TILE_BLOCK] = LoadTexture("assets/block.png");
    m_textures[TILE_BLOCKB] = LoadTexture("assets/blockB.png");
    m_textures[TILE_BLOCKBL] = LoadTexture("assets/blockBL.png");
    m_textures[TILE_BLOCKBLH] = LoadTexture("assets/blockBLH.png");
    m_textures[TILE_BLOCKBP] = LoadTexture("assets/blockBP.png");
    m_textures[TILE_BLOCKBR] = LoadTexture("assets/blockBR.png");
    m_textures[TILE_BLOCKBRH] = LoadTexture("assets/blockBRH.png");
    m_textures[TILE_BLOCKF] = LoadTexture("assets/blockF.png");
    m_textures[TILE_BLOCKH] = LoadTexture("assets/blockH.png");
    m_textures[TILE_BLOCKL] = LoadTexture("assets/blockL.png");
    m_textures[TILE_BLOCKLP] = LoadTexture("assets/blockLP.png");
    m_textures[TILE_BLOCKR] = LoadTexture("assets/blockR.png");
    m_textures[TILE_BLOCKRP] = LoadTexture("assets/blockRP.png");
    m_textures[TILE_BLOCKT] = LoadTexture("assets/blockT.png");
    m_textures[TILE_BLOCKTL] = LoadTexture("assets/blockTL.png");
    m_textures[TILE_BLOCKTLH] = LoadTexture("assets/blockTLH.png");
    m_textures[TILE_BLOCKTP] = LoadTexture("assets/blockTP.png");
    m_textures[TILE_BLOCKTR] = LoadTexture("assets/blockTR.png");
    m_textures[TILE_BLOCKTRH] = LoadTexture("assets/blockTRH.png");
    m_textures[TILE_BLOCKV] = LoadTexture("assets/blockV.png");
    m_textures[TILE_ARROW] = LoadTexture("assets/arrow.png");
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
    if (isWithinBounds(posx, posy)){
        if(m_map[posx][posy] != nullptr) delete m_map[posx][posy];
        m_map[posx][posy] = p_tile;
    }
}

void TileMap::setLamp(Lamp* p_lamp){
    setTile(p_lamp);
    m_lamps.push_back(p_lamp);
}

void TileMap::setWater(Tile* p_tile) {
    Vector2 pos = p_tile->getPosition();
    int posx = pos.x, posy = pos.y;
    if (isWithinBounds(posx, posy)){
        if(m_map[posx][posy] != nullptr) delete m_map[posx][posy];
        m_map[posx][posy] = p_tile;
    }
    m_waterTiles.push_back(std::make_pair(posx, posy));
}

void TileMap::setMagCore(MagCore* p_magCore){
    m_magCores.push_back(p_magCore);
}

void TileMap::setMagSlot(Tile* p_magSlot, Lamp* p_lamp){
    setTile(p_magSlot);
    m_magSlots[p_magSlot] = p_lamp;
}

void TileMap::activateMagSlot(Tile* p_magSlot){
    Lamp* lamp = m_magSlots[p_magSlot];
    lamp->breakLamp();
    lamp->setType(TILE_LAMPBROKEN);
    p_magSlot->setType(TILE_MAGSLOTACTIVE);
}

void TileMap::draw() {
    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            Tile* t = m_map[x][y];
            if (t != nullptr && t->getType() != TILE_WATER)
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
    
    // Draw border
    addBlockRectangle(0, 0, kMapWidth, kMapHeight - 1, false);
    for(int i = 0; i < kMapWidth; i++)
        setTile(new Tile( {i*1.f,kMapHeight-1.f}, TILE_BLOCKF, true));
    
    if(mapNumber == 1){
        addExit(2);
        addBlockPlatform(27, 13, 3, false);
        addBlockPlatform(21, 11, 4, false);
        addBlockPlatform(14, 11, 4, false);
        addBlockPlatform(7, 11, 4, false);
        addBlockPlatform(2, 8, 1, false);
        addBlockPlatform(3, 10, 2, false);
        addBlockPlatform(7, 7, 4, false);
        addBlockPlatform(14, 5, 1, false);
        addBlockPlatform(18, 5, 1, false);
        addBlockPlatform(22, 5, 1, false);
        addBlockPlatform(26, 5, 3, false);
    }
    else if(mapNumber == 2){
        addExit(2);
        addBlockPlatform(27, 13, 3, false);
        addBlockPlatform(21, 11, 1, false);
        addBlockPlatform(15, 11, 4, false);
        addBlockPlatform(7, 12, 4, false);
        addBlockPlatform(7, 9, 2, false);
        addBlockPlatform(7, 7, 1, false);
        addBlockPlatform(8, 6, 3, false);
        addBlockPlatform(14, 6, 1, false);
        addBlockPlatform(18, 6, 3, false);
        addBlockPlatform(19, 3, 1, false);
        addBlockPlatform(24, 5, 3, false);
        setLamp(new Lamp({27.f, 14.f}, TILE_LAMP, true, 1));
        setLamp(new Lamp({8.f,10.f}, TILE_LAMP, true, 1));
        setLamp(new Lamp({19.f,4.f}, TILE_LAMP, true, 1));
    }
    else if(mapNumber == 3){
        addExit(2);
        addBlockPlatform(23, 13, 2, false);
        addBlockPlatform(15, 12, 3, false);
        addBlockPlatform(11, 11, 1, false);
        addBlockPlatform(5, 12, 3, false);
        addBlockPlatform(3, 9, 2, false);
        addBlockPlatform(6, 6, 2, false);
        addBlockPlatform(12, 6, 3, false);
        addBlockPlatform(13, 2, 1, false);
        addBlockPlatform(19, 6, 1, false);
        addBlockPlatform(26, 7, 3, false);
        addBlockPlatform(26, 4, 1, false);
        addBlockPlatform(27, 3, 1, false);
        addBlockPlatform(28, 4, 2, false);
        setLamp(new Lamp({6.f, 13.f}, TILE_LAMP, true, 2));
        setLamp(new Lamp({16.f,13.f}, TILE_LAMP, true, 2));
        setLamp(new Lamp({13.f,3.f}, TILE_LAMP, true, 2));
        setLamp(new Lamp({27.f,4.f}, TILE_LAMP, true, 2));
    }
    else if(mapNumber == 4){
        addExit(2);
        addBlockPlatform(3, 8, 3, false);
        addFillRectOnFloor(12, 7, 3, 10);
        addWaterPool(7, 16, 4, true);
        addWaterPool(17, 13, 4, false);
        addWaterPool(27, 9, 2, false);
    }
    else if(mapNumber == 5){
        addExit(2);
        addBlockPlatform(3, 13, 2, false);
        addBlockPlatform(7, 10, 2, false);
        addBlockPlatform(2, 8, 1, false);
        addBlockPlatform(6, 6, 1, false);
        addBlockPlatform(10, 2, 12, true);
        addBlockPlatform(12, 10, 1, false);
        addBlockPlatform(15, 10, 1, false);
        addBlockPlatform(17, 2, 12, true);
        addBlockPlatform(27, 14, 1, false);
        addBlockPlatform(29, 11, 1, false);
        addBlockPlatform(22, 10, 2, false);
        addBlockPlatform(19, 8, 1, false);
        addBlockPlatform(22, 6, 1, false);
        addBlockPlatform(24, 6, 6, false);
        addBlockPlatform(25, 1, 1, false);
        addBlockPlatform(28, 1, 1, false);
        Lamp* lamp1 = new Lamp({12.f,11.f}, TILE_LAMP, true, 4);
        setLamp(lamp1);
        setMagCore(new MagCore( {6.f*kTileSize, 14.f*kTileSize} ));
        setMagSlot(new Tile( {6.f, 3.f}, TILE_MAGSLOT, false ), lamp1);
        Lamp* lamp2 = new Lamp({28.f,2.f}, TILE_LAMP, true, 3);
        setLamp(lamp2);
        setMagCore(new MagCore( {20.f*kTileSize, 3.f*kTileSize} ));
        setMagSlot(new Tile( {29.f, 14.f}, TILE_MAGSLOT, false ), lamp2);
        setLamp(new Lamp({15.f,11.f}, TILE_LAMP, true, 4));
        setLamp(new Lamp({25.f,2.f}, TILE_LAMP, true, 3));
    }
    
    // Destroy lamp to use cesium on water
    // Destroy lamp to make two modules available (extra lamp to restart)
    // Tight spot between lamps
    
    ////////// old //////////
    else if(mapNumber == 6){
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
    else if(mapNumber == 7){
        addBlockPlatform(2, 2, 4, false);
        addBlockPlatform(2, 3, 2, true);
        addBlockRectangle(8, 2, 2, 2, true);
        addBlockRectangle(12, 2, 5, 5, true);
        addBlockRectangle(18, 2, 5, 5, false);
        addWaterPool(4, kMapHeight-6, 4, false);
        addWaterPool(16, kMapHeight-6, 1, false);
        addWaterPool(10, kMapHeight-2, 4, true);
        addExit(kMapHeight - 6);
    }
    else if(mapNumber == 8){
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
    else if(mapNumber == 9){
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
        Lamp* lamp1 = new Lamp({5.f,kMapHeight-5.f}, TILE_LAMP, true, 2);
        setLamp(lamp1);
        setMagCore(new MagCore( {5.f*kTileSize, (kMapHeight - 8.f)*kTileSize} ));
        setMagCore(new MagCore( {3.f*kTileSize, (kMapHeight - 8.f)*kTileSize} ));
        setMagSlot(new Tile( {26.f, kMapHeight-8.f}, TILE_MAGSLOT, false ), lamp1);
    }
    else if(mapNumber == 10){
        addBlockPlatform(2, 2, 4, false);
        addBlockPlatform(2, 3, 2, true);
        addBlockRectangle(8, 2, 2, 2, true);
        addBlockRectangle(12, 2, 5, 5, true);
        addBlockRectangle(18, 2, 5, 5, false);
        addWaterPool(4, kMapHeight-6, 4, false);
        addWaterPool(16, kMapHeight-6, 1, false);
        addWaterPool(10, kMapHeight-2, 4, true);
        addExit(kMapHeight - 6);
    }
}

void TileMap::addBlockPlatform(float p_posX, float p_posY, int p_length, bool p_vertical){
    if(p_length == 1){
        setTile(new Tile( {p_posX,p_posY}, TILE_BLOCK, true));
    }
    else{ 
        if(p_vertical){
            setTile(new Tile( {p_posX,p_posY}, TILE_BLOCKTP, true));
            setTile(new Tile( {p_posX,p_posY+p_length-1}, TILE_BLOCKBP, true));
            for(int i = 1; i < p_length - 1; i++)
                setTile(new Tile( {p_posX,p_posY+i}, TILE_BLOCKV, true));
        }
        else{
            setTile(new Tile( {p_posX,p_posY}, TILE_BLOCKLP, true));
            setTile(new Tile( {p_posX+p_length-1,p_posY}, TILE_BLOCKRP, true));
            for(int i = 1; i < p_length - 1; i++)
                setTile(new Tile( {p_posX+i,p_posY}, TILE_BLOCKH, true));
        }
    }
}
void TileMap::addBlockRectangle(float p_posX, float p_posY, int p_width, int p_height, bool p_fill){
    if(p_fill){
        setTile(new Tile( {p_posX,p_posY}, TILE_BLOCKTL, true));
        setTile(new Tile( {p_posX+p_width-1,p_posY}, TILE_BLOCKTR, true));
        setTile(new Tile( {p_posX,p_posY+p_height-1}, TILE_BLOCKBL, true));
        setTile(new Tile( {p_posX+p_width-1,p_posY+p_height-1}, TILE_BLOCKBR, true));
        for(int i = 1; i < p_width - 1; i++)
            setTile(new Tile( {p_posX+i,p_posY}, TILE_BLOCKT, true));
        for(int i = 1; i < p_width - 1; i++)
            setTile(new Tile( {p_posX+i,p_posY+p_height-1}, TILE_BLOCKB, true));
        for(int i = 1; i < p_height - 1; i++)
            setTile(new Tile( {p_posX,p_posY+i}, TILE_BLOCKL, true));
        for(int i = 1; i < p_height - 1; i++)
            setTile(new Tile( {p_posX+p_width-1,p_posY+i}, TILE_BLOCKR, true));
        for(int i = 1; i < p_width - 1; i++)
            for(int j = 1; j < p_height - 1; j++)
                setTile(new Tile( {p_posX+i,p_posY+j}, TILE_BLOCKF, true));
    }
    else{
        setTile(new Tile( {p_posX,p_posY}, TILE_BLOCKTLH, true));
        setTile(new Tile( {p_posX+p_width-1,p_posY}, TILE_BLOCKTRH, true));
        setTile(new Tile( {p_posX,p_posY+p_height-1}, TILE_BLOCKBLH, true));
        setTile(new Tile( {p_posX+p_width-1,p_posY+p_height-1}, TILE_BLOCKBRH, true));
        for(int i = 1; i < p_width - 1; i++){
            setTile(new Tile( {p_posX+i,p_posY}, TILE_BLOCKH, true));
            setTile(new Tile( {p_posX+i,p_posY+p_height-1}, TILE_BLOCKH, true));
        }
        for(int i = 1; i < p_height - 1; i++){
            setTile(new Tile( {p_posX,p_posY+i}, TILE_BLOCKV, true));
            setTile(new Tile( {p_posX+p_width-1,p_posY+i}, TILE_BLOCKV, true));
        }
    }
}
void TileMap::addWaterPool(float p_posX, float p_posY, int p_length, bool p_onFloor){
    for(int i = 0; i < p_length; i++)
        setWater(new Tile( {p_posX+i,p_posY}, TILE_WATER, false));
    if(!p_onFloor){
        setTile(new Tile( {p_posX-1,p_posY}, TILE_BLOCKTP, true));
        setTile(new Tile( {p_posX+p_length,p_posY}, TILE_BLOCKTP, true));
    }
    else{
        setTile(new Tile( {p_posX-1,p_posY}, TILE_BLOCKTRH, true));
        setTile(new Tile( {p_posX+p_length,p_posY}, TILE_BLOCKTLH, true));
    }
    setTile(new Tile( {p_posX-1,p_posY+1}, TILE_BLOCKBLH, true));
    setTile(new Tile( {p_posX+p_length,p_posY+1}, TILE_BLOCKBRH, true));
    for(int i = 0; i < p_length; i++)
        setTile(new Tile( {p_posX+i,p_posY+1}, TILE_BLOCKH, true));
}
void TileMap::addExit(int p_posY){
    setTile(new Tile( {kMapWidth-1.f,p_posY-1.f}, TILE_BLOCKBP, true));
    setTile(new Tile( {kMapWidth-1.f,p_posY+3.f}, TILE_BLOCKTP, true));
    for(int i = 0; i < 3; i++){
        delete m_map[kMapWidth - 1][p_posY + i];
        m_map[kMapWidth - 1][p_posY + i] = nullptr;
    }
    setTile(new Tile( {kMapWidth-1.f,p_posY+1.f}, TILE_ARROW, false));
}
void TileMap::addFillRectOnFloor(float p_posX, float p_posY, int p_width, int p_height){
    setTile(new Tile( {p_posX,p_posY}, TILE_BLOCKTLH, true));
    setTile(new Tile( {p_posX+p_width-1,p_posY}, TILE_BLOCKTRH, true));
    setTile(new Tile( {p_posX,p_posY+p_height-1}, TILE_BLOCKBRH, true));
    setTile(new Tile( {p_posX+p_width-1,p_posY+p_height-1}, TILE_BLOCKBLH, true));
    for(int i = 1; i < p_width - 1; i++)
        setTile(new Tile( {p_posX+i,p_posY}, TILE_BLOCKH, true));
    for(int i = 1; i < p_width - 1; i++)
        setTile(new Tile( {p_posX+i,p_posY+p_height-1}, TILE_BLOCKB, true));
    for(int i = 1; i < p_height - 1; i++)
        setTile(new Tile( {p_posX,p_posY+i}, TILE_BLOCKV, true));
    for(int i = 1; i < p_height - 1; i++)
        setTile(new Tile( {p_posX+p_width-1,p_posY+i}, TILE_BLOCKV, true));
    for(int i = 1; i < p_width - 1; i++){
        for(int j = 1; j < p_height - 1; j++)
            setTile(new Tile( {p_posX+i,p_posY+j}, TILE_BLOCKF, true));
        setTile(new Tile( {p_posX+i,p_posY+p_height-1}, TILE_BLOCKF, true));
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
    m_magSlots.clear();
    
    for(auto& mc : m_magCores)
        delete mc;
    m_magCores.clear();
}