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
	int32_t mask = cb->popValue().toInt();
	int32_t frame = cb->popValue().toInt();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->draw(x,y,frame,mask);
}

void ImageInterface::commandDrawGhostImage(void) {
	//int32_t id = cb->popValue().getInt();
	//CBImage *img = cbImages[cb->popValue().getInt()];
	STUB;
}

void ImageInterface::commandDrawImageBox(void) {
	STUB;
}

void ImageInterface::commandMaskImage(void) {
	sf::Color mask;
	mask.r = cb->popValue().toByte();
	mask.g = cb->popValue().toByte();
	mask.b = cb->popValue().toByte();
	mask.a = 255;
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->maskImage(mask);
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
	int32_t animL = cb->popValue().toInt();
	int32_t startF = cb->popValue().toInt();
	int32_t frameH = cb->popValue().toInt();
	int32_t frameW = cb->popValue().toInt();
	string path = cb->popValue().getString();
	CBImage *image = new CBImage;
	if (!image->load(path))
	{
		delete image;
		FIXME("Loading image %s failed.",path.c_str());
		cb->pushValue(0);
	}
	image->setAnimParams(frameW,frameH,startF,animL);
	int32_t id = nextId();
	cbImages[id] = image;
	cb->pushValue(id);
}

void ImageInterface::functionMakeImage(void) {
	int32_t frameCount = cb->popValue().toInt();
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
