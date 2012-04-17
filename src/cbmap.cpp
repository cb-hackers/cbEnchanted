#include "cbmap.h"
#include "cbenchanted.h"
#include "precomp.h"

/** Constructs a new CBMap and calls the constructor for CBObject */
CBMap::CBMap() : CBObject() {
	layerShowing[0] = 1;
	layerShowing[1] = 1;
}

/** Destroys all layers and frees resources */
CBMap::~CBMap() {
	for (int i = 0; i < 4; ++i) {
		delete [] layers[i];
	}
	delete [] animLength;
	delete [] animSlowness;
	delete [] currentFrame;
}

/** Creates a new map
 * @param width The width of the map, in tiles
 * @param height The height of the map, in tiles
 * @param tileW Width of a single tile, in pixels
 * @param tileH Height of a single tile, in pixels
 */
bool CBMap::create(uint32_t width, uint32_t height, uint16_t tileW, uint16_t tileH) {
	mapWidth = width;
	mapHeight = height;
	tileWidth = tileW;
	tileHeight = tileH;
	for (int i = 0; i < 4; ++i) {
		layers[i] = new int32_t[mapWidth * mapHeight];
	}
	return true;
}

/** Loads a map from file.
 * @param file Path to mapfile
 */
bool CBMap::loadMap(string file) {
	fstream mapStream(file.c_str(), ios::binary | ios::in);
	unsigned char checkNum[4];
	float version;
	uint8_t empty;

	if (mapStream.is_open()) {
		//Tarkistustavut...
		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 40 || checkNum[1] != 192 || checkNum[2] != 13 || checkNum[3] != 139) {
			FIXME("Map: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1] ,checkNum[2], checkNum[3]);
		}


		mapStream.read((char*)&version, 4);
		if (!(version>=1.0 && version <= 2.0)) {
			FIXME("Version isn't right!");
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

		animLength = new int32_t [tileCount];
		animSlowness = new int32_t [tileCount];
		currentFrame = new float [tileCount];
		memset(currentFrame,0,sizeof(float)*tileCount);

		mapStream.read((char*)&tileWidth, 4);
		mapStream.read((char*)&tileHeight, 4);

		mapStream.read((char*)&mapWidth, 4);
		mapStream.read((char*)&mapHeight, 4);

		for (int i = 0; i < 4; ++i) {
			layers[i] = new int32_t [mapWidth * mapHeight];
		}

		mapStream.read((char*)checkNum, 4);

		//Karttadata alkaa
		if (checkNum[0] != 254 ||
			checkNum[1] != 45 ||
			checkNum[2] != 12 ||
			checkNum[3] != 166
		) {
			FIXME("Map layer0: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
		}

		mapStream.read((char*)layers[0], 4 * mapHeight * mapWidth);

		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 253 ||
			checkNum[1] != 44 ||
			checkNum[2] != 11 ||
			checkNum[3] != 165
		) {
			FIXME("Map layer1: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
		}

		//Collision layer
		mapStream.read((char*)layers[2], 4 * mapHeight * mapWidth);

		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 252 ||
			checkNum[1] != 43 ||
			checkNum[2] != 10 ||
			checkNum[3] != 164
		) {
			FIXME("Map layer2: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
		}

		mapStream.read((char*)layers[1], 4*mapHeight*mapWidth);

		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 251 ||
			checkNum[1] != 42 ||
			checkNum[2] != 9 ||
			checkNum[3] != 163
		) {
			FIXME("Map layer3: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
		}

		mapStream.read((char*)layers[3], 4 * mapHeight * mapWidth);

		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 250 ||
			checkNum[1] != 41 ||
			checkNum[2] != 8 ||
			checkNum[3] != 162
		) {
			FIXME("Tiles: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
		}

		for (int32_t i = 1; i < tileCount; ++i) {
			mapStream.read((char*)&animLength[i], 4);
			mapStream.read((char*)&animSlowness[i], 4);
		}

	}
	else { // mapstream.is_open() returned false
		string err = "Can not open: " + file;
		INFO(err.c_str());
	}
	mapStream.close();
	sizeX = tileWidth * mapWidth;
	sizeY = tileHeight * mapHeight;
	return true;
}

/** Loads a tileset from an image file.
 * @param path Path to an image file containing the tilemap
 */
bool CBMap::loadTileset(string path) {
	bool success = load(path, al_map_rgb(maskR, maskG, maskB));
	return success;
}

/** Sets layer visibility.
 * @param back Is the back layer visible or not
 * @param over Is the over layer visible or not
 */
void CBMap::setLayers(uint8_t back, uint8_t over) {
	layerShowing[0] = back;
	layerShowing[1] = over;
}

/** Draws the given layer to the given RenderTarget.
 * @param level Which layer should we draw.
 * @param target To where should we draw, @see RenderTarget
 */
void CBMap::drawLayer(uint8_t level, RenderTarget &target) {
	if (level > 1) {
		return;
	}
	if (layerShowing[level] < 1) {
		return;
	}

	float camX = CBEnchanted::instance()->getCameraX() - posX;
	float camY = CBEnchanted::instance()->getCameraY() - posY;


	int32_t piirto_x = camX + getSizeX() / 2 - target.width() / 2;
	int32_t piirto_y = -camY + getSizeY() / 2 - target.height() / 2;
	int32_t tile_y = piirto_y / tileHeight;
	int32_t jarjestys_y = -(piirto_y % tileHeight);

	while (jarjestys_y < target.height()) {
		tile_y %= getSizeY();
		if (tile_y >= mapHeight) {
			break;
		}

		int32_t tile_x = piirto_x / tileWidth;
		int32_t jarjestys_x = -(piirto_x % tileHeight);

		while (jarjestys_x < target.width()) {
			int32_t getX = tile_x % getSizeX();
			if (getX >= mapWidth) {
				break;
			}

			int32_t tileNum = getMap(level, getX, tile_y);
			if (tileNum > 0) {
				drawTile(target, tileNum+(int)currentFrame[tileNum], jarjestys_x, jarjestys_y);
			}

			tile_x++;
			jarjestys_x += tileWidth;
		}
		tile_y++;
		jarjestys_y += tileHeight;
	}
}

/** Draws a single tile.
 * @param target To where should we draw to, @see RenderTarget
 * @param tile Which tile should we draw from the tileset, a number from 1 to tile count
 * @param x,y Drawing coordinates
 */
void CBMap::drawTile(RenderTarget &target, int32_t tile, float x, float y) {
	if (tile == 0) {
		return;
	}
	tile--;

	int32_t fX = 0;
	int32_t fY = 0;
	int32_t framesX = renderTarget->width() / tileWidth;
	int32_t framesY = renderTarget->height() / tileHeight;
	fX = tile % framesX;
	fY = tile / framesY;

    target.drawBitmapRegion(texture, fX * tileWidth, fY * tileHeight, tileWidth, tileHeight, x, y);
}

/** Updates the map animation.
 * @param timestep Does something related to time and step
 * @returns Always false
 */
bool CBMap::updateObject(float timestep) {
	if (!playing) {
		return false;
	}
	// Loop through every tile and update the currentFrame array if the tile has animation
	for (uint16_t tiles = 0; tiles < tileCount; tiles++) {
		if (animLength[tiles]) {
			currentFrame[tiles] = currentFrame[tiles] + timestep / ((float)animSlowness[tiles] * animSpeed);
			if ((int32_t)currentFrame[tiles] > animLength[tiles]) {
				currentFrame[tiles] = 0;
			}
		}
	}
	return false;
}

/** Sets tile animation properties.
 * @param tile Which tile are we modifying
 * @param length How many of the following tiles are attached to the animation
 * @param slowness How slowly will the tile animate
 */
void CBMap::setTile(uint32_t tile, uint32_t length, uint32_t slowness) {
	animLength[tile] = length;
	animSlowness[tile] = slowness;
}

/** Manipulates the loaded map realtime.
 * @param maplayer Which layer of the map are we modifying
 * @param tileX,tileY Tile coordinates
 * @param tileIndex The index of the tile to be drawn
 */
void CBMap::edit(uint8_t maplayer, int32_t tileX, int32_t tileY, int32_t tileIndex) {
	int32_t position = tileY * mapWidth + tileX;
	layers[maplayer][position] = tileIndex;
}

/** Returns the hit data in the given tile coordinates.
 * @param x,y Tile coordinates
 * @returns What data is there in the second layer of the map
 */
int32_t CBMap::getHit(int32_t x, int32_t y) {
	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
		// No hit outside the map
		return 0;
	}
	// Calculate index position and return the data in hit layer (layer2)
	int32_t position = y * mapWidth + x;
	return layers[2][position];
}

/** Returns data from the given map layer.
 * @param maplayer Which layer are we reading from
 * @param tileX,tileY Tile coordinates
 * @returns What data is there in the given layer
 */
int32_t CBMap::getMap(uint8_t maplayer, int32_t tileX, int32_t tileY) {
	if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight) {
		// There's no data outside the map region
		return 0;
	}
	// Calculate index position and return the data in the given layer
	int32_t position = tileY * mapWidth + tileX;
	return layers[maplayer][position];
}

/** Does a raycast from given object to this map.
 *
 * @param fromObject From which object will the raycast start
 * @param returnX This variable will be set to the x-coordinate of raycast collision point.
 * @param returnY This variable will be set to the y-coordinate of raycast collision point.
 *
 * @returns Whether the ray hit the object or not.
 */
bool CBMap::rayCast(CBObject *obj, float &returnX, float &returnY) {
	// Convert from world coordinates to tilemap based coordinates
	float startX = obj->getX() + this->mapWidth * this->tileWidth / 2;
	float startY = -obj->getY() + this->mapHeight * this->tileHeight / 2;
	float endX = startX + cos((obj->getAngle() / 180.0) * M_PI) * (this->sizeX * this->sizeY);
	float endY = startY + sin((obj->getAngle() / 180.0) * M_PI) * (this->sizeX * this->sizeY);

	// Do the raycast
	bool didRayHit = this->mapRayCast(startX, startY, endX, endY, returnX, returnY);

	// Convert returned values from tilemap based coordinates back to world coordinates
	returnX = returnX - this->mapWidth * this->tileWidth / 2;
	returnY = this->mapHeight * this->tileHeight / 2 - returnY;

	return didRayHit;
}

/** Does a raycast between given coordinates (relative to tilemap) and sets the raycast end
 * point to the referenced variables. */
bool CBMap::mapRayCast(float startX, float startY, float endX, float endY, float &returnX, float &returnY) {
	DEBUG("Raycasting from (%f, %f) to (%f, %f)", startX, startY, endX, endY);

	returnX = endX;
	returnY = endY;

	return false;
}
