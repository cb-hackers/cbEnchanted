#include "drawingprimitives.h"
#ifdef WIN32
	#include <Windows.h>
	#include <GL/GL.h>
#else
	#include <GL/gl.h>
#endif

void Line::Render(sf::RenderTarget& target,sf::Renderer& renderer) const{
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
}

void Circle::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
	float theta = 2 * 3.1415926 / float(CIRCLE_SEGMENT_COUNT); 
	float c = cosf(theta);
	float s = sinf(theta);
	float t;
	float x = r;
	float y = 0; 
	if (fill)
	{
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(cx,cy);
	}
	else
		glBegin(GL_LINE_LOOP); 
	for(int ii = 0; ii != CIRCLE_SEGMENT_COUNT; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);

		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	if (fill) glVertex2f(cx +r,cy);
	glEnd();
}