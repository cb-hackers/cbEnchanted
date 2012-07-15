#ifndef MAPINTERFACE_H
#define MAPINTERFACE_H

#include "precomp.h"
#include "cbmap.h"

class CBEnchanted;

class MapInterface {
	public:
#ifndef CBE_LIB
		MapInterface();
		~MapInterface();

		void commandEditMap(void);
		void commandSetMap(void);
		void commandSetTile(void);

		void functionLoadMap(void);
		void functionMakeMap(void);
		void functionGetMap(void);
		void functionGetMap2(void);
		void functionMapWidth(void);
		void functionMapHeight(void);
#endif
		CBMap *getTileMap(){return tileMap;}
		void deleteTileMap() { delete tileMap; tileMap = 0; }
	private:
		CBEnchanted *cb;
		CBMap *tileMap;


};

#endif
