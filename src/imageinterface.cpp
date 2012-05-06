#include "precomp.h"
#include "imageinterface.h"
#include "cbenchanted.h"
#include "cbimage.h"
#include "errorsystem.h"


ImageInterface::ImageInterface() {
	cb = static_cast <CBEnchanted *> (this);
	defaultMask = al_map_rgb(0, 0, 0);
	defaultMaskToggled = true;
}

ImageInterface::~ImageInterface() {

}

void ImageInterface::commandSaveImage(void) {
	int32_t frame = cb->popValue().toInt();
	const ISString &path = cb->popValue().toString();
	int32_t id = cb->popValue().toInt();
	CBImage *img = getImage(id);
	img->save(path.getUtf8Encoded());
}

void ImageInterface::commandDrawImage(void) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawImageToWorld() && !cb->drawingOnImage());
	bool useMask = cb->popValue().toInt();
	int32_t frame = cb->popValue().toInt();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->draw(*cb->getCurrentRenderTarget(), x, y, frame, useMask);
}

void ImageInterface::commandDrawGhostImage(void) {
	// TODO: Draw with frames
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

	CBImage *img = cbImages[id];
	img->drawAlphaBlended(*cb->getCurrentRenderTarget(), x, y, alpha);
}

void ImageInterface::commandDrawImageBox(void) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawImageToWorld() && !cb->drawingOnImage());
	bool useMask = cb->popValue().toBool();
	int32_t frame = cb->popValue().toInt();
	float sh = cb->popValue().toFloat();
	float sw = cb->popValue().toFloat();
	float sy = cb->popValue().toFloat();
	float sx = cb->popValue().toFloat();
	float ty = cb->popValue().toFloat();
	float tx = cb->popValue().toFloat();
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->drawBox(*cb->getCurrentRenderTarget(), sx, sy, sw, sh, tx, ty, frame, useMask);
}

void ImageInterface::commandMaskImage(void) {
	int32_t b = cb->popValue().getInt();
	int32_t g = cb->popValue().getInt();
	int32_t r = cb->popValue().getInt();
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->maskImage(al_map_rgb(r, g, b));
}

void ImageInterface::commandDefaultMask(void) {
	int32_t b = cb->popValue().getInt();
	int32_t g = cb->popValue().getInt();
	int32_t r = cb->popValue().getInt();
	defaultMaskToggled = cb->popValue().toBool();

	if (defaultMaskToggled) {
		defaultMask = al_map_rgb(r, g, b);
	}
}

void ImageInterface::commandResizeImage(void) {
	int32_t h = cb->popValue().toInt();
	int32_t w = cb->popValue().toInt();
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->resize(w, h);
}

void ImageInterface::commandRotateImage(void) {
	STUB;
}

void ImageInterface::commandPickImageColor(void) {
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	int32_t imgId = cb->popValue().toInt();
	CBImage *image = getImage(imgId);
	cb->setDrawColor(image->getRenderTarget()->getPixel(x, y));
}

void ImageInterface::commandPickImageColor2(void) {
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	int32_t imgId = cb->popValue().toInt();
	CBImage *image = getImage(imgId);
	cb->setDrawColor(image->getRenderTarget()->getPixel(x, y));
}

void ImageInterface::commandHotSpot(void) {
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	cbImages[id]->setHotspot(x, y);
}

void ImageInterface::commandDeleteImage(void) {
	int32_t id = cb->popValue().getInt();
	delete cbImages[id];
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
	int32_t id = nextId();
	cbImages[id] = image;
	cb->pushValue(id);
}

void ImageInterface::functionCloneImage(void) {
	CBImage *img = cbImages[cb->popValue().getInt()];
	CBImage *newImg = img->clone();
	int32_t id = nextId();
	cbImages[id] = newImg;
	cb->pushValue(id);
}

void ImageInterface::functionImageWidth(void) {
	CBImage *img = cbImages[cb->popValue().getInt()];
	cb->pushValue(img->width());
}

void ImageInterface::functionImageHeight(void) {
	CBImage *img = cbImages[cb->popValue().getInt()];
	cb->pushValue(img->height());
}

void ImageInterface::functionImagesOverlap(void) {
	STUB;
}

void ImageInterface::functionImagesCollide(void) {
	STUB;
}
int32_t ImageInterface::nextId()
{
	return ++idCounter;
}
