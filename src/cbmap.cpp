#include "cbmap.h"
#include "cbenchanted.h"
#include "precomp.h"

CBMap::CBMap():CBObject(){
	layerShowing[0] = 1;
	layerShowing[1] = 1;
}


CBMap::~CBMap(){
	for(int i = 0; i < 4; ++i){
		delete [] layers[i];
	}
	delete [] animLenght;
	delete [] animSlowness;
	delete [] currentFrame;
}

bool CBMap::create(uint32_t width, uint32_t height, uint16_t tileW, uint16_t tileH){
	mapWidth = width;
	mapHeight = height;
	tileWidth = tileW;
	tileHeight = tileH;
	for(int i = 0; i < 4; ++i){
		layers[i] = new int32_t[mapWidth*mapHeight];
	}
	return true;
}

bool CBMap::loadMap(string file){


	fstream mapStream(file.c_str(), ios::binary | ios::in);
	unsigned char checkNum[4];
	float version;
	uint8_t empty;

	if(mapStream.is_open()){

		//Tarkistustavut...
		mapStream.read((char*)checkNum, 4);

		if(checkNum[0] != 40  || checkNum[1] != 192 || checkNum[2] != 13 || checkNum[3] != 139){
			FIXME("Map: Incorrect magic nums (%u, %u,%u, %u)!",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);
		}


		mapStream.read((char*)&version, 4);
		if(!(version>=1.0 && version <= 2.0)){
			FIXME("Version isn't right!")
		}


		mapStream.seekg(520);

		mapStream.read((char*)&maskR, 1);
		mapStream.read((char*)&maskG, 1);
		mapStream.read((char*)&maskB, 1);
		mapStream.read((char*)&empty, 1);

		mapStream.seekg(820);


		mapStream.read((char*)&tileCount, 4);

		if (tileCount < 0) {
			return false;
		}

		animLenght = new int32_t [tileCount];
		animSlowness = new int32_t [tileCount];
		currentFrame = new float [tileCount];
		memset(currentFrame,0,sizeof(float)*tileCount);

		mapStream.read((char*)&tileWidth, 4);
		mapStream.read((char*)&tileHeight, 4);

		mapStream.read((char*)&mapWidth, 4);
		mapStream.read((char*)&mapHeight, 4);

		for(int i = 0; i < 4; ++i){
			layers[i] = new int32_t[mapWidth*mapHeight];
		}

		mapStream.read((char*)checkNum, 4);

		//Karttadata alkaa
		if(checkNum[0] != 254  ||
		   checkNum[1] != 45   ||
		   checkNum[2] != 12   ||
		   checkNum[3] != 166)
			FIXME("Map layer0: Incorrect magic nums (%u, %u,%u, %u)!",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		mapStream.read((char*)layers[0], 4*mapHeight*mapWidth);

		mapStream.read((char*)checkNum, 4);

		if(checkNum[0] != 253  ||
		   checkNum[1] != 44   ||
		   checkNum[2] != 11   ||
		   checkNum[3] != 165)
			FIXME("Map layer1: Incorrect magic nums (%u, %u,%u, %u)!",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		//Collision layer
		mapStream.read((char*)layers[2], 4*mapHeight*mapWidth);

		mapStream.read((char*)checkNum, 4);

		if(checkNum[0] != 252  ||
		   checkNum[1] != 43   ||
		   checkNum[2] != 10   ||
		   checkNum[3] != 164)
			FIXME("Map layer2: Incorrect magic nums (%u, %u,%u, %u)!",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		mapStream.read((char*)layers[1], 4*mapHeight*mapWidth);

		mapStream.read((char*)checkNum, 4);

		if(checkNum[0] != 251  &&
		   checkNum[1] != 42   &&
		   checkNum[2] != 9    &&
		   checkNum[3] != 163)
			FIXME("Map layer3: Incorrect magic nums (%u, %u,%u, %u)!",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		mapStream.read((char*)layers[3], 4*mapHeight*mapWidth);

		mapStream.read((char*)checkNum, 4);

		if(checkNum[0] != 250  ||
		   checkNum[1] != 41   ||
		   checkNum[2] != 8    ||
		   checkNum[3] != 162)
			FIXME("Tiles: Incorrect magic nums (%u, %u,%u, %u)!",checkNum[0],checkNum[1],checkNum[2],checkNum[3]);

		for(int32_t i = 1; i < tileCount; ++i){
			mapStream.read((char*)&animLenght[i], 4);
			mapStream.read((char*)&animSlowness[i], 4);
		}

	}else{
		string err = "Can not open: "+file;
		INFO(err.c_str());
	}
	mapStream.close();
	sizeX = tileWidth*mapWidth;
	sizeY = tileHeight*mapHeight;
	return true;
}



bool CBMap::loadTileset(string path){
	bool success =  load(path,al_map_rgb(maskR,maskG,maskB));
	return success;
}

void CBMap::setLayers(uint8_t back, uint8_t over){
	layerShowing[0] = back;
	layerShowing[1] = over;
}

void CBMap::drawLayer(uint8_t level, RenderTarget &target){
	if(level > 1)
		return;
	if(layerShowing[level] < 1)
		return;

	float camX = CBEnchanted::instance()->getCameraX()-posX;
	float camY = CBEnchanted::instance()->getCameraY()-posY;


	int32_t piirto_x = camX+getSizeX()/2-target.width()/2;
	int32_t piirto_y = -camY+getSizeY()/2-target.height()/2;
	int32_t tile_y = piirto_y / tileHeight;
	int32_t jarjestys_y=-(piirto_y % tileHeight);

	while(jarjestys_y < target.height()){
		tile_y %= getSizeY();
		if (tile_y >= mapHeight) break;
		int32_t tile_x = piirto_x / tileWidth;
		int32_t jarjestys_x =-(piirto_x % tileHeight);
		while(jarjestys_x < target.width()){
			int32_t getX = tile_x % getSizeX();
			if (getX >= mapWidth) break;
			int32_t tileNum = getMap(level, getX, tile_y);
			if(tileNum > 0){
				drawTile(target, tileNum+(int)currentFrame[tileNum], jarjestys_x, jarjestys_y);
			}
			tile_x++;
			jarjestys_x+=tileWidth;
		}
		tile_y++;
		jarjestys_y+=tileHeight;
	}


}


void CBMap::drawTile(RenderTarget &target, int32_t tile, float x, float y) {
	if(tile == 0)
		return;
	tile--;

	int32_t fX = 0;
	int32_t fY = 0;
	int32_t framesX = renderTarget->width() / tileWidth;
	int32_t framesY = renderTarget->height() / tileHeight;
	fX = (tile % framesX);
	fY = (tile / framesY);

	target.drawBitmapRegion(texture,fX*tileWidth, fY*tileHeight, tileWidth, tileHeight,x,y);

}


bool CBMap::updateObject(float timestep) {
	if (playing) {
		for(uint16_t tiles = 0; tiles < tileCount; tiles++){
			if(animLenght[tiles]){
				currentFrame[tiles] = currentFrame[tiles] + timestep /((float)animSlowness[tiles]*animSpeed);
				if((int32_t)currentFrame[tiles] > animLenght[tiles]) {
					currentFrame[tiles] = 0;
				}
			}
		}
	}
	return false;
}


void CBMap::setTile(uint32_t tile, uint32_t lenght, uint32_t slowness){
	animLenght[tile] = lenght;
	animSlowness[tile] = slowness;
}

void CBMap::edit(uint8_t maplayer, int32_t MapX, int32_t MapY, int32_t tile){
	int32_t position = MapY * mapWidth + MapX;
	layers[maplayer][position] = tile;
}

int32_t CBMap::getHit(int32_t x, int32_t y){
	if(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
		return 0;
	int32_t position = y * mapWidth + x;
	return layers[2][position];
}

int32_t CBMap::getMap(uint8_t maplayer, int32_t MapX, int32_t MapY){
	if(MapX < 0 || MapX >= mapWidth || MapY < 0 || MapY >= mapHeight)
		return 0;
	int32_t position = MapY * mapWidth + MapX;
	return layers[maplayer][position];
}
