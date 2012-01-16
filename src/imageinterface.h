#ifndef IMAGEINTERFACE_H
#define IMAGEINTERFACE_H
#include "precomp.h"
class CBEnchanted;
class CBImage;
class ImageInterface {
	public:
		ImageInterface();
		~ImageInterface();
		
		void commandSaveImage(void);
		void commandDrawImage(void);
		void commandDrawGhostImage(void);
		void commandDrawImageBox(void);
		void commandMaskImage(void);
		void commandDefaultMask(void);
		void commandResizeImage(void);
		void commandRotateImage(void);
		void commandPickImageColor(void);
		void commandPickImageColor2(void);
		void commandHotSpot(void);
		void commandDeleteImage(void);
		
		void functionLoadImage(void);
		void functionLoadAnimImage(void);
		void functionMakeImage(void);
		void functionCloneImage(void);
		void functionImageWidth(void);
		void functionImageHeight(void);
		void functionImagesOverlap(void);
		void functionImagesCollide(void);

        CBImage *getImage(int32_t id){return cbImages[id];}
	private:
        CBEnchanted *cb;
        int32_t idCounter;
        int32_t nextId();
        map<int32_t,CBImage*> cbImages;
};

#endif
