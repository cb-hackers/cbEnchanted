#include "precomp.h"
#include "debug.h"
#include "mapinterface.h"
#include "cbobject.h"
#include "debug.h"
#include "cbenchanted.h"

MapInterface::MapInterface() {
	cb = static_cast<CBEnchanted*>(this);
	tileMap = 0;
}

MapInterface::~MapInterface() {
	if(tileMap!=NULL)
		delete tileMap;
}

void MapInterface::commandEditMap(void) {
	uint16_t tile = cb->popValue().toInt();
	uint32_t y = cb->popValue().toInt();
	uint32_t x = cb->popValue().toInt();
	uint8_t layer = cb->popValue().toInt();
	uint32_t map = cb->popValue().toInt();
	tileMap->edit(layer, x, y, tile);
}

void MapInterface::commandSetMap(void) {
	uint8_t over = cb->popValue().toInt();
	uint8_t back = cb->popValue().toInt();
	tileMap->setLayers(back, over);
}

void MapInterface::commandSetTile(void) {
	uint32_t slowness = cb->popValue().toInt();
	uint32_t lenght = cb->popValue().toInt();
	uint32_t tile = cb->popValue().toInt();
	tileMap->setTile(tile, lenght, slowness);
}

void MapInterface::functionLoadMap(void) {
	INFO("asd?");
	string tilesetpath = cb->popValue().toString();
	INFO(tilesetpath.c_str());
	string mappath = cb->popValue().toString();
	INFO(mappath.c_str());
	if (tileMap) delete tileMap;
	tileMap = new CBMap();
	if(tileMap->loadMap(mappath) == false){
		INFO("Cannot load map!");
	}else{
		INFO("Map loading success!");
	}
	tileMap->loadTileset(tilesetpath);
	int32_t id = cb->addMap(tileMap);
	cb->pushValue(id);
}

void MapInterface::functionMakeMap(void) {
	if (tileMap) delete tileMap;
	uint16_t tileH = cb->popValue().toInt();
	uint16_t tileW = cb->popValue().toInt();
	uint32_t mapH = cb->popValue().toInt();
	uint32_t mapW = cb->popValue().toInt();
	tileMap->create(mapW, mapH, tileW, tileH);
	int32_t id = cb->addMap(tileMap);
	cb->pushValue(id);
}

void MapInterface::functionGetMap(void) {
	int32_t posY = cb->popValue().toInt();
	int32_t posX = cb->popValue().toInt();
	uint8_t layer = cb->popValue().toInt();
	uint32_t mapX = (tileMap->getSizeX()/2+posX) / tileMap->getTileWidth();
	uint32_t mapY = (tileMap->getSizeX()/2+posX) / tileMap->getTileWidth();
	cb->pushValue(tileMap->getMap(layer, mapX, mapY));
}

void MapInterface::functionGetMap2(void) {
	int32_t posY = cb->popValue().toInt();
	int32_t posX = cb->popValue().toInt();
	uint8_t layer = cb->popValue().toInt();
	cb->pushValue(tileMap->getMap(layer, posX, posY));
}

void MapInterface::functionMapWidth(void) {
	int32_t width = tileMap->mapSizeX();
	cb->pushValue(width);
}

void MapInterface::functionMapHeight(void) {
	int32_t height = tileMap->mapSizeY();
	cb->pushValue(height);
}
