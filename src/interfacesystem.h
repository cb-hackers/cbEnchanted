#ifndef INTERFACESYSTEM_H
#define INTERFACESYSTEM_H
#include "mathinterface.h"
#include "stringinterface.h"
#include "textinterface.h"
#include "fileinterface.h"
#include "meminterface.h"
#include "inputinterface.h"
#include "sysinterface.h"
#include "soundinterface.h"
#include "animinterface.h"
#include "imageinterface.h"
#include "gfxinterface.h"
#include "objectinterface.h"
#include "camerainterface.h"
#include "mapinterface.h"
#include "effectinterface.h"

class InterfaceSystem:
		public MathInterface,
		public GfxInterface,
		public StringInterface,
		public TextInterface,
		public InputInterface,
		public SysInterface,
		public ObjectInterface,
		public MemInterface,
		public FileInterface,
		public MapInterface,
		public CameraInterface,
		public ImageInterface,
		public EffectInterface,
		public AnimInterface,
		public SoundInterface
{
	public:
		InterfaceSystem();
		bool init();
		static InterfaceSystem *instance();
};

#endif // INTERFACESYSTEM_H
