#include "precomp.h"
#include "cbenchanted.h"
#include "soundinterface.h"
#include "variablestack.h"
#include "animinterface.h"
#include "debug.h"
#include "errorsystem.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>


#ifndef CBE_LIB
AnimInterface::AnimInterface() {
	mCBInstance = CBEnchanted::instance();
	
}

AnimInterface::~AnimInterface() {

}

bool AnimInterface::initializeAnimInterface() {
	return al_init_video_addon();
}

void AnimInterface::commandStopAnimation(void) {
	if(mVideo) al_close_video(mVideo);
	mVideo = nullptr;
}

void AnimInterface::commandDrawAnimation(void) {
	al_draw_scaled_bitmap(mCurrentFrame, 0, 0, mVideoWidth, mVideoHeight, 0, 0, mScaleX, mScaleY, 0);
}

void AnimInterface::functionPlayAnimation(void) {
	ISString input = mCBInstance->popValue().toString();
	if(input.empty()) {
		mCBInstance->errors->createError("Cannot play animation! Given path is empty!");
	}

	if(mVideo) al_close_video(mVideo);

	std::string path = input.getStdString();
	mVideo = al_open_video(path.c_str());
	if(mVideo == nullptr) {
		mCBInstance->pushValue((int)0);
		return;
	}
	
	al_register_event_source(mCBInstance->getEventQueue(), al_get_video_event_source(mVideo));
	al_start_video(mVideo, mCBInstance->soundInterface->getMixer());
}

void AnimInterface::functionAnimationWidth(void) {
	if(mVideo) {
		mCBInstance->pushValue((int)((float)mVideoWidth * mScaleX));
		return;
	}
	mCBInstance->errors->createFatalError("There isn't video playing!");
	mCBInstance->pushValue((int)0);
}

void AnimInterface::functionAnimationHeight(void) {
	if(mVideo) {
		mCBInstance->pushValue((int)((float)mVideoWidth * mScaleX));
		return;
	}
	mCBInstance->errors->createFatalError("There isn't video playing!");
	mCBInstance->pushValue((int)0);
}

void AnimInterface::functionAnimationPlaying(void) {
	mCBInstance->pushValue((int)(mVideo != nullptr));
}

void AnimInterface::acquireNewFrame() {
	mCurrentFrame = al_get_video_frame(mVideo);
	mScaleX = al_get_video_scaled_width(mVideo);
	mScaleY = al_get_video_scaled_height(mVideo);
	mVideoWidth = 1.0f * al_get_bitmap_width(mCurrentFrame);
	mVideoHeight = 1.0f * al_get_bitmap_height(mCurrentFrame);
}

#endif
