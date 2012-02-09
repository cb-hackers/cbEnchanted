#ifndef MAPINTERFACE_H
#define MAPINTERFACE_H

#include "cbmap.h"

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
	
		CBMap *map;
};

#endif
