#include "precomp.h"
#include "imageinterface.h"
#include "cbenchanted.h"
#include "cbimage.h"
#include "errorsystem.h"
#include "gfxinterface.h"

#ifndef CBE_LIB

ImageInterface::ImageInterface() {
	cb = CBEnchanted::instance();
	defaultMaskToggled = true;
	defaultHotspotToggled = false;
	defaultHotspotX = -1;
	defaultHotspotY = -1;
}

ImageInterface::~ImageInterface() {

}

void ImageInterface::initializeImages() {
	defaultMask = al_map_rgb(0, 0, 0);
}

void ImageInterface::commandSaveImage(void) {
	int32_t frame = cb->popValue().toInt();
	const ISString &path = cb->popValue().toString();
	int32_t id = cb->popValue().toInt();
	CBImage *img = getImage(id);
	img->save(path.getUtf8Encoded());
}

void ImageInterface::commandDrawImage(void) {
	cb->gfxInterface->getCurrentRenderTarget()->useWorldCoords(cb->gfxInterface->getDrawImageToWorld() && !cb->gfxInterface->drawingOnImage());
	bool useMask = cb->popValue().toInt();
	int32_t frame = cb->popValue().toInt();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	CBImage *img = getImage(cb->popValue().getInt());
	img->draw(*cb->gfxInterface->getCurrentRenderTarget(), x, y, frame, useMask);
}

void ImageInterface::commandDrawGhostImage(void) {
	// TODO: Draw with frames
	cb->gfxInterface->getCurrentRenderTarget()->useWorldCoords(cb->gfxInterface->getDrawImageToWorld() && !cb->gfxInterface->drawingOnImage());
	float alpha = cb->popValue().toFloat() / 100.0f;
	int32_t frame = cb->popValue().toInt();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();

	if (alpha < 0.0f) {
		alpha = 0.0f;
	}
	else if (alpha > 1.0f) {
		alpha = 1.0f;
	}

	CBImage *img = getImage(id);
	img->drawAlphaBlended(*cb->gfxInterface->getCurrentRenderTarget(), x, y, frame, alpha);
}

void ImageInterface::commandDrawImageBox(void) {
	cb->gfxInterface->getCurrentRenderTarget()->useWorldCoords(cb->gfxInterface->getDrawImageToWorld() && !cb->gfxInterface->drawingOnImage());
	bool useMask = cb->popValue().toBool();
	int32_t frame = cb->popValue().toInt();
	float sh = cb->popValue().toFloat();
	float sw = cb->popValue().toFloat();
	float sy = cb->popValue().toFloat();
	float sx = cb->popValue().toFloat();
	float ty = cb->popValue().toFloat();
	float tx = cb->popValue().toFloat();
	CBImage *img = getImage(cb->popValue().getInt());
	img->drawBox(*cb->gfxInterface->getCurrentRenderTarget(), sx, sy, sw, sh, tx, ty, frame, useMask);
}

void ImageInterface::commandMaskImage(void) {
	int32_t b = cb->popValue().getInt();
	int32_t g = cb->popValue().getInt();
	int32_t r = cb->popValue().getInt();
	CBImage *img = getImage(cb->popValue().getInt());
	img->maskImage(al_map_rgba(r, g, b, 255));
}

void ImageInterface::commandDefaultMask(void) {
	int32_t b = cb->popValue().getInt();
	int32_t g = cb->popValue().getInt();
	int32_t r = cb->popValue().getInt();
	defaultMaskToggled = cb->popValue().toBool();

	if (defaultMaskToggled) {
		defaultMask = al_map_rgba(r, g, b, 255);
	}
}

void ImageInterface::commandResizeImage(void) {
	int32_t h = cb->popValue().toInt();
	int32_t w = cb->popValue().toInt();
	CBImage *img = getImage(cb->popValue().getInt());
	img->resize(w, h);
}

void ImageInterface::commandRotateImage(void) {
	float angle = cb->popValue().toFloat();
	CBImage *img = getImage(cb->popValue().getInt());
	img->rotate(angle);
}

void ImageInterface::commandPickImageColor(void) {
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	int32_t imgId = cb->popValue().toInt();
	CBImage *image = getImage(imgId);
	cb->gfxInterface->setDrawColor(image->getRenderTarget()->getPixel(x, y));
}

void ImageInterface::commandPickImageColor2(void) {
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	int32_t imgId = cb->popValue().toInt();
	CBImage *image = getImage(imgId);
	cb->gfxInterface->setDrawColor(image->getRenderTarget()->getPixel(x, y));
}

void ImageInterface::commandHotSpot(void) {
	int32_t y = cb->popValue().getInt();
	int32_t x = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	if (id == 0) { //OFF
		defaultHotspotToggled = false;
	}
	else if (id == 1) { //ON
		defaultHotspotToggled = true;
		defaultHotspotX = x;
		defaultHotspotY = y;
	}
	else { // Image ID
		CBImage *img = getImage(id);
		img->setHotspot(x, y);
	}
}

void ImageInterface::commandDeleteImage(void) {
	int32_t id = cb->popValue().getInt();
	delete getImage(id);
	cbImages.erase(id);
}

void ImageInterface::functionLoadImage(void) {
	ALLEGRO_PATH *path = cb->popValue().getString().getPath();
	const char *cpath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

	CBImage *image = new CBImage;
	if(!image->load(cpath)){
		cb->errors->createError("LoadImage() failed!", "Failed to load file \"" + string(cpath) + "\"");
		cb->pushValue(0);
		al_destroy_path(path);
		return;
	}
	al_destroy_path(path);
	if (defaultMaskToggled) {
		image->maskImage(defaultMask);
	}
	if (defaultHotspotToggled) {
		image->setHotspot(defaultHotspotX, defaultHotspotY);
	}

	int32_t id = nextId();
	cbImages[id] = image;
	cb->pushValue(id);
}

void ImageInterface::functionLoadAnimImage(void) {
	int32_t animL = cb->popValue().toInt();
	int32_t startF = cb->popValue().toInt();
	int32_t frameH = cb->popValue().toInt();
	int32_t frameW = cb->popValue().toInt();
	ALLEGRO_PATH *path = cb->popValue().getString().getPath();
	const char *cpath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

	CBImage *image = new CBImage;
	if(!image->load(cpath)){
		cb->errors->createError("LoadAnimImage() failed!", "Failed to load file \"" + string(cpath) + "\"");
		cb->pushValue(0);
		al_destroy_path(path);
		return;
	}
	al_destroy_path(path);

	image->setAnimParams(frameW, frameH, startF, animL);
	if (defaultMaskToggled) {
		image->maskImage(defaultMask);
	}
	if (defaultHotspotToggled) {
		image->setHotspot(defaultHotspotX, defaultHotspotY);
	}

	int32_t id = nextId();
	cbImages[id] = image;
	cb->pushValue(id);
}

void ImageInterface::functionMakeImage(void) {
	int32_t frameCount = cb->popValue().toInt();
	int32_t h = cb->popValue().toInt();
	int32_t w = cb->popValue().toInt();
	CBImage *image = new CBImage;
	image->makeImage(w, h);
	if (defaultMaskToggled) {
		image->maskImage(defaultMask);
	}
	if (defaultHotspotToggled) {
		image->setHotspot(defaultHotspotX, defaultHotspotY);
	}

	int32_t id = nextId();
	cbImages[id] = image;
	cb->pushValue(id);
}

void ImageInterface::functionCloneImage(void) {
	CBImage *img = getImage(cb->popValue().getInt());
	CBImage *newImg = img->clone();
	int32_t id = nextId();
	cbImages[id] = newImg;
	cb->pushValue(id);
}

void ImageInterface::functionImageWidth(void) {
	CBImage *img = getImage(cb->popValue().getInt());
	cb->pushValue(img->width());
}

void ImageInterface::functionImageHeight(void) {
	CBImage *img = getImage(cb->popValue().getInt());
	cb->pushValue(img->height());
}

void ImageInterface::functionImagesOverlap(void) {
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	int32_t id2 = cb->popValue().getInt();
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();
	int32_t id1 = cb->popValue().getInt();

	CBImage *img1 = getImage(id1);
	CBImage *img2 = getImage(id2);


	if (img1->overlaps(img2, x1, y1, x2, y2)) {
		cb->pushValue(1);
	}
	else {
		cb->pushValue(0);
	}
}

void ImageInterface::functionImagesCollide(void) {
	int32_t startFrame2 = cb->popValue().toInt();
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	int32_t id2 = cb->popValue().getInt();
	int32_t startFrame1 = cb->popValue().toInt();
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();
	int32_t id1 = cb->popValue().getInt();

	CBImage *img1 = getImage(id1);
	CBImage *img2 = getImage(id2);

	if (img1->collides(img2, x1, y1, x2, y2)) {
		cb->pushValue(1);
	}
	else {
		cb->pushValue(0);
	}
}

void ImageInterface::cbeMakeImage(void) {
	int32_t h = cb->popValue().toInt();
	int32_t w = cb->popValue().toInt();
	CBImage *image = new CBImage;
	image->makeImageTransparent(w, h);
	if (defaultMaskToggled) {
		image->maskImage(defaultMask);
	}
	if (defaultHotspotToggled) {
		image->setHotspot(defaultHotspotX, defaultHotspotY);
	}

	int32_t id = nextId();
	cbImages[id] = image;
	cb->pushValue(id);
}
#endif

CBImage *ImageInterface::getImage(int32_t id) {
	map<int32_t, CBImage*>::iterator imgI = cbImages.find(id);
	if (imgI == cbImages.end()) {
		cb->errors->createError("Image Access Violation", "Could not find image with ID " + boost::lexical_cast<string>(id));
		return 0;
	}
	return imgI->second;
}
