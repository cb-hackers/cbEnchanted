#include "customfunctions.h"
#include "cbenchanted.h"
/** @addtogroup customfunctions
  * @{ */

/** @brief VesQ's awesome triangle drawing function
  */
void commandTriangle(CBEnchanted *cb) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawDrawCommandToWorld());
	bool fill = cb->popValue().toInt();
	float thickness = cb->popValue().toFloat();
	float y3 = cb->popValue().toFloat();
	float x3 = cb->popValue().toFloat();
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();
	cb->getCurrentRenderTarget()->drawTriangle(
		x1,
		y1,
		x2,
		y2,
		x3,
		y3,
		thickness,
		fill,
		cb->getDrawColor()
	);
	cb->pushValue(0);
}



/** @} */
