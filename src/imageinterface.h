#ifndef IMAGEINTERFACE_H
#define IMAGEINTERFACE_H
#include "precomp.h"
class CBEnchanted;
class CBImage;
class ImageInterface {
	public:
#ifndef CBE_LIB
		ImageInterface();
		~ImageInterface();
		void initializeImages();

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
#endif

		CBImage *getImage(int32_t id);

		bool isDefaultMaskToggled() const { return defaultMaskToggled; }
	private:
		CBEnchanted *cb;

		/** Holds all loaded images in a map. */
		map <int32_t, CBImage*> cbImages;

		/** Returns new id for cbImages. Images IDs start from 2. */
		int32_t nextId() {static int32_t idCounter = 1; return ++idCounter;}

		/** Default mask color */
		ALLEGRO_COLOR defaultMask;

		/** Is default mask on */
		bool defaultMaskToggled;

		/** Default hotspot coordinates, if < 0, hotspot is made to be at the center of image. */
		int32_t defaultHotspotX, defaultHotspotY;

		/** Should future loaded/generated images have a hotspot set automatically. */
		bool defaultHotspotToggled;
};

#endif
