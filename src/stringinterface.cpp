#include "precomp.h"
#include "cbemu.h"
#include "stringinterface.h"

StringInterface::StringInterface() : cb(static_cast <CBEmu *> (this)) {
	
}
		
void StringInterface::functionStr(void) {
	cb->pushValue(lexical_cast<string>(cb->popValue<int32_t>()));
}
