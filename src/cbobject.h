#ifndef OBJECT_H
#define OBJECT_H
#include "precomp.h"
#include "rendertarget.h"
#include "collision.h"

class CBObject{
	public:
		enum Type {
			Object,
			Map,
			ParticleEmitter
		};

		CBObject(bool floor = false);
		virtual ~CBObject();
		bool load(const string &file);
		bool load(const string &file,const sf::Color &mask);
		bool loadAnimObject(const string &file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount);
		void paintObject(const sf::Texture &txt);
		void paintObject(const CBObject &obj);
		void moveObject(float fwrd, float sdwrd);
		void positionObject(float x, float y);
		void positionObject(const sf::Vector2f &p) {posX = p.x;posY = p.y;}
		void translateObject(float hor, float ver, float depth);
		void turnObject(float speed);
		virtual void render(RenderTarget &target);
		void setPosition(float x, float y);
		void rotateObject(float a){angle = a;}
		void ghostObject(uint8_t ab);
		void maskObject(uint8_t r, uint8_t g, uint8_t b);
		virtual bool isMap()const{return false;}
		virtual Type type()const{return Object;}
		float getX();
		float getY();
		void showObject(bool t) {visible = t;}
		sf::Vector2f getPos()const{return sf::Vector2f(posX,posY);}
		float getAngle(); //Funktion nimi on nyt vähän harhaanjohtava, mutta se nyt saa luvan kelvata koska se on luokan sisällä.
		static void setDefaultVisible(bool t);
		int32_t getObjectInteger()const{return objectIntData;}
		float getObjectFloat()const{return objectFloatData;}
		string getObjectString()const{return objectStringData;}
		void setObjectInteger(int32_t i){objectIntData = i;}
		void setObjectFloat(float f){objectFloatData = f;}
		void setObjectString(const string &s){objectStringData = s;}
		void setFrame(uint16_t frame);
		CBObject *copyObject()const;
		void setObjectSize(float x,float y){sizeX = x;sizeY = y;}
		float getObjectSizeX()const{return sizeX;}
		float getObjectSizeY()const{return sizeY;}
		bool isFloorObject()const{return floor;}
		void setLife(uint32_t energy);
		bool isPlaying()const{return playing;}
		void startPlaying(uint16_t startf, uint16_t endf, float spd, bool continuous);
		void setLooping(bool t){animLooping = t;}
		bool isLooping()const{return animLooping;}
		inline uint8_t isAnimated()const{return (maxFrames > 0);}
		inline float getCurrentFrame(){return currentFrame;}

		virtual bool updateObject(float timestep);

		uint32_t getLife();
		bool isLife();
		//Draw order
		CBObject *nextObj;
		CBObject *lastObj;
	protected:
		//Created using copy
		bool copied;
		bool floor;
		bool painted;
		float posX, posY;
		float sizeX,sizeY;
		float angle;
		bool visible;
		sf::Image *imgtex; //Kuva tekstuurille. Värimaskin takia pitää tehdä näin vitun vaikeasti. -.-
		sf::Texture *texture; //Tekstuurin "välittäjä"
		sf::Sprite sprite; //Ja itse piirtäjä.
		uint8_t alphaBlend;
		uint16_t frameWidth;
		uint16_t frameHeight;
		uint16_t startFrame;
		uint16_t maxFrames;
		float currentFrame;
		uint8_t picksTyle;
		//CB: ObjectInteger, ObjectFloat, ObjectString
		int32_t objectIntData;
		string objectStringData;
		float objectFloatData;
		bool usingLife; //Elämä käytössä?
		uint32_t life;

		uint16_t animStartFrame;
		uint16_t animEndingFrame;
		float animSpeed;
		bool animLooping;
		bool playing;
		std::vector<Collision> collisionList;
};

#endif // OBJECT_H
