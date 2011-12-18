#ifndef IMAGEINTERFACE_H
#define IMAGEINTERFACE_H

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
	private:
	
};

#endif
