#ifndef MAPINTERFACE_H
#define MAPINTERFACE_H

#include "precomp.h"
#include "cbmap.h"

class CBEnchanted;

class MapInterface {
	public:
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
	private:
		CBEnchanted *cb;
		CBMap *tileMap;
};

#endif
