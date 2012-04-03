#ifndef OBJECT_H
#define OBJECT_H

#include "precomp.h"
#include "rendertarget.h"
#include "collision.h"

/** Implementation of CoolBasic objects.
 * Maps and particles inherit this class.
 */
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
		bool load(const string &file,const ALLEGRO_COLOR &mask);
		bool loadAnimObject(const string &file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount);
		void paintObject(const RenderTarget &tex);
		void paintObject(const CBObject &obj);
		void moveObject(float fwrd, float sdwrd);
		void positionObject(float x, float y);
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
		/** Is the object created as a copy */
		bool copied;
		/** Is the object a floor made with MakeObjectFloor() */
		bool floor;
		/** Is the object painted */
		bool painted;

		/** The world coordinate position of the object */
		float posX, posY;
		/** Object size */
		float sizeX, sizeY;
		/** Object angle, in degrees */
		float angle;

		/** Is the object visible */
		bool visible;
		/** The object is loaded to this RenderTarget */
		RenderTarget *renderTarget;
		/** Holds the masked bitmap data */
		ALLEGRO_BITMAP *texture;
		/** Mask color */
		ALLEGRO_COLOR maskColor;
		/** Amount of alpha blending, 0...255 */
		uint8_t alphaBlend;

		/** Width of a single frame, px */
		uint16_t frameWidth;
		/** Height of a single frame, px */
		uint16_t frameHeight;
		/** Index of the starting frame, zero-based */
		uint16_t startFrame;
		/** Maximum amount of frames */
		uint16_t maxFrames;
		/** Current frame */
		float currentFrame;

		/** Pick style. Unimplemented. */
		uint8_t pickStyle;

		/** Integer data stored with ObjectInteger */
		int32_t objectIntData;
		/** String data stored with ObjectString */
		string objectStringData;
		/** Float data stored with ObjectFloat */
		float objectFloatData;

		/** Is the object using life, related to particles */
		bool usingLife;
		/** Amount of life left, related to particles */
		uint32_t life;

		/** Index of starting frame, zero-based */
		uint16_t animStartFrame;
		/** Index of ending frame, zero-based */
		uint16_t animEndingFrame;
		/** Speed of animation, the higher the faster */
		float animSpeed;
		/** Is the animation looping continuously */
		bool animLooping;
		/** Is the animation currently playing */
		bool playing;

		/** Collision data. Unimplemented. */
		std::vector<Collision> collisionList;
};

#endif // OBJECT_H
