#ifndef _CBMAP_H_
#define _CBMAP_H_

#include "precomp.h"
#include "cbobject.h"


/** Class for tilemaps */
class CBMap : public CBObject {
	public:
		CBMap();

		~CBMap();

		Type type()const{return Map;}

		bool loadMap(string path);
		bool loadTileset(string path);
		/** Changes the map tileset */
		void paintObject(const RenderTarget &tex);
		bool create(uint32_t width, uint32_t height, uint16_t tileW, uint16_t tileH);
		void drawLayer(uint8_t level, RenderTarget &target);
		void render(RenderTarget &target);
		bool isMap() const { return true; }

		void drawTile(RenderTarget &target, int32_t tile,float x,float y);

		void edit(uint8_t maplayer, int32_t MapX, int32_t tileY, int32_t tileIndex);
		int32_t getMap(uint8_t mapLayer, int32_t MapX, int32_t tileY);
		int32_t getMapWorldCoordinates(uint8_t mapLayer, float x, float y);
		int32_t getHit(int32_t x, int32_t y);
		int32_t mapSizeX(){return mapWidth;}
		int32_t mapSizeY(){return mapHeight;}

		int32_t getSizeX() const {
			return mapWidth * tileWidth;
		}
		int32_t getSizeY() const {
			return mapHeight * tileHeight;
		}

		int32_t getTileWidth() const {return tileWidth;}
		int32_t getTileHeight() const {return tileHeight;}

		void setLayers(uint8_t back, uint8_t over);
		void setTile(uint32_t tile, uint32_t length, uint32_t slowness);
		bool updateObject(float timestep);

		/** Does a raycast according to object position and angle and sets the raycast end point
		 * to the referenced variables. */
		bool rayCast(CBObject *obj, float &returnX, float &returnY);

		/** Does a raycast between given coordinates (relative to tilemap) and sets the raycast end
		 * point to the referenced variables. */
		bool mapRayCast(float x1, float y1, float x2, float y2, float &returnX, float &returnY);

		/** Checks whether there's a wall between two points (relative to tilemap). */
		bool mapRayCast(float x1, float y1, float x2, float y2);

		/** Converts tilemap based coordinates to world coordinates */
		void mapCoordinatesToWorldCoordinates(float &x, float &y);
		/** Converts world coordinates to tilemap based coordinates */
		void worldCoordinatesToMapCoordinates(float &x, float &y);
	private:

		int32_t *layers[4];

		uint32_t tileCount;
		int32_t mapWidth;
		int32_t mapHeight;
		int32_t tileWidth;
		int32_t tileHeight;

		uint8_t maskR, maskG, maskB;

		int32_t *animLength;
		int32_t *animSlowness;
		float *currentFrame;
		uint8_t layerShowing[2];

		/** Draws debug box for raycasting */
		void drawRayCastDebugBox(float tileX, float tileY);

		/** Are tile coordinates out of map bounds */
		bool outOfBounds(int tileX, int tileY);
};



#endif
