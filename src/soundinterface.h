#ifndef SOUNDINTERFACE_H
#define SOUNDINTERFACE_H

class SoundInterface {
	public:
		SoundInterface();
		~SoundInterface();
		
		void commandPlaySound(void);
		void commandSetSound(void);
		void commandStopSound(void);
		void commandDeleteSound(void);
		
		void functionLoadSound(void);
		void functionPlaySound(void);
		void functionSoundPlaying(void);
	private:
	
};

#endif
