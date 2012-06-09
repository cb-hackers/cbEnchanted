#include "precomp.h"
#include "debug.h"
#include "mapinterface.h"
#include "cbobject.h"
#include "debug.h"
#include "cbenchanted.h"
#include "errorsystem.h"

MapInterface::MapInterface() {
	cb = static_cast<CBEnchanted*>(this);
	tileMap = 0;
}

MapInterface::~MapInterface() {
}

void MapInterface::commandEditMap(void) {
	uint16_t tile = cb->popValue().toInt();
	uint32_t y = cb->popValue().toInt() - 1;
	uint32_t x = cb->popValue().toInt() - 1;
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
	ALLEGRO_PATH *tilesetpath = cb->popValue().getString().getPath();
	const char *ctilesetpath = al_path_cstr(tilesetpath, ALLEGRO_NATIVE_PATH_SEP);
	ALLEGRO_PATH *mappath = cb->popValue().getString().getPath();
	const char *cmappath = al_path_cstr(mappath, ALLEGRO_NATIVE_PATH_SEP);

	if (tileMap) {
		cb->removeFromDrawOrder(tileMap);
		delete tileMap;
	}
	tileMap = new CBMap();
	if(!tileMap->loadMap(cmappath)) {
		cb->errors->createError("LoadMap() failed!", "Failed to load map file \"" + string(cmappath) + "\"");
		cb->pushValue(0);
		al_destroy_path(mappath);
		return;
	}
	al_destroy_path(mappath);
	cb->addToDrawOrder(tileMap);
	if(!tileMap->loadTileset(ctilesetpath)) {
		cb->errors->createError("LoadMap() failed!", "Failed to load tileset \"" + string(ctilesetpath) + "\"");
		cb->pushValue(0);
		al_destroy_path(tilesetpath);
		return;
	}
	al_destroy_path(tilesetpath);

	int32_t id = cb->addMap(tileMap);
	cb->pushValue(id);
}

void MapInterface::functionMakeMap(void) {
	if (tileMap) {
		cb->removeFromDrawOrder(tileMap);
		delete tileMap;
	}
	tileMap = new CBMap();
	cb->addToDrawOrder(tileMap);
	uint16_t tileH = cb->popValue().toInt();
	uint16_t tileW = cb->popValue().toInt();
	uint32_t mapH = cb->popValue().toInt();
	uint32_t mapW = cb->popValue().toInt();
	tileMap->create(mapW, mapH, tileW, tileH);
	int32_t id = cb->addMap(tileMap);
	cb->pushValue(id);
}

void MapInterface::functionGetMap(void) {
	float posY = cb->popValue().toFloat();
	float posX = cb->popValue().toFloat();
	uint8_t layer = cb->popValue().toInt();
	cb->pushValue(tileMap->getMapWorldCoordinates(layer, posX, posY));
}

void MapInterface::functionGetMap2(void) {
	int32_t posY = cb->popValue().toInt() - 1;
	int32_t posX = cb->popValue().toInt() - 1;
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
