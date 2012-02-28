#include "precomp.h"
#include "imageinterface.h"
#include "cbenchanted.h"
#include "cbimage.h"

		
ImageInterface::ImageInterface():cb(static_cast<CBEnchanted*>(this)) {
	
}

ImageInterface::~ImageInterface() {
	
}

void ImageInterface::commandSaveImage(void) {
	/*string path = cb->popValue().toString();
	int32_t id = cb->popValue().toInt();*/
	STUB;

}

void ImageInterface::commandDrawImage(void) {
	cb->getCurrentRenderTarget()->setViewTo(cb->getDrawImageToWorld());
	bool mask = cb->popValue().getInt();
	int32_t frame = cb->popValue().toInt();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->draw(x,y);
}

void ImageInterface::commandDrawGhostImage(void) {
	STUB;
}

void ImageInterface::commandDrawImageBox(void) {
	STUB;
}

void ImageInterface::commandMaskImage(void) {
	STUB;
}

void ImageInterface::commandDefaultMask(void) {
	STUB;
}

void ImageInterface::commandResizeImage(void) {
	STUB;
}

void ImageInterface::commandRotateImage(void) {
	STUB;
}

void ImageInterface::commandPickImageColor(void) {
	STUB;
}

void ImageInterface::commandPickImageColor2(void) {
	STUB;
}

void ImageInterface::commandHotSpot(void) {
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	cbImages[id]->setHotspot(x,y);
}

void ImageInterface::commandDeleteImage(void) {
	int32_t id = cb->popValue().getInt();
	delete cbImages[id];
	cbImages.erase(id);
}

void ImageInterface::functionLoadImage(void) {
	string path = cb->popValue().getString();
	CBImage *image = new CBImage;
	if (!image->load(path))
	{
		delete image;
		FIXME("Loading image %s failed.",path.c_str());
		cb->pushValue(0);
	}
	int32_t id = nextId();
	cbImages[id] = image;
	cb->pushValue(id);
}

void ImageInterface::functionLoadAnimImage(void) {
	STUB;
}

void ImageInterface::functionMakeImage(void) {
	int32_t h = cb->popValue().toInt();
	int32_t w = cb->popValue().toInt();
	CBImage *image = new CBImage;
	image->makeImage(w,h);
	int32_t id = nextId();
	cbImages[id] = image;
	cb->pushValue(id);
}

void ImageInterface::functionCloneImage(void) {
	STUB;
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
