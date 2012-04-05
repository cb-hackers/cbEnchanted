
#include "precomp.h"
#include "debug.h"
#include "cbobject.h"
#include <math.h>
#include "cbenchanted.h"

static bool defaultVisible = true;

/** Constructs a new object and inits default values.
 * @param floor Is the object a floor or not, @see ObjectInterface::functionMakeObjectFloor()
 */
CBObject::CBObject(bool floor):
	visible(defaultVisible),
	posX(0),
	posY(0),
	angle(0),
	alphaBlend(255),
	startFrame(0),
	maxFrames(0),
	frameWidth(0),
	frameHeight(0),
	currentFrame(0),
	painted(false),
	floor(floor),
	texture(0),
	copied(false),
	sizeX(0),
	renderTarget(0),
	sizeY(0),
	usingLife(false),
	life(0),
	lastObj(0),
	nextObj(0),
	playing(false)
{
	maskColor = al_map_rgba_f(0, 0, 0, 1);
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
	if (!renderTarget) {
		renderTarget = new RenderTarget;
	}
	if (texture) {
		al_destroy_bitmap(texture);
	}
	renderTarget->copy(&tex);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture, maskColor);
	painted = true;
}

/** Paints the object with another object.
 * @param obj The object whose texture is to be copied to this object
 */
void CBObject::paintObject(const CBObject &obj) {
	if (!renderTarget) {
		renderTarget = new RenderTarget;
	}
	if (texture) {
		al_destroy_bitmap(texture);
	}
	renderTarget->copy(obj.renderTarget);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture, maskColor);
	painted = true;
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

/** Updates the animation and life of an object.
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
	return false;
}

/** Sets the amount of alpha blending of the object.
 * @param ab The amount of blending, range 0...255
 */
void CBObject::ghostObject(uint8_t ab) {
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
	al_destroy_bitmap(texture);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture, al_map_rgb(r, g, b));
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
	float camX = CBEnchanted::instance()->getCameraX();
	float camY = CBEnchanted::instance()->getCameraY();
	if (visible && painted) {
		if (floor) {
			//Drawing floor objects

			float snappedx = floorf(camX / renderTarget->width());
			float snappedy = floorf(camY / renderTarget->height());

			float ekax = camX + snappedx * renderTarget->width();
			float ekay = -camY - snappedy * renderTarget->height();

			float xx = ekax;
			while (xx < target.width()) {
				float yy = ekay;
				while (yy < target.height()) {
					target.drawBitmap(texture, xx, yy);
					yy = yy + renderTarget->height();
				}
				xx = xx + renderTarget->width();
			}
		}
		else { // Non-floor objects
			if (maxFrames != 0) {
				// Draw animated objects
				int32_t frame = (int)currentFrame;
				//INFO("%i", frame)
				int32_t framesX = renderTarget->width() / frameWidth;
				int32_t framesY = renderTarget->height() / frameHeight;
				int32_t copyX = frame % framesX;
				int32_t copyY = (frame - copyX) / framesY;
				//INFO("X, Y: %i,%i", copyX, copyY)
				target.drawBitmapRegion(
					texture,
					copyX * frameWidth,
					copyY * frameHeight,
					frameWidth,
					frameHeight,
					al_map_rgba(255, 255, 255, alphaBlend),
					posX,
					posY,
					(angle * M_PI) / 180.0f
				);
				return;
			}
			if (alphaBlend != 255) {
				// Draw alpha blended objects
				target.drawBitmap(
					texture,
					posX,
					posY,
					(angle / 180.0) * M_PI,
					al_map_rgba(255, 255, 255, alphaBlend)
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
	CBObject *obj = new CBObject(floor);
	obj->texture = texture;
	obj->renderTarget = renderTarget;
	obj->copied = true;
	obj->frameHeight = frameHeight;
	obj->frameWidth = frameWidth;
	obj->visible = true;
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

