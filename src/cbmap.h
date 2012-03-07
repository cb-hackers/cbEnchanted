#ifndef _CBMAP_H_
#define _CBMAP_H_

#include "precomp.h"
#include "cbobject.h"

class CBMap : public CBObject{


		int32_t *layer[4];

		int32_t tilecount;
		int32_t mapWidth;
		int32_t mapHeight;
		int32_t tileWidth;
		int32_t tileHeight;


		uint8_t maskR, maskG, maskB;

		int32_t *animLenght;
		int32_t *animSlowness;
		float *currentFrame;
		uint8_t layerShowing[2];

	public:
		CBMap();

		~CBMap();


		bool loadMap(string path);
		bool loadTileset(string path);
		bool create(uint32_t width, uint32_t height, uint16_t tileW, uint16_t tileH);
		void drawLayer(uint8_t level, RenderTarget &target);

		void drawTile(RenderTarget &target, int32_t tile,float x,float y, uint16_t frame = 0);

		void edit(uint8_t maplayer, int32_t MapX, int32_t MapY, int32_t tile);
		int32_t getMap(uint8_t maplayer, int32_t MapX, int32_t MapY);

		int32_t mapSizeX(){return mapWidth;}
		int32_t mapSizeY(){return mapHeight;}

		int32_t getSizeX(){return mapWidth*tileWidth;}
		int32_t getSizeY(){return mapHeight*tileHeight;}

		int32_t getTileWidth(){return tileHeight;}
		int32_t getTileHeight(){return tileWidth;}

		void setLayers(uint8_t back, uint8_t over);
		void playObject();
		void setTile(uint32_t tile, uint32_t lenght, uint32_t slowness);
		void paintObject(const sf::Texture &txt);
};



#endif
