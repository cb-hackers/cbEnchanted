#include "precomp.h"
#include "imageinterface.h"
#include "cbenchanted.h"
#include "cbimage.h"


ImageInterface::ImageInterface() {
	cb = static_cast <CBEnchanted *> (this);
}

ImageInterface::~ImageInterface() {

}

void ImageInterface::commandSaveImage(void) {
	/*string path = cb->popValue().toString();
	int32_t id = cb->popValue().toInt();*/
	STUB;

}

void ImageInterface::commandDrawImage(void) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawImageToWorld());
	int32_t mask = cb->popValue().toInt();
	int32_t frame = cb->popValue().toInt();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->draw(*cb->getCurrentRenderTarget(),x,y,frame,mask);
}

void ImageInterface::commandDrawGhostImage(void) {
	//int32_t id = cb->popValue().getInt();
	//CBImage *img = cbImages[cb->popValue().getInt()];
	STUB;
}

void ImageInterface::commandDrawImageBox(void) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawImageToWorld());
	int32_t mask = cb->popValue().toInt();
	int32_t frame = cb->popValue().toInt();
	float h = cb->popValue().toFloat();
	float w = cb->popValue().toFloat();
	float sy = cb->popValue().toFloat();
	float sx = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->draw(*cb->getCurrentRenderTarget(),x,y,frame,mask);
}

void ImageInterface::commandMaskImage(void) {

	float b = cb->popValue().toFloat()/255.0f;
	float g= cb->popValue().toFloat()/255.0f;
	float r = cb->popValue().toFloat()/255.0f;
	CBImage *img = cbImages[cb->popValue().getInt()];
	img->maskImage(al_map_rgb_f(r,g,b));
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
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	int32_t imgId = cb->popValue().toInt();
	CBImage *image = getImage(imgId);
	cb->setDrawColor(image->getRenderTarget()->getPixel(x,y));
}

void ImageInterface::commandPickImageColor2(void) {
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	int32_t imgId = cb->popValue().toInt();
	CBImage *image = getImage(imgId);
	cb->setDrawColor(image->getRenderTarget()->getPixel(x,y));
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
	string path = cb->popValue().getString().getRef();
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
	string path = cb->popValue().getString().getRef();
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
