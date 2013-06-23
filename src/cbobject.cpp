#include "precomp.h"
#include "debug.h"
#include "cbobject.h"
#include <math.h>
#include "cbenchanted.h"
#include "collisioncheck.h"
#include "camerainterface.h"

static bool defaultVisible = true;

/** Constructs a new object and inits default values.
 * @param floor Is the object a floor or not, @see ObjectInterface::functionMakeObjectFloor()
 */
CBObject::CBObject(bool floor):
	visible(defaultVisible),
	posX(0),
	posY(0),
	angle(0),
	alphaBlend(1.0f),
	startFrame(0),
	maxFrames(0),
	frameWidth(0),
	frameHeight(0),
	currentFrame(0),
	painted(false),
	isFloor(floor),
	texture(0),
	copied(false),
	sizeX(0),
	renderTarget(0),
	sizeY(0),
	usingLife(false),
	life(0),
	beforeObj(0),
	afterObj(0),
	playing(false),
	animEndingFrame(0),
	animSpeed(0),
	animStartFrame(0),
	objectIntData(0),
	objectStringData(),
	objectFloatData(0),
	checkCollisions(true),
	pickStyle(NoPick)
{
	objectRange[0] = 0.0;
	objectRange[1] = 0.0;
	maskColor = al_map_rgb(0, 0, 0);
}

/** Destructor for CBObject.
 *
 * Destroys the attached textures if this object isn't a copy of another object.
 */
CBObject::~CBObject() {
	if (!copied)
	{
		if (renderTarget) {
			delete renderTarget;
		}
		if (texture) {
			al_destroy_bitmap(texture);
		}
	}
}

/** Loads a new object from an image file.
 * @param file Path to the image to be loaded.
 */
bool CBObject::load(const string &file) {
	return load(file, maskColor);
}

/** Loads a new object from an image file masked with a given color.
 * @param file Path to the image to be loaded.
 * @param mask A color to be used as the mask color
 * @returns Was the object loaded successfully
 */
bool CBObject::load(const string &file, const ALLEGRO_COLOR &mask) {
	if (renderTarget) {
		delete renderTarget;
	}
	renderTarget = new RenderTarget;
	if (!renderTarget->load(file)) {
		return false;
	}

	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture, mask);
	maskColor = mask;
	sizeX = renderTarget->width();
	sizeY = renderTarget->height();
	objectRange[0] = sizeX;
	objectRange[1] = sizeY;
	frameWidth = 0;
	frameHeight = 0;
	startFrame = 0;
	maxFrames = 0;
	alphaBlend = 255;
	angle = 0;
	currentFrame = 0;
	painted = true;
	return true;
}

/** Loads an animated object.
 * @param file Path to the image to be loaded
 * @param fw Width of a single frame, in pixels
 * @param fh Height of a single frame, in pixels
 * @param startf The index of the starting frame, zero-based
 * @param framecount How many frames are there
 * @returns Was the object loaded successfully
 */
bool CBObject::loadAnimObject(const string &file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount) {
	if (!load(file)) {
		return false;
	}

	if ((int)fw > renderTarget->width()) {
		INFO("Frame width %u is more than texture width: %u", fw, renderTarget->width());
		return false;
	}
	if ((int)fh > renderTarget->height()) {
		INFO("Frame height %u is more than texture height: %u", fh, renderTarget->height());
		return false;
	}
	if (framecount > (renderTarget->width() / fw) * (renderTarget->height() / fh)) {
		FIXME("Too much frames!")
		return false;
	}
	frameWidth = fw;
	frameHeight = fh;
	sizeX = frameWidth;
	sizeY = frameHeight;
	startFrame = startf;
	maxFrames = framecount;
	alphaBlend = 255;
	angle = 0;
	painted = true;
	currentFrame = 0;
	playing = false;
	return true;
}

/** Positions an object to the given coordinates
 * @param x,y The world coordinates to where the object should be positioned
 */
void CBObject::positionObject(float x, float y) {
	posX = x;
	posY = y;
}

/** Paints the object with a texture.
 * @param tex The new texture
 */
void CBObject::paintObject(const RenderTarget &tex) {
	if (!this->copied) {
		if (!renderTarget) {
			renderTarget = new RenderTarget;
		}
		if (texture) {
			al_destroy_bitmap(texture);
		}
	}
	else {
		renderTarget = new RenderTarget;
		this->copied = false;
	}
	renderTarget->copy(&tex);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture, maskColor);
	painted = true;
	sizeX = tex.width();
	sizeY = tex.height();
}

/** Paints the object with another object.
 * @param obj The object whose texture is to be copied to this object
 */
void CBObject::paintObject(const CBObject &obj) {
	if (!this->copied) {
		if (!renderTarget) {
			renderTarget = new RenderTarget;
		}
		if (texture) {
			al_destroy_bitmap(texture);
		}
	}
	else {
		renderTarget = new RenderTarget;
		this->copied = false;
	}
	if (obj.renderTarget) {
		renderTarget->copy(obj.renderTarget);
		texture = al_clone_bitmap(obj.texture);
		painted = true;
	}
	else {
		renderTarget = 0;
		texture = 0;
		painted = false;
	}
	sizeX = obj.sizeX;
	sizeY = obj.sizeY;
}

/** Sets the animation frame.
 * @param frame Index of the frame, zero-based
 */
void CBObject::setFrame(uint16_t frame) {
	currentFrame = frame;
}

/** Start playing the animation for this object.
 * @param startf From which frame will we start animating
 * @param endf To which index should the animation end
 * @param spd The speed of the animation, the higher the faster
 * @param continuous Should the animation loop continuously
 */
void CBObject::startPlaying(uint16_t startf, uint16_t endf, float spd, bool continuous) {
	if ((!continuous && !playing) ||
		(continuous && (currentFrame < startf || currentFrame > endf))
	) {
		currentFrame = startf;
	}
	animStartFrame = startf;
	animEndingFrame = endf;
	animSpeed = spd;
	playing = true;
}

/** Stops playing the objects animation.
 *
 * @param keepCurrentFrame If true, doesn't reset the current frame to 0.
 */
void CBObject::stopPlaying(bool keepCurrentFrame) {
	playing = false;
	if (!keepCurrentFrame) {
		currentFrame = 0;
	}
}

/** Updates the animation and life of an object and also clears the collisions.
 * @param timestep Something related to animation and timestep. Not used here.
 * @returns Should the object be deleted afterwards
 */
bool CBObject::updateObject(float timestep) {
	if (playing) {
		if (animStartFrame < animEndingFrame) {
			currentFrame = currentFrame + animSpeed;
			if ((int)currentFrame > animEndingFrame) {
				currentFrame = animStartFrame;
				if (!animLooping) {
					playing = false;
				}
			}
		}
		else {
			currentFrame = currentFrame - animSpeed;
			if ((int)currentFrame < animStartFrame) {
				currentFrame = animEndingFrame;
				if (!animLooping) {
					playing = false;
				}
			}
		}
	}
	if (usingLife) {
		--life;
		if (life <= 0) {
			life = 0;
			return true; //Delete object
		}
	}

	// Clear collisions
	eraseCollisions();

	return false;
}

/** Sets the amount of alpha blending of the object.
 * @param ab The amount of blending, range 0.0f ... 1.0f
 */
void CBObject::ghostObject(float ab) {
	alphaBlend = ab;
}

/** Masks the object with the given color values
 * @param r The RED component of the mask color
 * @param g The GREEN component of the mask color
 * @param b The BLUE component of the mask color
 */
void CBObject::maskObject(uint8_t r, uint8_t g, uint8_t b) {
	if (!painted) {
		FIXME("Masking object without texture");
		return;
	}
	maskColor = al_map_rgb(r, g, b);
	al_destroy_bitmap(texture);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture, maskColor);
}

/** Moves the object.
 * @param fwrd Speed of forward movement
 * @param sdwrd Speed of the movement sideways
 */
void CBObject::moveObject(float fwrd, float sdwrd) {
	posX += cos(angle / 180.0 * M_PI) * fwrd + cos((angle - 90.0) / 180.0 * M_PI) * sdwrd;
	posY += sin(angle / 180.0 * M_PI) * fwrd + sin((angle - 90.0) / 180.0 * M_PI) * sdwrd;
}

/** Moves the given object regardless of its angle
 * @param hor Horizontal movement
 * @param ver Vertical movemement
 * @param depth --Not yet implemented--
 */
void CBObject::translateObject(float hor, float ver, float depth) {
	posX += hor;
	posY += ver;
}

/** Turns the object.
 * @param speed How many angles should the object be turned, in degrees
 *
 * Keeps the angle of the object in range 0...360
 */
void CBObject::turnObject(float speed) {
	angle += speed;
	if (angle < 0) {
		while(angle < 0) {
			angle += 360;
		}
	}
	else if (angle > 360) {
		while (angle > 360) {
			angle -= 360;
		}
	}
}

/** Renders the object to the given target.
 * @param target Target to render to
 */
void CBObject::render(RenderTarget &target) {
	if (visible && painted) {
		if (isFloor) { //Drawing floor objects
			CBEnchanted *cb = CBEnchanted::instance();
			float camX = cb->cameraInterface->getCameraX();
			float camY = cb->cameraInterface->getCameraY();

			float scrW = cb->cameraInterface->getDrawAreaWidth();
			float scrH = cb->cameraInterface->getDrawAreaHeight();

			// Calculate the coordinates of visible area
			float areaTop = camY + 0.5f * scrH;
			float areaBottom = camY - 0.5f * scrH;
			float areaLeft = camX - 0.5f * scrW;
			float areaRight = camX + 0.5f * scrW;

			// Coordinates to draw to. Need to flip Y-coordinate cuz CB is weeeird...
			float x = posX;
			float y = -posY;

			// Move drawing coordinates so that they're just outside the visible areas
			// bottom left corner.
			if (x > areaLeft) {
				while (x > areaLeft) {
					x -= sizeX;
				}
			}
			else {
				while (x + sizeX < areaLeft) {
					x += sizeX;
				}
			}

			if (y > areaBottom) {
				while (y > areaBottom) {
					y -= sizeY;
				}
			}
			else {
				while (y + sizeY < areaBottom) {
					y += sizeY;
				}
			}

			// Now do the loop-de-la-loop to fill the visible area with copies of this object.
			for (; x - sizeX < areaRight; x += sizeX) {
				for (float iterY = y; iterY - sizeY < areaTop; iterY += sizeY) {
					if (alphaBlend < 1.0f) {
						target.drawBitmap(texture, x, iterY, al_map_rgba_f(1.0f, 1.0f, 1.0f, alphaBlend));
					}
					else {
						target.drawBitmap(texture, x, iterY);
					}
				}
			}
		}
		else { // Non-floor objects
			if (maxFrames != 0) {
				// Draw animated objects
				int32_t frame = (int)currentFrame;
				//INFO("%i", frame)
				int32_t framesX = renderTarget->width() / frameWidth;
				int32_t copyX = frame % framesX;
				int32_t copyY = frame / framesX;
				target.drawBitmapRegion(
					texture,
					copyX * frameWidth,
					copyY * frameHeight,
					frameWidth,
					frameHeight,
					al_map_rgba_f(1.0f, 1.0f, 1.0f, alphaBlend),
					posX,
					posY,
					-(angle / 180.0) * M_PI
				);
				return;
			}
			if (alphaBlend < 1.0f) {
				// Draw alpha blended objects
				target.drawBitmap(
					texture,
					posX,
					posY,
					-(angle / 180.0) * M_PI,
					al_map_rgba_f(1.0f, 1.0f, 1.0f, alphaBlend)
				);
				return;
			}

			// What's left here is only a regular object without alpha-blending
			target.drawBitmap(texture, posX, posY, -(angle / 180.0) * M_PI);
		}
	}
}

/** Positions the object to the given coordinates.
 * @param x,y World coordinates to set the object to
 */
void CBObject::setPosition(float x, float y) {
	posX = x;
	posY = y;
}

/** @returns X position of the object */
float CBObject::getX() {
	return posX;
}

/** @returns Y position of the object */
float CBObject::getY() {
	return posY;
}

/** @returns Angle of the object */
float CBObject::getAngle() {
	return angle;
}

/** Sets the default visibility for ALL objects */
void CBObject::setDefaultVisible(bool t) {
	defaultVisible = t;
}

/** Copies an object so that this can use the texture from the master object */
CBObject *CBObject::copyObject() const {
	CBObject *obj = new CBObject(isFloor);
	obj->texture = texture;
	obj->maskColor = this->maskColor;
	obj->renderTarget = renderTarget;
	obj->copied = true;
	obj->frameHeight = frameHeight;
	obj->frameWidth = frameWidth;
	obj->animEndingFrame = this->animEndingFrame;
	obj->animLooping = this->animLooping;
	obj->animSpeed = this->animSpeed;
	obj->animStartFrame = this->animStartFrame;
	obj->visible = true;
	obj->maxFrames = this->maxFrames;
	obj->painted = this->painted;
	obj->objectRange[0] = this->sizeX;
	obj->objectRange[1] = this->sizeY;
	return obj;
}

/** Sets the life of an object, @see updateObject */
void CBObject::setLife(uint32_t energy) {
	usingLife = true;
	life = energy;
}

/** @returns The life of this object */
uint32_t CBObject::getLife() {
	return life;
}

/** @returns Is the object using life or not */
bool CBObject::isLife() {
	return usingLife;
}

/** Adds a collision to collisionList */
void CBObject::addCollision(Collision *collision) {
	collisionList.push_back(collision);
}

/** Gets a collision from collisionList */
Collision* CBObject::getCollision(uint32_t id) {
	if (collisionList.size() >= id) {
		return collisionList.at(id - 1);
	}
	else {
		return NULL;
	}
}

/** Does a raycast from given object to this object.
 *
 * @param fromObject From which object will the raycast start
 * @param returnX This variable will be set to the x-coordinate of raycast collision point.
 * @param returnY This variable will be set to the y-coordinate of raycast collision point.
 *
 * @returns Whether the ray hit the object or not.
 */
bool CBObject::rayCast(CBObject *fromObject, float &returnX, float &returnY) {
	switch (this->pickStyle) {
		case BoxPick:
			return this->boxRayCast(fromObject, returnX, returnY);
		case CirclePick:
			return this->circleRayCast(fromObject, returnX, returnY);
		default:
			return false;
	}
}

/** Sets the way this object is picked.
 *
 * @param style The way this object is to be picked. 1 = Box, 2 = Circle, 3 = Pixel
 * @returns Was the pick style OK
 */
bool CBObject::setPickStyle(int32_t style) {
	switch (style) {
		case 1:
			pickStyle = BoxPick;
			INFO("Set BoxPick for object %i", this->id);
			return true;
		case 2:
			pickStyle = CirclePick;
			INFO("Set CirclePick for object %i", this->id);
			return true;
		case 3:
			pickStyle = PixelPick;
			INFO("Set PixelPick for object %i", this->id);
			return true;
		default:
			FIXME("Unsupported pick type %i", style);
			return false;
	}
}

/** Mirrors this object vertically and/or horizontally. */
void CBObject::mirrorObject(int32_t dir) {
	if (dir < 0 || dir > 2) {
		return;
	}

	RenderTarget *newRT = new RenderTarget;
	newRT->create(renderTarget->width(), renderTarget->height());

	switch (dir) {
		case 0: // Horizontal
			newRT->drawBitmapMirrored(renderTarget->getBitmap(), ALLEGRO_FLIP_HORIZONTAL);
		break;
		case 1: // Vertical
			newRT->drawBitmapMirrored(renderTarget->getBitmap(), ALLEGRO_FLIP_VERTICAL);
		break;
		case 2: // Both
			newRT->drawBitmapMirrored(renderTarget->getBitmap(), ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);
		break;
	}

	if (!this->copied) {
		if (renderTarget) {
			delete renderTarget;
		}
		if (texture) {
			al_destroy_bitmap(texture);
		}
	}
	else {
		this->copied = false;
	}
	renderTarget = newRT;
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture, maskColor);
	painted = true;
}

/** Does a raycast from given object to this circle-shaped object.
 *
 * @param fromObject From which object will the raycast start
 * @param returnX This variable will be set to the x-coordinate of raycast collision point.
 * @param returnY This variable will be set to the y-coordinate of raycast collision point.
 *
 * @returns Whether the ray hit the object or not.
 *
 * @see http://stackoverflow.com/a/1084899/1152564
 */
bool CBObject::circleRayCast(CBObject *fromObject, float &returnX, float &returnY) {
	// Start point
	float startX = fromObject->getX();
	float startY = fromObject->getY();

	// Center of circle
	float circleX = this->getX();
	float circleY = this->getY();
	// Radius of circle
	float r = this->getRange1() / 2;

	// Vector from center of circle to ray start
	float cvX = startX - circleX;
	float cvY = startY - circleY;

	// If ray start point is inside this circle, don't pick this.
	if ((cvX * cvX + cvY * cvY) < r*r) {
		returnX = startX;
		returnY = startY;
		return false;
	}

	// Ray end point
	float endX = startX + cos((fromObject->getAngle() / 180.0) * M_PI) * 1e7;
	float endY = startY + sin((fromObject->getAngle() / 180.0) * M_PI) * 1e7;

	// Direction vector of ray, from start to end
	float dirX = endX - startX;
	float dirY = endY - startY;

	// Solve quadratic product: t^2 * (d DOT d) + 2t*( f DOT d ) + ( f DOT f - r^2 ) = 0
	// where d is direction vector and f is vector from center of circle to ray start

	//float a = d.Dot( d );
	float a = dirX * dirX + dirY * dirY;
	//float b = 2*f.Dot( d ) ;
	float b = 2 * (dirX * cvX + dirY * cvY);
	//float c = f.Dot( f ) - r*r ;
	float c = (cvX * cvX + cvY * cvY) - r * r;

	float discriminant = b * b - 4 * a *c;
	if( discriminant < 0 ) {
		// no intersection
		returnX = endX;
		returnY = endY;
		return false;
	}
	else {
		// ray didn't totally miss sphere,
		// so there is a solution to
		// the equation.


		discriminant = sqrt(discriminant);
		float t1 = (-b + discriminant) / (2 * a);
		float t2 = (-b - discriminant) / (2 * a);

		if (t2 >= 0 && t2 <= 1) {
			// solution on is ON THE RAY.
			returnX = startX + t2 * dirX;
			returnY = startY + t2 * dirY;
			return true;
		}
		else {
			// solution "out of range" of ray
		}

		// use t1 for second point
		if (t1 >= 0 && t1 <= 1) {
			// solution on is ON THE RAY.
			returnX = startX + t1 * dirX;
			returnY = startY + t1 * dirY;
			return true;
		}
		else {
			// solution "out of range" of ray
		}

		// If we get here, there's no hit.
		returnX = endX;
		returnY = endY;
		return false;
	}
}

/** Does a raycast from given object to this rectangle-shaped object.
 *
 * @param fromObject From which object will the raycast start
 * @param returnX This variable will be set to the x-coordinate of raycast collision point.
 * @param returnY This variable will be set to the y-coordinate of raycast collision point.
 *
 * @returns Whether the ray hit the object or not.
 */
bool CBObject::boxRayCast(CBObject *fromObject, float &returnX, float &returnY) {
	// Get box top left corner coordinates and width and height for collision box
	float rectW = this->getRange1();
	float rectH = this->getRange2();
	float rectX = this->getX() - rectW/2;
	float rectY = this->getY() + rectH/2;

	// For simplicity, calculate rectangle side coordinates here
	float left = rectX;
	float top = rectY;
	float right = rectX + rectW;
	float bottom = rectY - rectH;

	float startX = fromObject->getX();
	float startY = fromObject->getY();
	float angle = fromObject->getAngle();

	float x,y;
	float k = tan((angle / 180.0) * M_PI);
	float b = startY - k * startX;

	if (angle > 180) {
		// Top side
		y = top;
		x = (y - b) / k;
		if (startY > y && x > left && x < right) {
			returnX = x;
			returnY = y;
			return true;
		}
	}
	else {
		// Bottom side
		y = bottom;
		x = (y - b) / k;
		if (startY < y && x > left && x < right) {
			returnX = x;
			returnY = y;
			return true;
		}
	}

	if (angle < 90 || angle > 270) {
		// Left side
		x = left;
		y = k * x + b;
		if (startX < x && y > bottom && y < top) {
			returnX = x;
			returnY = y;
			return true;
		}
	}
	else {
		// Right side
		x = right;
		y = k * x + b;
		if (startX > x && y > bottom && y < top) {
			returnX = x;
			returnY = y;
			return true;
		}
	}

	// If we ended up here, ray doesn't cross the rectangle.
	return false;
}

/** Is the given world coordinate inside this object.
 *
 * @param x,y World coordinates to check against.
 */
bool CBObject::canPick(float x, float y) {
	if (pickStyle == BoxPick) {
		float w = getRange1();
		float h = getRange2();
		if ((fabs(posX - x) < w / 2) && (fabs(posY - y) < h / 2)) {
			return true;
		}
	}
	else if (pickStyle == CirclePick) {
		float r = getRange1() * 0.5f;
		float dx = posX - x;
		float dy = posY - y;

		float distSq = dx * dx + dy * dy;
		if (distSq < r*r) {
			return true;
		}
	}
	return false;
}
