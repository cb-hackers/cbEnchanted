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
	STUB;
}

void MapInterface::commandSetTile(void) {
	STUB;
}

void MapInterface::functionLoadMap(void) {
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
	STUB;
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
	STUB;
}

void MapInterface::functionMapWidth(void) {
	int32_t width = tileMap->mapSizeX();
	cb->pushValue(width);
}

void MapInterface::functionMapHeight(void) {
	int32_t height = tileMap->mapSizeY();
	cb->pushValue(height);
}
