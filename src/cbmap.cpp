
#include "cbmap.h"
#include "cbenchanted.h"
#include "precomp.h"
CBMap::CBMap():CBObject(){
	layerShowing[0] = 1;
	layerShowing[1] = 1;
}


CBMap::~CBMap(){
	for(int i = 0; i < 4; ++i){
		delete [] layer[i];
	}
	delete [] animLenght;
	delete [] animSlowness;
}

bool CBMap::create(uint32_t width, uint32_t height, uint16_t tileW, uint16_t tileH){
	mapWidth = width;
	mapHeight = height;
	tileWidth = tileW;
	tileHeight = tileH;
	for(int i = 0; i < 4; ++i){
		layer[i] = new int32_t[mapWidth*mapHeight];
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
		mapStream.read((char*)&checkNum[0], 1);
		mapStream.read((char*)&checkNum[1], 1);
		mapStream.read((char*)&checkNum[2], 1);
		mapStream.read((char*)&checkNum[3], 1);

		if(checkNum[0] != 23  &&
		   checkNum[1] != 179 &&
		   checkNum[2] != 243 &&
		   checkNum[3] != 56){
			FIXME("Incorrect magic numbers!")
		}


		mapStream.read((char*)&version, 4);
		if(!(version>=1.0 && version <= 2.0)){
			FIXME("Version isn't right!")
		}


		for(int i = 0; i < 512; ++i)
			mapStream.read((char*)&empty, 1);

		mapStream.read((char*)&maskR, 1);
		mapStream.read((char*)&maskG, 1);
		mapStream.read((char*)&maskB, 1);
		mapStream.read((char*)&empty, 1);

		for(int i = 0; i < 40; ++i)
				mapStream.read((char*)&empty, 1);
		for(int i = 0; i < 256; ++i)
				mapStream.read((char*)&empty, 1);


		mapStream.read((char*)&tilecount, 4);

		if(tilecount < 0)
				return false;

		animLenght = new int32_t [tilecount];
		animSlowness = new int32_t [tilecount];

		mapStream.read((char*)&tileWidth, 4);
		mapStream.read((char*)&tileHeight, 4);

		mapStream.read((char*)&mapWidth, 4);
		mapStream.read((char*)&mapHeight, 4);

		for(int i = 0; i < 4; ++i){
			layer[i] = new int32_t[mapWidth*mapHeight];
		}

		mapStream.read((char*)&checkNum[0], 1);
		mapStream.read((char*)&checkNum[1], 1);
		mapStream.read((char*)&checkNum[2], 1);
		mapStream.read((char*)&checkNum[3], 1);

		//Karttadata alkaa
		if(checkNum[0] != 254  &&
		   checkNum[1] != 45   &&
		   checkNum[2] != 12   &&
		   checkNum[3] != 166)
			FIXME("Incorrect magic nums!")

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

		if(checkNum[0] != 253  &&
		   checkNum[1] != 44   &&
		   checkNum[2] != 11   &&
		   checkNum[3] != 165)
				FIXME("Incorrect magic nums!");

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

		if(checkNum[0] != 251  &&
		   checkNum[1] != 42   &&
		   checkNum[2] != 9    &&
		   checkNum[3] != 163)
				FIXME("Incorrect magic nums!");

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

		if(checkNum[0] != 250  &&
		   checkNum[1] != 41   &&
		   checkNum[2] != 8    &&
		   checkNum[3] != 162)
				FIXME("Incorrect magic nums!");

		for(int32_t i = 1; i < tilecount; ++i){
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
	return load(path,sf::Color(maskR,maskG,maskB));
}

void CBMap::setLayers(uint8_t back, uint8_t over){
	layerShowing[0] = back;
	layerShowing[1] = over;
}

void CBMap::drawLayer(uint8_t level, RenderTarget &target){
	if(level > 2)
			return;
	if(layerShowing[level] < 1)
			return;

	float camX = CBEnchanted::instance()->getCameraX()-posX;
	float camY = CBEnchanted::instance()->getCameraY()-posY;


	int32_t piirto_x = camX-tileWidth+getSizeX()/2-target.width()/2;
	int32_t piirto_y = camY+getSizeY()/2-target.height()/2;
	int32_t tile_y = piirto_y / tileHeight;
	int32_t jarjestys_y=-(piirto_y % tileHeight);

	while(jarjestys_y < target.height()){
		tile_y %= getSizeY();
		if (tile_y >= mapHeight) break;
		int32_t tile_x = piirto_x / tileWidth+1;
		int32_t jarjestys_x =-(piirto_x % tileHeight);
		while(jarjestys_x < target.width()){
			int32_t getX = tile_x % getSizeX();
			if (getX >= mapWidth) break;
			int32_t tileNum = getMap(level, getX, tile_y);
			if(tileNum > 0){
				drawTile(target, tileNum, jarjestys_x, jarjestys_y);
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
	int32_t framesX = texture->GetWidth() / tileWidth;
	int32_t framesY = texture->GetHeight() / tileHeight;

	fX = (tile % framesX);
	fY = (tile / framesY);

	sprite.SetTextureRect(sf::IntRect(fX*tileWidth, fY*tileHeight, tileWidth, tileHeight));
	sprite.SetPosition(x, y);
	target.draw(sprite);

}

void CBMap::setTile(uint32_t tile, uint32_t lenght, uint32_t slowness){
	animLenght[tile] = lenght;
	animSlowness[tile] = slowness;
}

void CBMap::edit(uint8_t maplayer, int32_t MapX, int32_t MapY, int32_t tile){
	int32_t position = MapY * mapWidth + MapX;
	layer[maplayer][position] = tile;
}

int32_t CBMap::getMap(uint8_t maplayer, int32_t MapX, int32_t MapY){
	if(MapX < 0 || MapX >= mapWidth || MapY < 0 || MapY >= mapHeight)
		return 0;
	int32_t position = MapY * mapWidth + MapX;
	return layer[maplayer][position];
}
