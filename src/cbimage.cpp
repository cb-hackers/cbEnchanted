#include "cbimage.h"
#include "cbenchanted.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
CBImage::CBImage():hotspotX(0),hotspotY(0),image(0),renderTargetPointer(&renderTexture)
{

}

CBImage::~CBImage() {
    if (image) delete image;

}

bool CBImage::load(const string &path) {
    sf::Texture temp;
    if (!temp.LoadFromFile(path))
    {
        return false;
    }
    renderTexture.Create(temp.GetWidth(),temp.GetHeight());
    sf::Sprite tempsprite(temp);
    renderTexture.Draw(tempsprite); //Draws loaded texture
    renderTexture.Display();
    return true;
}

void CBImage::Draw(sf::RenderTarget &target, sf::RenderStates states) const{
    glEnable(GL_TEXTURE_2D);
    renderTexture.GetTexture().Bind();

    glBegin(GL_QUADS);
    if (CBEnchanted::instance()->getDrawImageToWorld())
    {
        glTexCoord2f(0.0f,1.0f);
        glVertex2f(hotspotX+drawingPos.x,hotspotY+drawingPos.y);
        glTexCoord2f(1.0f,1.0f);
        glVertex2f(hotspotX+drawingPos.x+renderTexture.GetWidth(),hotspotY+drawingPos.y);
        glTexCoord2f(1.0f,0.0f);
        glVertex2f(hotspotX+drawingPos.x+renderTexture.GetWidth(),hotspotY+drawingPos.y-renderTexture.GetHeight());
        glTexCoord2f(0.0f,0.0f);
        glVertex2f(hotspotX+drawingPos.x,hotspotY+drawingPos.y-renderTexture.GetHeight());
    }
    else {
        glTexCoord2f(0.0f,1.0f);
        glVertex2f(hotspotX+drawingPos.x,hotspotY+drawingPos.y);
        glTexCoord2f(1.0f,1.0f);
        glVertex2f(hotspotX+drawingPos.x+renderTexture.GetWidth(),hotspotY+drawingPos.y);
        glTexCoord2f(1.0f,0.0f);
        glVertex2f(hotspotX+drawingPos.x+renderTexture.GetWidth(),hotspotY+drawingPos.y+renderTexture.GetHeight());
        glTexCoord2f(0.0f,0.0f);
        glVertex2f(hotspotX+drawingPos.x,hotspotY+drawingPos.y+renderTexture.GetHeight());
    }
    glEnd();
}

void CBImage::makeImage(int32_t w, int32_t h)
{
    renderTexture.Create(w,h);
}
