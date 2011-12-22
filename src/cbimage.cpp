#include "cbimage.h"
#include "cbenchanted.h"
CBImage::CBImage():hotspotX(0),hotspotY(0),image(0),renderTargetPointer(&renderTexture)
{

}

bool CBImage::load(const string &path)
{
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

void CBImage::Render(sf::RenderTarget &target, sf::Renderer &renderer) const
{
    glEnable(GL_TEXTURE_2D);
    renderer.SetTexture(&renderTexture.GetTexture());
    const sf::Vector2f pos(GetPosition());
    renderer.Begin(sf::Renderer::QuadList);
    if (CBEnchanted::instance()->getDrawImageToWorld())
    {
        renderer.AddVertex(hotspotX+pos.x,hotspotY+pos.y,0.0f,1.0f);
        renderer.AddVertex(hotspotX+pos.x+renderTexture.GetWidth(),hotspotY+pos.y,1.0f,1.0f);
        renderer.AddVertex(hotspotX+pos.x+renderTexture.GetWidth(),hotspotY+pos.y-renderTexture.GetHeight(),1.0f,0.0f);
        renderer.AddVertex(hotspotX+pos.x,hotspotY+pos.y-renderTexture.GetHeight(),0.0f,0.0f);
    }
    else {
        renderer.AddVertex(hotspotX+pos.x,hotspotY+pos.y,0.0f,1.0f);
        renderer.AddVertex(hotspotX+pos.x+renderTexture.GetWidth(),hotspotY+pos.y,1.0f,1.0f);
        renderer.AddVertex(hotspotX+pos.x+renderTexture.GetWidth(),hotspotY+pos.y+renderTexture.GetHeight(),1.0f,0.0f);
        renderer.AddVertex(hotspotX+pos.x,hotspotY+pos.y+renderTexture.GetHeight(),0.0f,0.0f);
    }
    renderer.End();


    glDisable(GL_TEXTURE_2D);
}
