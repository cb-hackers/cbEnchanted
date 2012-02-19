#include "precomp.h"
#include "debug.h"
#include "mapinterface.h"
#include "cbobject.h"
#include "debug.h"
#include "cbenchanted.h"

MapInterface::MapInterface() {
	cb = static_cast<CBEnchanted*>(this);
}

MapInterface::~MapInterface() {
	if(tileMap!=NULL)
		delete tileMap;
}

void MapInterface::commandEditMap(void) {
	STUB;
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
	STUB;
}

void MapInterface::functionGetMap2(void) {
	STUB;
}

void MapInterface::functionMapWidth(void) {
	STUB;
}

void MapInterface::functionMapHeight(void) {
	STUB;
}
