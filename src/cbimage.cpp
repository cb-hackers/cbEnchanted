#include "cbimage.h"
#include "cbenchanted.h"

bool CBImage::useARBShaders;
GLuint CBImage::maskShaderProgram;
GLuint CBImage::maskColorUniformPos;


CBImage::CBImage() :
	hotspotX(0),
	hotspotY(0),
	renderTarget(),
	frameWidth(0),
	frameHeight(0),
	animBegin(0),
	animLength(0)
{
	maskColor = al_map_rgb_f(0, 0, 0);
}

CBImage::~CBImage() {

}

bool CBImage::load(const string &path) {
	return renderTarget.load(path);
}

void CBImage::draw(RenderTarget &r,float x, float y, bool useMask) {
	if (useMask && maskShaderProgram) {
		if (useARBShaders) {
			glUseProgramObjectARB(maskShaderProgram);
			glUniform4fARB(maskColorUniformPos,maskColor.r,maskColor.g,maskColor.b,maskColor.a);
		}
		else {
			glUseProgram(maskShaderProgram);
			glUniform4f(maskColorUniformPos,maskColor.r,maskColor.g,maskColor.b,maskColor.a);
		}
		r.drawBitmap(renderTarget.getBitmap(), x - hotspotX, y - hotspotY);
		if (useARBShaders) {
			glUseProgramObjectARB(0);
		}
		else {
			glUseProgram(0);
		}
		return;
	}
	r.drawBitmap(renderTarget.getBitmap(), x - hotspotX, y - hotspotY);
}

void CBImage::draw(RenderTarget &r, float x, float y, int frame, bool useMask)
{
	if (animLength == 0) { //Not anim image
		draw(r,x,y,useMask);
		return;
	}
	//INFO("%i", frame)
	int32_t framesX = renderTarget.width() / frameWidth;
	int32_t framesY = renderTarget.height() / frameHeight;
	int32_t copyX = frame % framesX;
	int32_t copyY = (frame - copyX) / framesY;

	float frameAreaLeft = (copyX * frameWidth);
	float frameAreaTop = (copyY * frameWidth);
	float frameAreaHeight = frameHeight;
	float frameAreaWidth = frameWidth;
	if (useMask) {
		//TODO MASKING
		r.drawBitmapRegion(
			renderTarget.getBitmap(),
			frameAreaLeft,
			frameAreaTop,
			frameAreaWidth,
			frameAreaHeight,
			x - hotspotX,
			y - hotspotY
		);
	}
	else {
		r.drawBitmapRegion(
			renderTarget.getBitmap(),
			frameAreaLeft,
			frameAreaTop,
			frameAreaWidth,
			frameAreaHeight,
			x - hotspotX,
			y - hotspotY
		);
	}
}

void CBImage::drawBox(RenderTarget &r, float sx, float sy, float sw, float sh, float tx, float ty, bool useMask) {
	if (useMask && maskShaderProgram) {
		if (useARBShaders) {
			glUseProgramObjectARB(maskShaderProgram);
			glUniform4fARB(maskColorUniformPos,maskColor.r,maskColor.g,maskColor.b,maskColor.a);
		}
		else {
			glUseProgram(maskShaderProgram);
			glUniform4f(maskColorUniformPos,maskColor.r,maskColor.g,maskColor.b,maskColor.a);
		}
		r.drawBitmapRegion(renderTarget.getBitmap(),sx,sy,sw,sh,tx,ty);
		if (useARBShaders) {
			glUseProgramObjectARB(0);
		}
		else {
			glUseProgram(0);
		}
		return;
	}
	r.drawBitmapRegion(renderTarget.getBitmap(),sx,sy,sw,sh,tx,ty);
}

void CBImage::drawBox(RenderTarget &r, float sx, float sy, float sw, float sh, float tx, float ty, int frame, bool useMask) {

	if (animLength == 0) { //Not anim image
		drawBox(r,sx,sy,sw,sh,tx,ty,useMask);
		return;
	}
	//INFO("%i", frame)
	int32_t framesX = renderTarget.width() / frameWidth;
	int32_t framesY = renderTarget.height() / frameHeight;
	int32_t copyX = frame % framesX;
	int32_t copyY = (frame - copyX) / framesY;

	float frameAreaLeft = (copyX * frameWidth)+sx;
	float frameAreaTop = (copyY * frameWidth)+sy;
	drawBox(r,frameAreaLeft,frameAreaTop,sw,sh,tx,ty,useMask);
}

void CBImage::makeImage(int32_t w, int32_t h) {
	renderTarget.create(w, h);
	renderTarget.clear(al_map_rgb_f(0, 0, 0));
}

const char * imageMaskFragmentShaderCode =
		"uniform sampler2D texture;\n"
		"uniform vec4 maskColor;\n"
		"const float offset = 0.00196078431372549019607843137255;\n"
		"void main(void)\n"
		"{\n"
		"    vec4 color = texture2D(texture,gl_TexCoord[0].st);"
		"    vec4 div = abs(maskColor - color);\n"
		"    if ((div.r <= offset) && (div.g <= offset) && (div.b <= offset) && (div.a <= offset)) color = vec4(0.0);\n"
		"    gl_FragColor = color;"
		"}\n"
		"";

void CBImage::initMaskShader() {
	maskShaderProgram = 0;
	if (al_get_opengl_version() > 0x02000000) {
		useARBShaders = false;
		GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader,1,&imageMaskFragmentShaderCode,0);
		glCompileShader(shader);
		if (glGetError() != GL_NO_ERROR) {
			INFO("Compiling image mask shader failed");
			glDeleteShader(shader);
			return;
		}

		maskShaderProgram = glCreateProgram();
		glAttachShader(maskShaderProgram, shader);
		glLinkProgram(maskShaderProgram);
		if (glGetError() != GL_NO_ERROR) {
			INFO("Linking image mask shader failed");
			glDeleteProgram(maskShaderProgram);
			glDeleteShader(shader);
			maskShaderProgram = 0;
			return;
		}
		maskColorUniformPos = glGetUniformLocationARB(maskShaderProgram,"maskColor");
	}
	else if (al_get_opengl_extension_list()->ALLEGRO_GL_ARB_fragment_shader) {
		useARBShaders = true;
		GLhandleARB shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		glShaderSourceARB(shader,1,&imageMaskFragmentShaderCode,0);
		glCompileShaderARB(shader);
		if (glGetError()) {
			INFO("Compiling image mask shader failed");
			glDeleteObjectARB(shader);
			return;
		}

		maskShaderProgram = glCreateProgramObjectARB();
		glAttachObjectARB(maskShaderProgram,shader);
		glLinkProgram(maskShaderProgram);
		if (glGetError() != GL_NO_ERROR) {
			INFO("Linking image mask shader failed");
			glDeleteProgramsARB(1,&maskShaderProgram);
			glDeleteObjectARB(shader);
			maskShaderProgram = 0;
			return;
		}
		maskColorUniformPos = glGetUniformLocation(maskShaderProgram,"maskColor");
	}
}
