#include "precomp.h"
#include "imageinterface.h"
#include "cbenchanted.h"
#include "cbimage.h"

		
ImageInterface::ImageInterface():cb(static_cast<CBEnchanted*>(this)) {
	
}

ImageInterface::~ImageInterface() {
	
}

void ImageInterface::commandSaveImage(void) {
	
}

void ImageInterface::commandDrawImage(void) {
    bool mask = cb->popValue().getInt();
    int32_t frame = cb->popValue().toInt();
    float y = cb->popValue().toFloat();
    float x = cb->popValue().toFloat();
    CBImage *img = cbImages[cb->popValue().getInt()];
    img->SetPosition(x,y);
    cb->getCurrentRenderTarget()->draw(*img);
}

void ImageInterface::commandDrawGhostImage(void) {
	
}

void ImageInterface::commandDrawImageBox(void) {
	
}

void ImageInterface::commandMaskImage(void) {
	
}

void ImageInterface::commandDefaultMask(void) {
	
}

void ImageInterface::commandResizeImage(void) {
	
}

void ImageInterface::commandRotateImage(void) {
	
}

void ImageInterface::commandPickImageColor(void) {
	
}

void ImageInterface::commandPickImageColor2(void) {
	
}

void ImageInterface::commandHotSpot(void) {
	
}

void ImageInterface::commandDeleteImage(void) {
	
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
	
}

void ImageInterface::functionMakeImage(void) {
	
}

void ImageInterface::functionCloneImage(void) {
	
}

void ImageInterface::functionImageWidth(void) {

}

void ImageInterface::functionImageHeight(void) {
	
}

void ImageInterface::functionImagesOverlap(void) {
	
}

void ImageInterface::functionImagesCollide(void) {
	
}
int32_t ImageInterface::nextId()
{
    return ++idCounter;
}
