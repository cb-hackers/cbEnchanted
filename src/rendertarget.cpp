#include "rendertarget.h"
#include "cbenchanted.h"
#include <SFML/Graphics/RenderTexture.hpp>
#define CIRCLE_VERTEX_COUNT 100
float circleVertexArray[(CIRCLE_VERTEX_COUNT+2)*2];
class CircleVertexArrayConstructDumbClass {
public:
    CircleVertexArrayConstructDumbClass()
    {
        circleVertexArray[0] = circleVertexArray[1] = 0;
        int index = 2;

        float theta = 2 * 3.1415926 / float(CIRCLE_VERTEX_COUNT);
        float c = cosf(theta);
        float s = sinf(theta);
        float t;
        float x = 1.0f;
        float y = 0;

        for(int ii = 0; ii < CIRCLE_VERTEX_COUNT; ii++) {
            circleVertexArray[index++] = x;
            circleVertexArray[index++] = y;
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }
        circleVertexArray[index++] = x;
        circleVertexArray[index++] = y;
    }
};
CircleVertexArrayConstructDumbClass circleVertexArrayConstructDumbClass;


RenderTarget::RenderTarget():defaultRenderState(sf::BlendAlpha)
{
    drawToWorldViewOn = false;
}

void RenderTarget::create(int w, int h)
{
    if (!target.Create(w,h)) {
        FIXME("Creating RenderTarget failed. Can't create RenderTexture.");
        return;
    }
    target.Clear();
    target.ResetGLStates();
    changedSinceDisplay = true;
    openGLDrawMode = false;
}

void RenderTarget::create(const sf::Texture &texture)
{
    if (!target.Create(texture.GetWidth(),texture.GetHeight())) {
        FIXME("Creating RenderTarget failed. Can't create RenderTexture.");
        return;
    }
    target.Clear(sf::Color(0,0,0,0));
    target.ResetGLStates();
    sf::Sprite temp(texture);
    target.Draw(temp);
    openGLDrawMode = false;
    changedSinceDisplay = true;
}

void RenderTarget::setup(){
    target.SetActive(true);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glBindTexture(GL_TEXTURE_2D,0);
    glMatrixMode(GL_PROJECTION);
    glViewport(0,0,target.GetWidth(),target.GetHeight());
    glLoadMatrixf(target.GetView().GetTransform().GetMatrix());
    glMatrixMode(GL_MODELVIEW);
    glColor4ub(CBEnchanted::instance()->getDrawColor().r,CBEnchanted::instance()->getDrawColor().r,CBEnchanted::instance()->getDrawColor().g,CBEnchanted::instance()->getDrawColor().b);
    glLoadIdentity();
    openGLDrawMode = true;
}

void RenderTarget::setViewTo(bool drawtoworld) {
    if (drawToWorldViewOn == drawtoworld) return; //Useless to update view
    if (drawtoworld) {
        sf::View view;
        view.SetSize(target.GetWidth(),target.GetHeight());
        view.SetCenter(CBEnchanted::instance()->getCameraX(),CBEnchanted::instance()->getCameraY());
        view.SetRotation(180); //Flip y
        target.SetView(view);
        drawToWorldViewOn = true;
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(target.GetView().GetTransform().GetMatrix());
        glMatrixMode(GL_MODELVIEW);
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
    target.Draw(d);
    changedSinceDisplay = true;
}

void RenderTarget::draw(const sf::Vertex *vertices, unsigned int vertexCount, sf::PrimitiveType type, const sf::RenderStates &states)
{
    target.Draw(vertices,vertexCount,type,states);
    changedSinceDisplay = true;
}

void RenderTarget::clear(const sf::Color &c)
{
    target.Clear(c);
    changedSinceDisplay = true;
}

void RenderTarget::display()
{
    if (changedSinceDisplay) target.Display();
    changedSinceDisplay = false;
}

void RenderTarget::drawLine(float x1, float y1, float x2, float y2)
{
    enableOpenGLDrawMode();
    /*sf::Vertex vertices[2];
    vertices[0].Position.x = x1;
    vertices[0].Position.y = y1;
    vertices[0].Color = color;
    vertices[1].Position.x = x2;
    vertices[1].Position.y = y2;
    vertices[1].Color = color;
    target.Draw(vertices,2,sf::Lines);*/

    glBegin(GL_LINES);
        glVertex2f(x1,y1);
        glVertex2f(x2,y2);
    glEnd();
    changedSinceDisplay = true;
}

void RenderTarget::drawCircle(float cx, float cy, float r, bool fill)
{
    enableOpenGLDrawMode();
    int vertexCount = 3+r;
    glTranslatef(cx,cy,0);
    glScalef(r,r,1.0);
    if (fill) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,0);
    }
    else
        glBegin(GL_LINE_LOOP);

    float theta = 2 * 3.1415926 / float(vertexCount);
    float c = cosf(theta);
    float s = sinf(theta);
    float t;
    float x = 1.0f;
    float y = 0;

    for(int ii = 0; ii < vertexCount; ii++) {
        glVertex2f(x,y);
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    if (fill)
    {
        glVertex2f(x,y);
    }
    glEnd();
    glLoadIdentity();
    changedSinceDisplay = true;
    /*
    glEnableClientState(GL_VERTEX_ARRAY);
    glTranslatef(cx,cy,0);
    glScalef(r,r,1.0f);
    glVertexPointer(2,GL_FLOAT,0,circleVertexArray);
    if (fill)
        glDrawArrays(GL_TRIANGLE_FAN,0,CIRCLE_VERTEX_COUNT+2);
    else
        glDrawArrays(GL_LINE_LOOP,1,CIRCLE_VERTEX_COUNT);
    glDisableClientState(GL_VERTEX_ARRAY);*/

}

void RenderTarget::drawBox(float x, float y, float w, float h, bool fill)
{
    enableOpenGLDrawMode();
    if (drawToWorldViewOn) h=-h;
    if (fill)
        glBegin(GL_QUADS);
    else
        glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+w,y);
    glVertex2f(x+w,y+h);
    glVertex2f(x,y+h);
    glEnd();
}

void RenderTarget::drawEllipse(float cx, float cy, float w, float h, bool fill)
{
    enableOpenGLDrawMode();
    int vertexCount = 3+(w+h)/2;
    glTranslatef(cx,cy,0);
    glScalef(w,h,1.0f);
    if (fill) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,0);
    }
    else
        glBegin(GL_LINE_LOOP);

    float theta = 2 * 3.1415926 / float(vertexCount);
    float c = cosf(theta);
    float s = sinf(theta);
    float t;
    float x = 1.0f;
    float y = 0;

    for(int ii = 0; ii < vertexCount; ii++) {
        glVertex2f(x,y);
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    if (fill)
    {
        glVertex2f(x,y);
    }
    glEnd();
    glLoadIdentity();
    changedSinceDisplay = true;
    /*glEnableClientState(GL_VERTEX_ARRAY);
    glTranslatef(cx,cy,0);
    glScalef(w,h,1.0f);
    glVertexPointer(2,GL_FLOAT,0,circleVertexArray);
    if (fill)
        glDrawArrays(GL_TRIANGLE_FAN,0,CIRCLE_VERTEX_COUNT);
    else
        glDrawArrays(GL_LINE_LOOP,1,CIRCLE_VERTEX_COUNT);
    glDisableClientState(GL_VERTEX_ARRAY);*/

}

void RenderTarget::drawDot(float x, float y)
{
    enableOpenGLDrawMode();
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
    changedSinceDisplay = true;
}

void RenderTarget::drawRenderTarget(const RenderTarget &rt, float x, float y) {
    /*enableSFMLDrawMode();
    sf::Sprite sprite(rt.getSurface()->GetTexture());

    sprite.SetPosition(x,y);
    target.Draw(sprite,defaultRenderState);*/
    enableOpenGLDrawMode();
    int h = rt.height();
    if (drawToWorldViewOn) h = -h;
    int w = rt.width();
    glEnable(GL_TEXTURE_2D);
    rt.getSurface()->GetTexture().Bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);
    glVertex2f(x,y);
    glTexCoord2f(1.0f,1.0f);
    glVertex2f(x+w,y);
    glTexCoord2f(1.0f,0.0f);
    glVertex2f(x+w,y+h);
    glTexCoord2f(0.0f,0.0f);
    glVertex2f(x,y+h);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_TEXTURE_2D);
    changedSinceDisplay = true;
}

void RenderTarget::enableOpenGLDrawMode()
{
    if (!openGLDrawMode) {setup();openGLDrawMode = true;}
}

void RenderTarget::enableSFMLDrawMode()
{
    if (openGLDrawMode) {target.ResetGLStates();openGLDrawMode = false;}
}
