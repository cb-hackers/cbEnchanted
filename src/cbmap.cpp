
#include "cbmap.h"
#include "cbenchanted.h"

CBMap::CBMap(){
}


CBMap::~CBMap(){
	for(int i = 0; i < 4; ++i){
		delete [] layer;
	}
	delete [] animLenght;
	delete [] animSlowness;
}

bool CBMap::loadMap(string file){
	fstream mapStream(file.c_str(), ios::binary | ios::in);
	uint8_t checkNum[4];
	float version;
	uint8_t empty;
	if(mapStream.is_open()){
		//Tarkistustavut...
		mapStream >> checkNum[0];
		mapStream >> checkNum[1];
		mapStream >> checkNum[2];
		mapStream >> checkNum[3];

		if(checkNum[0] != 23  &&
		   checkNum[1] != 179 &&
		   checkNum[2] != 243 &&
		   checkNum[3] != 56)
				return false;

		mapStream >> version;
		if(version!=1.3)
			return false;


		for(int i = 0; i < 512; ++i)
			mapStream >> empty;

		mapStream >> maskR;
		mapStream >> maskG;
		mapStream >> maskB;

		for(int i = 0; i < 40; ++i)
				mapStream >> empty;

		for(int i = 0; i < 256; ++i)
				mapStream >> empty;


		mapStream >> tilecount;
		if(tilecount < 0)
				return false;

		animLenght = new int32_t [tilecount];
		animSlowness = new int32_t [tilecount];

		mapStream >> tileWidth;
		mapStream >> tileHeight;

		mapStream >> mapWidth;
		mapStream >> mapHeight;

		for(int i = 0; i < 4; ++i){
			layer[i] = new int32_t[mapWidth*mapHeight];
		}

		//Karttadata alkaa
		if(checkNum[0] != 254  &&
		   checkNum[1] != 45   &&
		   checkNum[2] != 12   &&
		   checkNum[3] != 166)
				return false;

		for(int32_t y = 1; y < mapHeight; ++y){
			for(int32_t x = 1; x < mapHeight; ++x){
				int32_t position = y * mapWidth + x;
				mapStream >> layer[0][position];
			}
		}

		if(checkNum[0] != 253  &&
		   checkNum[1] != 44   &&
		   checkNum[2] != 11   &&
		   checkNum[3] != 165)
				return false;

		for(int32_t y = 1; y < mapHeight; ++y){
			for(int32_t x = 1; x < mapHeight; ++x){
				int32_t position = y * mapWidth + x;
				mapStream >> layer[1][position];
			}
		}

		if(checkNum[0] != 252  &&
		   checkNum[1] != 43   &&
		   checkNum[2] != 10   &&
		   checkNum[3] != 164)
				return false;

		for(int32_t y = 1; y < mapHeight; ++y){
			for(int32_t x = 1; x < mapHeight; ++x){
				int32_t position = y * mapWidth + x;
				mapStream >> layer[2][position];
			}
		}

		if(checkNum[0] != 251  &&
		   checkNum[1] != 42   &&
		   checkNum[2] != 9    &&
		   checkNum[3] != 163)
				return false;

		for(int32_t y = 1; y < mapHeight; ++y){
			for(int32_t x = 1; x < mapHeight; ++x){
				int32_t position = y * mapWidth + x;
				mapStream >> layer[3][position];
			}
		}


		if(checkNum[0] != 250  &&
		   checkNum[1] != 41   &&
		   checkNum[2] != 8    &&
		   checkNum[3] != 162)
				return false;

		for(int32_t i = 1; i < tilecount; ++i){
			mapStream >> animLenght[i];
			mapStream >> animSlowness[i];
		}

	}
	mapStream.close();

	return true;
}

bool CBMap::loadTileset(string path){
	return load(path);
}

void CBMap::drawBackLayer(RenderTarget &target){
	float camX = CBEnchanted::instance()->getCameraX()-posX;
	float camY = CBEnchanted::instance()->getCameraY()-posY;


	//Calculate bounds
	int32_t leftTile = floorf(mapWidth*0.5+(camX-(target.width()*0.5))/tileWidth);
	if (leftTile >= mapWidth) return; //Out of map
	if (leftTile < 0) leftTile = 0;

	int32_t topTile = floorf(-mapHeight*0.5+(camY+(target.height()*0.5))/tileWidth);
	if (topTile >= mapHeight) return; //Out of map
	if (topTile < 0) leftTile = 0;

	int32_t rightTile = leftTile + ceilf(float(target.width())/tileWidth);
	if (rightTile < 0) return;
	if (rightTile >= mapWidth) rightTile = mapWidth-1;

	int32_t bottomTile = topTile + ceilf(float(target.height())/tileHeight);
	if (bottomTile < 0) return;
	if (bottomTile >= mapHeight) rightTile = mapHeight-1;
	int32_t *tiles = layer[0];

	for (int32_t y = topTile; y <= bottomTile;++y) {
		for (int32_t x = leftTile;x <= rightTile;++x) {
			int32_t tile = *(tiles+y*mapWidth+x);
			if (tile > 0) {
				drawTile(tile,posX+(x-mapWidth*0.5)*tileWidth,posY+(y+mapHeight*0.5)*tileHeight);
			}
		}
	}
}
void CBMap::drawOverLayer(RenderTarget &target){

}

void CBMap::drawTile(int32_t tile, float x, float y) {

}

void CBMap::edit(uint8_t maplayer, int32_t MapX, int32_t MapY, int32_t tile){
        int32_t position = MapY * mapWidth + MapX;
        layer[maplayer][position] = tile;
}
