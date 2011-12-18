#ifndef ANIMINTERFACE_H
#define ANIMINTERFACE_H

class AnimInterface {
	public:
		AnimInterface();
		~AnimInterface();
		
		void commandStopAnimation(void);
		void commandDrawAnimation(void);
		
		void functionPlayAnimation(void);
		void functionAnimationWidth(void);
		void functionAnimationHeight(void);
		void functionAnimationPlaying(void);
	private:
	
};

#endif
