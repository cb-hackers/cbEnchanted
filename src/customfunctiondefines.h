#ifndef CUSTOMFUNCTIONDEFINES_H
#define CUSTOMFUNCTIONDEFINES_H

/** @addtogroup customfunctions
 * @{
 */

/** Group ID of custom functions of cbEnchanted itself */
#define CBE_CUSTOM_FUNCTION_GROUP 1

/** ID for custom function cbeTriangle() */
#define CBE_CUSTOM_DRAWTRIANGLE 1

/** ID for custom function cbeColor() */
#define CBE_CUSTOM_COLOR 2

/** ID for custom function cbeClsColor() */
#define CBE_CUSTOM_CLSCOLOR 3

/** ID for custom function cbeSetBlendMode() */
#define CBE_CUSTOM_SET_BLEND_MODE 4

/** ID for custom function cbeSetBlendModeAdvanced() */
#define CBE_CUSTOM_SET_BLEND_MODE_ADVANCED 5

/** ID for custom function cbeDrawTintedImage() */
#define CBE_DRAW_TINTED_IMAGE 6

/** ID for custom function cbeDrawImage() */
#define CBE_DRAW_IMAGE 29

/** IDs for binary operations */
#define CBE_BINARY_NOT 7
#define CBE_BINARY_AND 8
#define CBE_BINARY_OR 9
#define CBE_BINARY_XOR 10

/** ID for custom function cbeReadLineUTF8() */
#define CBE_READ_LINE_UTF8 11

/** ID for custom function cbeShowConsole() */
#define CBE_SHOW_CONSOLE 12

/** ID for custom function cbeSetSystemCursor() */
#define CBE_SET_SYSTEM_CURSOR 13

/** ID for custom function cbeTransformBuild() */
#define CBE_TRANSFORM_BUILD 14

/** ID for custom function cbeTransformRotate() */
#define CBE_TRANSFORM_ROTATE 15

/** ID for custom function cbeTransformScale() */
#define CBE_TRANSFORM_SCALE 16

/** ID for custom function cbeTransformTranslate() */
#define CBE_TRANSFORM_TRANSLATE 17

/** ID for custom function cbeTransformReset() */
#define CBE_TRANSFORM_RESET 18

/** ID for custom function cbeCameraAngle */
#define CBE_CAMERA_ANGLE 19

/** ID for custom function cbeCameraZoom */
#define CBE_CAMERA_ZOOM 20

/** ID for custom function cbeLineWidth */
#define CBE_SET_LINE_WIDTH 21

/** ID for custom function cbeFileDialog */
#define CBE_FILEDIALOG 22

/** ID for custom function cbeSToWX */
#define CBE_S_TO_W_X 23

/** ID for custom function cbeSToWY */
#define CBE_S_TO_W_Y 24

/** ID for custom function cbeWToSX */
#define CBE_W_TO_S_X 25

/** ID for custom function cbeWToSY */
#define CBE_W_TO_S_Y 26

/** ID for custom function cbeObjectSX */
#define CBE_OBJECT_S_X 27

/** ID for custom function cbeObjectSY */
#define CBE_OBJECT_S_Y 28

// NOTE: ID 29 is defined earlier as CBE_DRAW_IMAGE


/** @} */

#define CBE_STACK 100
#define CBE_PUSH_BYTE 1
#define CBE_PUSH_SHORT 2
#define CBE_PUSH_INTEGER 3
#define CBE_PUSH_FLOAT 4
#define CBE_PUSH_STRING 5
#define CBE_POP_BYTE 6
#define CBE_POP_SHORT 7
#define CBE_POP_INTEGER 8
#define CBE_POP_FLOAT 9
#define CBE_POP_STRING 10

#endif // CUSTOMFUNCTIONDEFINES_H
