#include "cbmap.h"
#include "cbenchanted.h"


/** Constructs a new CBMap and calls the constructor for CBObject */
CBMap::CBMap() :
	CBObject(),
	animLength(0),
	animSlowness(0)
{
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
	sizeX = width * tileW;
	sizeY = height * tileH;
	for (int i = 0; i < 4; ++i) {
		layers[i] = new int32_t[width * height];
		memset(layers[i], 0, width * height * sizeof(int32_t));
	}
	tileCount = 64; //Default size. Arrays will be resized if more tiles are used.
	currentFrame = new float [tileCount];
	animSlowness = new int32_t [tileCount];
	animLength = new int32_t [tileCount];
	memset(currentFrame, 0, sizeof(float) * tileCount);
	memset(animLength, 0, tileCount * sizeof(int32_t));
	for (uint32_t i = 0; i < tileCount; i++) {
		animSlowness[i] = 1;
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
			//FIXME("Map: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1] ,checkNum[2], checkNum[3]);
			mapStream.close();
			return false;
		}


		mapStream.read((char*)&version, 4);
		if (!(version>=1.0 && version <= 2.0)) {
			//FIXME("Version isn't right!");
			mapStream.close();
			return false;
		}


		mapStream.seekg(520);

		mapStream.read((char*)&maskR, 1);
		mapStream.read((char*)&maskG, 1);
		mapStream.read((char*)&maskB, 1);
		mapStream.read((char*)&empty, 1);

		mapStream.seekg(820);


		mapStream.read((char*)&tileCount, 4);

		if (tileCount < 0) {
			mapStream.close();
			return false;
		}

		animLength = new int32_t [tileCount];
		animSlowness = new int32_t [tileCount];
		currentFrame = new float [tileCount];
		memset(currentFrame,0,sizeof(float) * tileCount);

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
			//FIXME("Map layer0: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
			mapStream.close();
			return false;
		}

		mapStream.read((char*)layers[0], mapWidth * mapHeight * sizeof(int32_t));

		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 253 ||
			checkNum[1] != 44 ||
			checkNum[2] != 11 ||
			checkNum[3] != 165
		) {
			//FIXME("Map layer1: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
			mapStream.close();
			return false;
		}

		//Collision layer
		mapStream.read((char*)layers[2], mapWidth * mapHeight * sizeof(int32_t));
		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 252 ||
			checkNum[1] != 43 ||
			checkNum[2] != 10 ||
			checkNum[3] != 164
		) {
			//FIXME("Map layer2: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
			mapStream.close();
			return false;
		}


		mapStream.read((char*)layers[1], mapWidth * mapHeight * sizeof(int32_t));

		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 251 ||
			checkNum[1] != 42 ||
			checkNum[2] != 9 ||
			checkNum[3] != 163
		) {
			//FIXME("Map layer3: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
			mapStream.close();
			return false;
		}

		mapStream.read((char*)layers[3], mapWidth * mapHeight * sizeof(int32_t));

		mapStream.read((char*)checkNum, 4);

		if (checkNum[0] != 250 ||
			checkNum[1] != 41 ||
			checkNum[2] != 8 ||
			checkNum[3] != 162
		) {
			//FIXME("Tiles: Incorrect magic nums (%u, %u,%u, %u)!", checkNum[0], checkNum[1], checkNum[2], checkNum[3]);
			mapStream.close();
			return false;
		}

		for (uint32_t i = 1; i < tileCount; ++i) {
			mapStream.read((char*)&animLength[i], 4);
			mapStream.read((char*)&animSlowness[i], 4);
		}

	}
	else { // mapstream.is_open() returned false
		mapStream.close();
		return false;
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


/** Changes the map tileset */
void CBMap::paintObject(const RenderTarget &tex) {
	if (!renderTarget) {
		renderTarget = new RenderTarget;
	}
	if (texture) {
		al_destroy_bitmap(texture);
	}
	renderTarget->copy(&tex);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture, maskColor);
	painted = true;
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
	if (visible && painted) {
		if (level > 1) {
			return;
		}
		if (layerShowing[level] < 1) {
			return;
		}

		float camX = CBEnchanted::instance()->getCameraX() - posX;
		float camY = CBEnchanted::instance()->getCameraY() - posY;


		int32_t draw_x = camX + getSizeX() / 2 - target.width() / 2;
		int32_t draw_y = -camY + getSizeY() / 2 - target.height() / 2;
		int32_t tile_y = draw_y / tileHeight;
		int32_t order_y = -(draw_y % tileHeight);

		while (order_y < target.height()) {
			tile_y %= getSizeY();
			if (tile_y >= mapHeight) {
				break;
			}

			int32_t tile_x = draw_x / tileWidth;
			int32_t order_x = -(draw_x % tileHeight);

			while (order_x < target.width()) {
				int32_t getX = tile_x % getSizeX();
				if (getX >= mapWidth) {
					break;
				}

				int32_t tileNum = getMap(level, getX, tile_y);
				if (tileNum > 0) {
					drawTile(target, tileNum+(int)currentFrame[tileNum], order_x, order_y);
				}

				tile_x++;
				order_x += tileWidth;
			}
			tile_y++;
			order_y += tileHeight;
		}
	}
}

void CBMap::render(RenderTarget &target) {
	target.useWorldCoords(false);
	drawLayer(0, target);
	target.useWorldCoords(true);
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
	if (tile >= tileCount) {//Arrays are too small
		uint32_t newTileCount = tile + 1;
		//New arrays
		float *newCurrentFrame = new float [newTileCount];
		int32_t *newSlowness = new int32_t[newTileCount];
		int32_t *newLength = new int32_t [newTileCount];
		//Copy old data
		memcpy(newCurrentFrame, currentFrame, sizeof(float) * tileCount);
		memcpy(newSlowness, animSlowness, sizeof(int32_t) * tileCount);
		memcpy(newLength, animLength, sizeof(int32_t) * tileCount);
		//Initialize new part
		memset(newCurrentFrame + tileCount, 0, (newTileCount - tileCount) * sizeof(float));
		memset(newLength + tileCount, 0, (newTileCount - tileCount) * sizeof(int32_t));
		for (uint32_t i = tileCount; i < newTileCount; i++) {
			animSlowness[i] = 1;
		}
		//Delete old arrays
		delete[] currentFrame;
		delete[] animLength;
		delete[] animSlowness;

		tileCount = newTileCount;
		currentFrame = newCurrentFrame;
		animLength = newLength;
		animSlowness = newSlowness;
	}
	animLength[tile] = length;
	animSlowness[tile] = slowness;
}

/** Manipulates the loaded map realtime.
 * @param maplayer Which layer of the map are we modifying
 * @param tileX,tileY Tile coordinates
 * @param tileIndex The index of the tile to be drawn
 */
void CBMap::edit(uint8_t maplayer, int32_t tileX, int32_t tileY, int32_t tileIndex) {
	if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight) {
		return;
	}
	layers[maplayer][tileY * mapWidth + tileX] = tileIndex;
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
	return layers[2][y * mapWidth + x];
}

/** Returns data from the given map layer.
 * @param mapLayer Which layer are we reading from
 * @param tileX,tileY Tile coordinates
 * @returns What data is there in the given layer
 */
int32_t CBMap::getMap(uint8_t mapLayer, int32_t tileX, int32_t tileY) {
	if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight) {
		// There's no data outside the map region
		return 0;
	}
	return layers[mapLayer][tileY * mapWidth + tileX];
}

/** Returns data from the given map layer.
 * @param mapLayer Which layer are we reading from
 * @param x,y World coordinates
 * @returns What data is there in the given layer
 */
int32_t CBMap::getMapWorldCoordinates(uint8_t mapLayer, float x, float y) {
	int tileX = int((x - posX + mapWidth * tileWidth * 0.5f) / tileWidth);
	int tileY = int(-(y - posY - mapHeight * tileHeight * 0.5f) / tileHeight);
	if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight) {
		// There's no data outside the map region
		return 0;
	}
	return layers[mapLayer][tileY * mapWidth + tileX];
}

/** Does a raycast from given object to this map.
 *
 * @param obj From which object will the raycast start
 * @param returnX This variable will be set to the x-coordinate of raycast collision point.
 * @param returnY This variable will be set to the y-coordinate of raycast collision point.
 *
 * @returns Whether the ray hit the object or not.
 */
bool CBMap::rayCast(CBObject *obj, float &returnX, float &returnY) {
	// Convert from world coordinates to tilemap based coordinates
	float startX = obj->getX();
	float startY = obj->getY();
	worldCoordinatesToMapCoordinates(startX, startY);

	float endX = startX + cos((obj->getAngle() / 180.0) * M_PI) * (this->sizeX * this->sizeY);
	float endY = startY - sin((obj->getAngle() / 180.0) * M_PI) * (this->sizeX * this->sizeY);

	// Do the raycast
	bool didRayHit = this->mapRayCast(startX, startY, endX, endY, returnX, returnY);

	// Convert returned values from tilemap based coordinates back to world coordinates
	mapCoordinatesToWorldCoordinates(returnX, returnY);

	return didRayHit;
}

/** Does a raycast between given coordinates (relative to tilemap) and sets the raycast end
 * point to the referenced variables.
 *
 * @param x1,y1 Start point, relative to tilemap
 * @param x2,y2 End point, relative to tilemap
 * @param returnX,returnY The point where ray hit the tilemap is stored in here, relative to tilemap.
 *
 * @returns True if the ray hit a wall, false otherwise.
 */
bool CBMap::mapRayCast(float x1, float y1, float x2, float y2, float &returnX, float &returnY) {
	// Floating point error correction
	x1 += 0.00001f;
	y1 += 0.00001f;
	x2 += 0.00001f;
	y2 += 0.00001f;

	// Normalized start points
	float nX1 = x1 / tileWidth;
	float nY1 = y1 / tileHeight;

	// Tile to be tested
	int testTileX = int(x1 / tileWidth);
	int testTileY = int(y1 / tileHeight);

	float dirX = x2 - x1;
	float dirY = y2 - y1;

	float distSqr = dirX * dirX + dirY * dirY;
	if (distSqr < 0.00001f) {
		// Start and end points are the same
		returnX = x2;
		returnY = y2;
		return false;
	}

	// Find out how far to move on each axis for every whole integer step on the other
	float ratioX, ratioY = 0.0f;
	if (fabs(dirY) > 0.00001f) ratioX = dirX / dirY;
	if (fabs(dirX) > 0.00001f) ratioY = dirY / dirX;

	float nf = 1.0f / sqrt(distSqr);
	dirX *= nf;
	dirY *= nf;

	float deltaX = tileWidth / fabs(dirX);
	float deltaY = tileHeight / fabs(dirY);

	float maxX = testTileX * tileWidth - x1;
	float maxY = testTileY * tileHeight - y1;
	if (dirX >= 0.0f) maxX += tileWidth;
	if (dirY >= 0.0f) maxY += tileHeight;
	maxX /= dirX;
	maxY /= dirY;

	int stepX = dirX < 0 ? -1 : 1;
	int stepY = dirY < 0 ? -1 : 1;
	int endTileX = int(x2 / tileWidth);
	int endTileY = int(y2 / tileHeight);

	while ((testTileX != endTileX || testTileY != endTileY) &&
			!this->outOfBounds(testTileX + 1, testTileY + 1)
	) {
		if (maxX < maxY) {
			maxX += deltaX;
			testTileX += stepX;

			if (this->getHit(testTileX, testTileY)) {
				returnX = testTileX;
				if (stepX < 0) {
					returnX += 1.0f; // If we're going to left, add one.
				}
				returnY = nY1 + ratioY * (returnX - nX1);
				returnX = returnX * tileWidth; // Scale up
				returnY = returnY * tileHeight; // Scale up
				return true;
			}
		}
		else {
			maxY += deltaY;
			testTileY += stepY;

			if (this->getHit(testTileX, testTileY)) {
				returnY = testTileY;
				if (stepY < 0) {
					returnY += 1.0f; // Add one if going up
				}
				returnX = nX1 + ratioX * (returnY - nY1);
				returnX = returnX * tileWidth; // Scale up
				returnY = returnY * tileHeight; // Scale up
				return true;
			}
		}
	}

	returnX = x2;
	returnY = y2;
	return false;
}

/** Checks whether there's a wall between two points (relative to tilemap).
 *
 * @param x1,y1 Start point, relative to tilemap
 * @param x2,y2 End point, relative to tilemap
 *
 * @returns True if the ray hit a wall, false otherwise.
 */
bool CBMap::mapRayCast(float x1, float y1, float x2, float y2) {
	// Floating point error correction
	x1 += 0.00001f;
	y1 += 0.00001f;
	x2 += 0.00001f;
	y2 += 0.00001f;

	// Tile to be tested
	int testTileX = int(x1 / tileWidth);
	int testTileY = int(y1 / tileHeight);

	float dirX = x2 - x1;
	float dirY = y2 - y1;

	float distSqr = dirX * dirX + dirY * dirY;
	if (distSqr < 0.00001f) {
		// Start and end points are the same
		return false;
	}

	float nf = 1.0f / sqrt(distSqr);
	dirX *= nf;
	dirY *= nf;

	float deltaX = tileWidth / fabs(dirX);
	float deltaY = tileHeight / fabs(dirY);

	float maxX = testTileX * tileWidth - x1;
	float maxY = testTileY * tileHeight - y1;
	if (dirX >= 0.0f) maxX += tileWidth;
	if (dirY >= 0.0f) maxY += tileHeight;
	maxX /= dirX;
	maxY /= dirY;

	int stepX = dirX < 0 ? -1 : 1;
	int stepY = dirY < 0 ? -1 : 1;
	int endTileX = int(x2 / tileWidth);
	int endTileY = int(y2 / tileHeight);

	while ((testTileX != endTileX || testTileY != endTileY) &&
			!this->outOfBounds(testTileX + 1, testTileY + 1)
	) {
		if (maxX < maxY) {
			maxX += deltaX;
			testTileX += stepX;
		}
		else {
			maxY += deltaY;
			testTileY += stepY;
		}
		if (this->getHit(testTileX, testTileY)) {
			return true;
		}
	}

	return false;
}

/** Converts tilemap based coordinates to world coordinates */
void CBMap::worldCoordinatesToMapCoordinates(float &x, float &y) {
	x = x + this->mapWidth * this->tileWidth / 2;
	y = -y + this->mapHeight * this->tileHeight / 2;
}


/** Converts world coordinates to tilemap based coordinates */
void CBMap::mapCoordinatesToWorldCoordinates(float &x, float &y) {
	x = x - this->mapWidth * this->tileWidth / 2;
	y = this->mapHeight * this->tileHeight / 2 - y;
}


/** Draws debug box for raycasting */
void CBMap::drawRayCastDebugBox(float tileX, float tileY) {
	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	// Translate coords
	float x = tileX * tileWidth;
	float y = tileY * tileHeight;
	this->mapCoordinatesToWorldCoordinates(x, y);

	rendertarget->useWorldCoords(true);
	rendertarget->drawBox(x, y, tileWidth, tileHeight, false, al_map_rgb(0, 255, 0));
	rendertarget->useWorldCoords(false);
}

/** Are tile coordinates out of map bounds */
bool CBMap::outOfBounds(int tileX, int tileY) {
	return (tileX < 1 || tileX > mapWidth || tileY < 1 || tileY > mapHeight);
}
