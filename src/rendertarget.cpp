#include "rendertarget.h"
#include "cbenchanted.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#define CIRCLE_VERTEX_COUNT 100
#ifndef GL_COMBINE
	#define GL_COMBINE 0x8570
#endif
sf::Shader *imageMaskShader;

RenderTarget *currentRenderContext = 0;
inline void setCurrentRenderContext(RenderTarget *t) {
	if (currentRenderContext == t) return;
	if (currentRenderContext) currentRenderContext->display();
	currentRenderContext = t;
	currentRenderContext->getSurface()->SetActive();
}

static int32_t renderTargetIdCounter = 1;
RenderTarget::RenderTarget():defaultRenderState(sf::BlendAlpha),lockBuffer(0) {
	drawToWorldViewOn = false;
	id = renderTargetIdCounter++;
}

RenderTarget::~RenderTarget() {
	if (lockBuffer) delete lockBuffer;
}

void RenderTarget::create(int w, int h) {
	if (!target.Create(w, h)) {
		FIXME("Creating RenderTarget failed. Can't create RenderTexture.");
		return;
	}
	setCurrentRenderContext(this);
	target.Clear();
	setup();
	changedSinceDisplay = false;
}

void RenderTarget::create(const sf::Texture &texture) {
	if (!target.Create(texture.GetWidth(), texture.GetHeight())) {
		FIXME("Creating RenderTarget failed. Can't create RenderTexture.");
		return;
	}
	setCurrentRenderContext(this);
	target.Clear(sf::Color(0, 0, 0, 0));
	setup();
	drawTexture(texture, 0, 0);
	display();
	changedSinceDisplay = false;
}

void RenderTarget::setup(){
	target.SetActive(true);
	target.ResetGLStates();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //Disabling combining of glColor and texture color.

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, target.GetWidth(), target.GetHeight());
	glLoadMatrixf(target.GetView().GetTransform().GetMatrix());
	glMatrixMode(GL_MODELVIEW);
	glColor4ub(CBEnchanted::instance()->getDrawColor().r, CBEnchanted::instance()->getDrawColor().g, CBEnchanted::instance()->getDrawColor().b, CBEnchanted::instance()->getDrawColor().a);
	glLoadIdentity();
	drawingMode = DM_NoTextures;
}

void RenderTarget::setDrawingMode(RenderTarget::DrawingMode mode) {
	setCurrentRenderContext(this);
	if (drawingMode == mode) return;
	switch(mode) {
		case DM_Textures:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE); //Enable combining of glColor and texture color.
			glEnable(GL_TEXTURE_2D);
			break;
		case DM_NoTextures:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //Disabling combining of glColor and texture color.
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;
	}
}

void RenderTarget::setViewTo(bool drawtoworld, bool force) {
    if (drawToWorldViewOn == drawtoworld && !force) return; //Useless to update view
	if (drawtoworld) {
		sf::View view;
		view.SetSize(target.GetWidth(), target.GetHeight());
		view.SetCenter(CBEnchanted::instance()->getCameraX(), -CBEnchanted::instance()->getCameraY());
		target.SetView(view);
		drawToWorldViewOn = true;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(target.GetView().GetTransform().GetMatrix());
		glMatrixMode(GL_MODELVIEW);
		glScalef(1.0,-1.0,1.0);
	}
	else {
		target.SetView(target.GetDefaultView());
		drawToWorldViewOn = false;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(target.GetView().GetTransform().GetMatrix());
		glMatrixMode(GL_MODELVIEW);
	}
}

void RenderTarget::draw(const sf::Drawable &d) {
	setDrawingMode(DM_Textures);
	target.Draw(d);
	changedSinceDisplay = true;
}

void RenderTarget::draw(const sf::Vertex *vertices, unsigned int vertexCount, sf::PrimitiveType type, const sf::RenderStates &states) {
	setDrawingMode(DM_Textures);
	target.Draw(vertices, vertexCount, type, states);
	changedSinceDisplay = true;
}

void RenderTarget::clear(const sf::Color &c) {
	target.Clear(c);
	changedSinceDisplay = true;
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

void RenderTarget::init()
{
	imageMaskShader = new sf::Shader;
	bool shaderLoadingFailed =imageMaskShader->LoadFromMemory(string(imageMaskFragmentShaderCode),sf::Shader::Fragment);
	assert(shaderLoadingFailed);
}

void RenderTarget::display() const {
	if (changedSinceDisplay) {
		RenderTarget *t = const_cast<RenderTarget*>(this);
		t->target.Display();
		t->changedSinceDisplay = false;
	}
}

void RenderTarget::drawLine(float x1, float y1, float x2, float y2) {
	setDrawingMode(DM_NoTextures);
	/*sf::Vertex vertices[2];
	vertices[0].Position.x = x1;
	vertices[0].Position.y = y1;
	vertices[0].Color = color;
	vertices[1].Position.x = x2;
	vertices[1].Position.y = y2;
	vertices[1].Color = color;
	target.Draw(vertices, 2, sf::Lines);*/

	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
	changedSinceDisplay = true;
}

void RenderTarget::drawCircle(float cx, float cy, float r, bool fill) {
	setDrawingMode(DM_NoTextures);
	/*changedSinceDisplay = true;
	glTranslatef(cx, cy, 0);
	glScalef(r, r, 1.0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, circleVertexArray);
	if (fill) {
		glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_VERTEX_COUNT + 2);
	}
	else {
		glDrawArrays(GL_LINE_LOOP, 1, CIRCLE_VERTEX_COUNT);
	}
	glLoadIdentity();
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);*/
	int vertexCount = 3 + r;
	glTranslatef(cx, cy, 0);
	glScalef(r, r, 1.0);
	if (fill) {
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0, 0);
	}
	else
		glBegin(GL_LINE_LOOP);

	float theta = 2 * 3.1415926 / float(vertexCount);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;
	float x = 1.0f;
	float y = 0;

	for(int i = 0; i < vertexCount; i++) {
		glVertex2f(x, y);
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	if (fill)
	{
		glVertex2f(x, y);
	}
	glEnd();
	glLoadIdentity();
	if (drawToWorldViewOn) glScalef(1.0,-1.0,1.0);
	changedSinceDisplay = true;
}

void RenderTarget::drawBox(float x, float y, float w, float h, bool fill) {
	setDrawingMode(DM_NoTextures);
	if (drawToWorldViewOn) h = -h;
	if (fill)
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

void RenderTarget::drawEllipse(float cx, float cy, float w, float h, bool fill) {
	setDrawingMode(DM_NoTextures);
	int vertexCount = 3 + (w + h) / 2;
	glTranslatef(cx, cy, 0);
	glScalef(w, h, 1.0f);
	if (fill) {
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0, 0);
	}
	else
		glBegin(GL_LINE_LOOP);

	float theta = 2 * 3.1415926 / float(vertexCount);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;
	float x = 1.0f;
	float y = 0;

	for(int i = 0; i < vertexCount; i++) {
		glVertex2f(x, y);
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	if (fill)
	{
		glVertex2f(x, y);
	}
	glEnd();
	glLoadIdentity();
	if (drawToWorldViewOn) glScalef(1.0,-1.0,1.0);
	changedSinceDisplay = true;
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glTranslatef(cx, cy, 0);
	glScalef(w, h, 1.0f);
	glVertexPointer(2, GL_FLOAT, 0, circleVertexArray);
	if (fill)
		glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_VERTEX_COUNT);
	else
		glDrawArrays(GL_LINE_LOOP, 1, CIRCLE_VERTEX_COUNT);
	glDisableClientState(GL_VERTEX_ARRAY);*/

}

void RenderTarget::drawDot(float x, float y) {
	setDrawingMode(DM_NoTextures);
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();
	changedSinceDisplay = true;
}

void RenderTarget::drawParticles(CBImage *tex, const vector<Particle> &particles, int32_t particleLifeTime)
{

}

void RenderTarget::drawRenderTarget(const RenderTarget &rt, float x, float y) {
	/*enableSFMLDrawMode();
	sf::Sprite sprite(rt.getSurface()->GetTexture());

	sprite.SetPosition(x, y);
	target.Draw(sprite, defaultRenderState);*/
	setDrawingMode(DM_Textures);
	rt.display();
	int h = rt.height();
	if (drawToWorldViewOn) h = -h;
	int w = rt.width();
	rt.getSurface()->GetTexture().Bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x, y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x + w, y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x + w, y + h);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x, y + h);
	glEnd();
	changedSinceDisplay = true;
}

void RenderTarget::drawRenderTarget(const RenderTarget &rt, float x, float y, const sf::Color &mask)
{
	setDrawingMode(DM_Textures);
	rt.display();
	int h = rt.height();
	if (drawToWorldViewOn) h = -h;
	int w = rt.width();
	imageMaskShader->Bind();
	imageMaskShader->SetParameter("maskColor",mask);
	rt.getSurface()->GetTexture().Bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x, y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x + w, y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x + w, y + h);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x, y + h);
	glEnd();
	imageMaskShader->Unbind();
	changedSinceDisplay = true;
}

void RenderTarget::drawTexture(const sf::Texture &tex, float x, float y) {
	setDrawingMode(DM_Textures);
	int h = tex.GetHeight();
	if (drawToWorldViewOn) h = -h;
	int w = tex.GetWidth();
	tex.Bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x, y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x + w, y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x + w, y + h);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x, y + h);
	glEnd();
	changedSinceDisplay = true;
}

void RenderTarget::lock() {
	if (!lockBuffer) {
		lockBuffer = new sf::Image;
	}
	display();
	*lockBuffer = target.GetTexture().CopyToImage();
}

void RenderTarget::unlock() {
	if (lockBuffer) {
		sf::Texture temp;
		temp.LoadFromImage(*lockBuffer);
		target.Clear(sf::Color(0, 0, 0, 0));
		drawTexture(temp, 0, 0);
		delete lockBuffer;
		lockBuffer = 0;
	}
}

int32_t RenderTarget::getPixel2(int x, int y) {
	if (!lockBuffer) {
		FIXME("getPixel2: Buffer isn't locked");
		return 0;
	}
	sf::Color c = lockBuffer->GetPixel(x, y);
	return ((int)c.a << 24) + ((int)c.r << 16) + ((int)c.g << 8) + (int)c.b;
}

sf::Color colorFromPixel(int pixel) {
	return sf::Color((pixel << 8) >> 24, (pixel << 16) >> 24, (pixel << 24) >> 24, pixel >> 24);
}

void RenderTarget::putPixel2(int x, int y, int pixel) {
	if (!lockBuffer) {
		FIXME("putPixel2: Buffer isn't locked");
		return;
	}
	lockBuffer->SetPixel(x, y, colorFromPixel(pixel));
}
