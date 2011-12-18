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
	private:
	
};

#endif
