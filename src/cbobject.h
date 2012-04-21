#ifndef OBJECT_H
#define OBJECT_H

#include "precomp.h"
#include "rendertarget.h"
#include "collision.h"
#include "isstring.h"

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
		enum PickStyle {
			NoPick,
			BoxPick,
			CirclePick,
			PixelPick
		};

		CBObject(bool isFloor = false);
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
		ISString getObjectString()const{return objectStringData;}
		void setObjectInteger(int32_t i){objectIntData = i;}
		void setObjectFloat(float f){objectFloatData = f;}
		void setObjectString(const ISString &s){objectStringData = s;}
		void setFrame(uint16_t frame);
		CBObject *copyObject()const;
		void setObjectSize(float x,float y){sizeX = x;sizeY = y;}
		float getObjectSizeX()const{return sizeX;}
		float getObjectSizeY()const{return sizeY;}
		bool isFloorObject()const{return isFloor;}
		void setLife(uint32_t energy);
		bool isPlaying()const{return playing;}
		void startPlaying(uint16_t startf, uint16_t endf, float spd, bool continuous);
		void setLooping(bool t){animLooping = t;}
		bool isLooping()const{return animLooping;}
		inline uint8_t isAnimated()const{return (maxFrames > 0);}
		inline float getCurrentFrame(){return currentFrame;}

		virtual bool updateObject(float timestep);

		/** Sets object range, @see getRange1 and getRange2 */
		void setRange(float range1, float range2) { objectRange[0] = range1; objectRange[1] = range2; }
		/** Returns object range, box width in Box-collision and diameter in Circle-collision. */
		float getRange1() const { return objectRange[0]; }
		/** Returns object range, box height in Box-collision. */
		float getRange2() const { return objectRange[1]; }

		/** Adds a collision to collisionList. */
		void addCollision(Collision *collision);
		/** Gets the amount of collisions. */
		int32_t getCollisionCount() const { return collisionList.size(); }
		/** Gets a collision from collisionList */
		Collision* getCollision(int32_t id) { return collisionList.at(id - 1); }
		/** Sets collisions to be skipped or checked until next updateObject comes along. */
		void setCollisionChecking(bool checkOrNot) { checkCollisions = checkOrNot; }
		/** Should collision checking be made for this object or not. */
		bool isCollisionsOn() const { return checkCollisions; }

		/** Sets object ID which should be the same as the key stored in ObjectInterface::objectMap */
		void setID(int32_t pId) { id = pId; }
		/** Returns object ID */
		int32_t getID() const { return id; }

		/** Does a raycast from given object to this object. */
		virtual bool rayCast(CBObject *fromObject, float &returnX, float &returnY);

		/** Sets the way this object is picked. */
		bool setPickStyle(int32_t style);


		uint32_t getLife();
		bool isLife();
		//Draw order
		CBObject *afterObj;
		CBObject *beforeObj;
	protected:
		/** Is the object created as a copy */
		bool copied;
		/** Is the object a floor made with MakeObjectFloor() */
		bool isFloor;
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

		/** Integer data stored with ObjectInteger */
		int32_t objectIntData;
		/** String data stored with ObjectString */
		ISString objectStringData;
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

		/** The ID this object has. */
		int32_t id;

		/** Collisions. */
		std::vector<Collision*> collisionList;
		/** Should collision check be made for this object. */
		bool checkCollisions;
		/** Object range for collisions. */
		float objectRange[2];

		/** How the object can be picked if set. */
		PickStyle pickStyle;

		/** Does a raycast from given object to this circle-shaped object. */
		bool circleRayCast(CBObject *fromObject, float &returnX, float &returnY);
		/** Does a raycast from given object to this rectangle-shaped object. */
		bool boxRayCast(CBObject *fromObject, float &returnX, float &returnY);

		/** A helper function that tests whether two lines intersect. */
		bool linesIntersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float &retX, float &retY);
};

#endif // OBJECT_H
