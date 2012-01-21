#ifndef OBJECT_H
#define OBJECT_H
#include "cbenchanted.h"
#include "precomp.h"

class CBObject{
	public:
		CBObject();
		bool load(string file);
		bool loadAnimObject(string file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount);
		void paintObject(sf::Texture txt);
		void paintObject(CBObject &obj);
		void moveObject(float fwrd, float sdwrd);
		void positionObject(float x, float y);
		void translateObject(float hor, float ver, float depth);
		void turnObject(float speed);
		void render(sf::RenderTarget &target);
		void setPosition(float x, float y);
		void ghostObject(uint8_t ab);
		void maskObject(uint8_t r, uint8_t g, uint8_t b);
		float getX();
		float getY();
		float getAngle(); //Funktion nimi on nyt v‰h‰n harhaanjohtava, mutta se nyt saa luvan kelvata koska se on luokan sis‰ll‰.

	private:

		float posX, posY;
		float angle;
		sf::Image imgtex; //Kuva tekstuurille. V‰rimaskin takia pit‰‰ tehd‰ n‰in vitun vaikeasti. -.-
		sf::Texture texture; //Tekstuurin "v‰litt‰j‰"
		sf::Sprite sprite; //Ja itse piirt‰j‰.
		uint8_t alpha;
		uint16_t frameWidth;
		uint16_t frameHeight;
		uint16_t startframe;
		uint16_t maxframes;
		uint16_t currentframe;
};

#endif // OBJECT_H
