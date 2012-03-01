#ifndef OBJECT_H
#define OBJECT_H
#include "precomp.h"
#include "rendertarget.h"

class CBObject{
	public:
		CBObject(bool floor = false);
		~CBObject();
		bool load(string file);
		bool load(string file,const sf::Color &mask);
		bool loadAnimObject(string file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount);
		void paintObject(const sf::Texture &txt);
		void paintObject(const CBObject &obj);
		void moveObject(float fwrd, float sdwrd);
		void positionObject(float x, float y);
		void positionObject(const sf::Vector2f &p) {posX = p.x;posY = p.y;}
		void translateObject(float hor, float ver, float depth);
		void turnObject(float speed);
		void render(RenderTarget &target);
		void setPosition(float x, float y);
		void rotateObject(float a){angle = 180.0+a;}
		void ghostObject(uint8_t ab);
		void maskObject(uint8_t r, uint8_t g, uint8_t b);
		virtual bool isMap()const{return false;}
		float getX();
		float getY();
		void showObject(bool t) {visible = t;}
		sf::Vector2f getPos()const{return sf::Vector2f(posX,posY);}
		float getAngle(); //Funktion nimi on nyt v‰h‰n harhaanjohtava, mutta se nyt saa luvan kelvata koska se on luokan sis‰ll‰.
		void setDrawOrderNumber(int32_t n){drawOrderNumber = n;}
		int32_t getDrawOrderNumber()const{return drawOrderNumber;}
        static void setDefaultVisible(bool t);
		int32_t getObjectInteger()const{return objectIntData;}
		float getObjectFloat()const{return objectFloatData;}
		string getObjectString()const{return objectStringData;}
		void setObjectInteger(int32_t i){objectIntData = i;}
		void setObjectFloat(float f){objectFloatData = f;}
		void setObjectString(const string &s){objectStringData = s;}
		CBObject *copyObject()const;
		void setObjectSize(float x,float y){sizeX = x;sizeY = y;}
		float getObjectSizeX()const{return sizeX;}
		float getObjectSizeY()const{return sizeY;}
        bool isFloorObject()const{return floor;}
	protected:
		//Created using copy
		bool copied;
		bool floor;
		bool painted;
		int32_t drawOrderNumber;
		float posX, posY;
		float sizeX,sizeY;
		float angle;
		bool visible;
		sf::Image *imgtex; //Kuva tekstuurille. V‰rimaskin takia pit‰‰ tehd‰ n‰in vitun vaikeasti. -.-
		sf::Texture *texture; //Tekstuurin "v‰litt‰j‰"
		sf::Sprite sprite; //Ja itse piirt‰j‰.
		uint8_t alphablend;
		uint16_t frameWidth;
		uint16_t frameHeight;
		uint16_t startframe;
		uint16_t maxframes;
		uint16_t currentframe;
		uint8_t picksTyle;
		//CB: ObjectInteger, ObjectFloat, ObjectString
		int32_t objectIntData;
		string objectStringData;
		float objectFloatData;
};

#endif // OBJECT_H
