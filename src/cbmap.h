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

	public:
		CBMap();

		~CBMap();


		bool loadMap(string path);
		bool loadTileset(string path);

		void drawBackLayer(RenderTarget &target, float camX, float camY);
		void drawOverLayer(RenderTarget &target, float camX, float camY);
		void drawTile(int32_t tile,float x,float y);

		void edit(uint8_t maplayer, int32_t MapX, int32_t MapY, int32_t tile);

		int32_t mapSizeX(){return mapWidth;}
		int32_t mapSizeY(){return mapHeight;}

		int32_t getSizeX(){return mapWidth*tileWidth;}
		int32_t getSizeY(){return mapHeight*tileHeight;}
		void playObject();
};



#endif
