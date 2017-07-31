#ifndef ANIMINTERFACE_H
#define ANIMINTERFACE_H



struct ALLEGRO_VIDEO;
class CBEnchanted;

class AnimInterface {
	//Note(everyone): From now on (31.7.2017) we should probably start using some member convention. 
#ifndef CBE_LIB
	ALLEGRO_VIDEO* mVideo = nullptr; 
	CBEnchanted* mCBInstance = nullptr; 
	ALLEGRO_BITMAP* mCurrentFrame = nullptr;
	int mVideoWidth = 0, mVideoHeight = 0;
	float mScaleX = 1.0f, mScaleY = 1.0f; 
#endif 
	public:
#ifndef CBE_LIB
		AnimInterface();
		~AnimInterface();

		bool initializeAnimInterface();

		void commandStopAnimation(void);
		void commandDrawAnimation(void);

		void functionPlayAnimation(void);
		void functionAnimationWidth(void);
		void functionAnimationHeight(void);
		void functionAnimationPlaying(void);
		void acquireNewFrame(); 
#endif
};

#endif
