
#include "cbmap.h"
#include "cbenchanted.h"
#include "precomp.h"
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
	INFO("Load map called");

	fstream mapStream(file.c_str(), ios::binary | ios::in);
	unsigned char checkNum[4];
	float version;
	uint8_t empty;

	if(mapStream.is_open()){

		INFO("Map were opened for reading.");

		//Tarkistustavut...
		mapStream.read((char*)&checkNum[0], 1);
		mapStream.read((char*)&checkNum[1], 1);
		mapStream.read((char*)&checkNum[2], 1);
		mapStream.read((char*)&checkNum[3], 1);

		INFO("Magic number: %u, %u, %u, %u",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		if(checkNum[0] != 23  &&
		   checkNum[1] != 179 &&
		   checkNum[2] != 243 &&
		   checkNum[3] != 56){
			FIXME("Check nums aren't correct!")
		}


		mapStream.read((char*)&version, 4);
		INFO("Map version: %f", version);
		if(!(version>=1.0 && version <= 2.0)){
			FIXME("Version isn't right!")
		}


		for(int i = 0; i < 512; ++i)
			mapStream.read((char*)&empty, 1);

		mapStream.read((char*)&maskR, 1);
		mapStream.read((char*)&maskG, 1);
		mapStream.read((char*)&maskB, 1);
		mapStream.read((char*)&empty, 1);
		INFO("Mask Colors: %u, %u, %u: ", maskR, maskG, maskB);

		for(int i = 0; i < 40; ++i)
				mapStream.read((char*)&empty, 1);
		for(int i = 0; i < 256; ++i)
				mapStream.read((char*)&empty, 1);


		mapStream.read((char*)&tilecount, 4);
		INFO("Tile count: %u", tilecount);
		if(tilecount < 0)
				return false;

		animLenght = new int32_t [tilecount];
		animSlowness = new int32_t [tilecount];

		mapStream.read((char*)&tileWidth, 4);
		mapStream.read((char*)&tileHeight, 4);
		INFO("Tile width, height: %i, %i", tileWidth, tileHeight);

		mapStream.read((char*)&mapWidth, 4);
		mapStream.read((char*)&mapHeight, 4);
		INFO("Map width, height: %i, %i", mapWidth, mapHeight);

		for(int i = 0; i < 4; ++i){
			layer[i] = new int32_t[mapWidth*mapHeight];
		}

		mapStream.read((char*)&checkNum[0], 1);
		mapStream.read((char*)&checkNum[1], 1);
		mapStream.read((char*)&checkNum[2], 1);
		mapStream.read((char*)&checkNum[3], 1);
		INFO("Magic number: %u, %u, %u, %u",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		//Karttadata alkaa
		if(checkNum[0] != 254  &&
		   checkNum[1] != 45   &&
		   checkNum[2] != 12   &&
		   checkNum[3] != 166)
				FIXME("Check nums aren't correct!");

		for(int32_t y = 0; y < mapHeight; ++y){
			for(int32_t x = 0; x < mapWidth; ++x){
				int32_t position = y * mapWidth + x;
				mapStream.read((char*)&layer[0][position], 4);
			}
		}

		mapStream.read((char*)&checkNum[0], 1);
		mapStream.read((char*)&checkNum[1], 1);
		mapStream.read((char*)&checkNum[2], 1);
		mapStream.read((char*)&checkNum[3], 1);
		INFO("Magic number: %u, %u, %u, %u",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		if(checkNum[0] != 253  &&
		   checkNum[1] != 44   &&
		   checkNum[2] != 11   &&
		   checkNum[3] != 165)
				FIXME("Check nums aren't correct!");

		for(int32_t y = 0; y < mapHeight; ++y){
			for(int32_t x = 0; x < mapWidth; ++x){
				int32_t position = y * mapWidth + x;
				mapStream.read((char*)&layer[1][position], 4);
			}
		}

		mapStream.read((char*)&checkNum[0], 1);
		mapStream.read((char*)&checkNum[1], 1);
		mapStream.read((char*)&checkNum[2], 1);
		mapStream.read((char*)&checkNum[3], 1);
		INFO("Magic number: %u, %u, %u, %u",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		if(checkNum[0] != 252  &&
		   checkNum[1] != 43   &&
		   checkNum[2] != 10   &&
		   checkNum[3] != 164)
				FIXME("Check nums aren't correct!");

		for(int32_t y = 0; y < mapHeight; ++y){
			for(int32_t x = 0; x < mapWidth; ++x){
				int32_t position = y * mapWidth + x;
				mapStream.read((char*)&layer[2][position], 4);
			}
		}

		mapStream.read((char*)&checkNum[0], 1);
		mapStream.read((char*)&checkNum[1], 1);
		mapStream.read((char*)&checkNum[2], 1);
		mapStream.read((char*)&checkNum[3], 1);
		INFO("Magic number: %u, %u, %u, %u",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		if(checkNum[0] != 251  &&
		   checkNum[1] != 42   &&
		   checkNum[2] != 9    &&
		   checkNum[3] != 163)
				FIXME("Check nums aren't correct!");

		for(int32_t y = 0; y < mapHeight; ++y){
			for(int32_t x = 0; x < mapWidth; ++x){
				int32_t position = y * mapWidth + x;
				mapStream.read((char*)&layer[3][position], 4);
			}
		}

		mapStream.read((char*)&checkNum[0], 1);
		mapStream.read((char*)&checkNum[1], 1);
		mapStream.read((char*)&checkNum[2], 1);
		mapStream.read((char*)&checkNum[3], 1);
		INFO("Magic number: %u, %u, %u, %u",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		if(checkNum[0] != 250  &&
		   checkNum[1] != 41   &&
		   checkNum[2] != 8    &&
		   checkNum[3] != 162)
				FIXME("Check nums aren't correct!");

		for(int32_t i = 1; i < tilecount; ++i){
			mapStream.read((char*)&animLenght[i], 4);
			mapStream.read((char*)&animSlowness[i], 4);
		}

	}else{
		string err = "Can not open: "+file;
		INFO(err.c_str());
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
				drawTile(target,tile,posX+(x-mapWidth*0.5)*tileWidth,posY+(y+mapHeight*0.5)*tileHeight);
			}
		}
	}
}
void CBMap::drawOverLayer(RenderTarget &target){
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
	int32_t *tiles = layer[1];

	for (int32_t y = topTile; y <= bottomTile;++y) {
		for (int32_t x = leftTile;x <= rightTile;++x) {
			int32_t tile = *(tiles+y*mapWidth+x);
			if (tile > 0) {
				drawTile(target,tile,posX+(x-mapWidth*0.5)*tileWidth,posY+(y+mapHeight*0.5)*tileHeight);
			}
		}
	}
}

void CBMap::drawTile(RenderTarget &target, int32_t tile, float x, float y) {
        int32_t frameX = texture->GetWidth() / tileWidth;
        int32_t frameY = texture->GetHeight() / tileHeight;
        frameX = (frameX % tile) * frameWidth;
        frameY = (frameY / tile) * frameHeight;
        sprite.SetTextureRect(sf::Rect<int32_t>(frameX, frameY, tileWidth, tileHeight));
        target.draw(sprite);

}

void CBMap::edit(uint8_t maplayer, int32_t MapX, int32_t MapY, int32_t tile){
        int32_t position = MapY * mapWidth + MapX;
        layer[maplayer][position] = tile;
}
