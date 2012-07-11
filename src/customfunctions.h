#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H
class CBEnchanted;

/** @addtogroup customfunctions
 * @{
 */

void cbeTriangle(CBEnchanted *cb);

void cbeColor(CBEnchanted *cb);

void cbeClsColor(CBEnchanted *cb);

void cbeSetBlendMode(CBEnchanted *cb);

void cbeSetBlendModeAdvanced(CBEnchanted *cb);

void cbeDrawTintedScaledRotatedImageRegion(CBEnchanted *cb);

void cbeBAnd(CBEnchanted *cb);

void cbeBOr(CBEnchanted *cb);

void cbeBXor(CBEnchanted *cb);

void cbeBNot(CBEnchanted *cb);

void cbeReadLineUTF8(CBEnchanted *cb);

void cbeShowConsole(CBEnchanted *cb);

void cbeSetSystemCursor(CBEnchanted *cb);

void cbeTransformBuild(CBEnchanted *cb);

void cbeTransformRotate(CBEnchanted *cb);

void cbeTransformScale(CBEnchanted *cb);

void cbeTransformTranslate(CBEnchanted *cb);

void cbeTransformReset(CBEnchanted *cb);

void cbeCameraZoom(CBEnchanted *cb);

void cbeCameraAngle(CBEnchanted *cb);

void cbeSetLineWidth(CBEnchanted *cb);

void cbeFileDialog(CBEnchanted *cb);

void cbeWToSX(CBEnchanted *cb);

void cbeWToSY(CBEnchanted *cb);

void cbeSToWX(CBEnchanted *cb);

void cbeSToWY(CBEnchanted *cb);

void cbeObjectSX(CBEnchanted *cb);

void cbeObjectSY(CBEnchanted *cb);

void cbeLinkCustomFunctions(CBEnchanted *cb);

void cbeLoadLibrary(CBEnchanted *cb);

/** @} */

#endif // CUSTOMFUNCTIONS_H
