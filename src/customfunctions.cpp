#include "customfunctions.h"
#include "cbenchanted.h"

/** @addtogroup customfunctions
 * @{
 */

/** VesQ's awesome triangle drawing function */
void cbETriangle(CBEnchanted *cb) {
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

/** Sets draw color with alpha */
void cbEColor(CBEnchanted *cb) {
	int32_t a = cb->popValue().toInt();
	int32_t b= cb->popValue().toInt();
	int32_t g = cb->popValue().toInt();
	int32_t r = cb->popValue().toInt();
	cb->setDrawColor(al_map_rgba(r,g,b,a));
	cb->pushValue(0);
}

/** Sets clear color with alpha */
void cbEClsColor(CBEnchanted *cb) {
	int32_t a = cb->popValue().toInt();
	int32_t b= cb->popValue().toInt();
	int32_t g = cb->popValue().toInt();
	int32_t r = cb->popValue().toInt();
	cb->setClearColor(al_map_rgba(r,g,b,a));
	cb->pushValue(0);
}

/** @} */
