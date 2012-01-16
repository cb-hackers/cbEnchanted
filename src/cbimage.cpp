#include "cbimage.h"
#include "cbenchanted.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
CBImage::CBImage():hotspotX(0),hotspotY(0),renderTarget()
{

}

CBImage::~CBImage() {

}

bool CBImage::load(const string &path) {
    sf::Texture temp;
    if (!temp.LoadFromFile(path))
    {
        return false;
    }
    renderTarget.create(temp.GetWidth(),temp.GetHeight());
    sf::Sprite tempsprite(temp);
    renderTarget.draw(tempsprite); //Draws loaded texture
    renderTarget.display();
    return true;
}

void CBImage::draw(float x, float y) {
    CBEnchanted::instance()->getCurrentRenderTarget()->drawRenderTarget(renderTarget,x-hotspotX,y-hotspotY);
}

void CBImage::makeImage(int32_t w, int32_t h)
{
    renderTarget.create(w,h);
    renderTarget.clear(sf::Color(0,0,0));
}
