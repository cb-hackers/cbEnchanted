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
<<<<<<< HEAD
		CBMap *tileMap;
=======

		CBMap *getTileMap(){return tileMap;}
>>>>>>> 0797ca189d153b122ecbcc94fe72a3972c3abfef
	private:
		CBEnchanted *cb;


};

#endif
