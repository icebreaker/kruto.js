#include "kruto/sandbox/canvas.hpp"
#include "kruto/sandbox/console.hpp"
#include "kruto/sandbox/image.hpp"

#include <math.h>

#include <SDL/SDL.h>
#include <GL/gl.h>

#define KR_CENTER_WINDOW

#ifndef KR_VSYNC
	#define KR_VSYNC 1
#endif

#ifndef KR_CANVAS_COLOR
	#define KR_CANVAS_COLOR 1.0f
#endif

#ifndef KR_MSAA
	#define KR_MSAA 4
#endif

namespace kruto {

bool Canvas::setVideoMode(const unsigned int pWidth, const unsigned int pHeight, const bool pFullScreen, const char *pColor)
{
	int flags = SDL_OPENGL;

	if(pFullScreen)
		flags |= SDL_FULLSCREEN;

#ifdef KR_CENTER_WINDOW
	#if __GNUC__ >= 4
		#if __GNUC_MINOR__ >= 6
			#pragma GCC diagnostic push
		#endif
		#pragma GCC diagnostic ignored "-Wwrite-strings"
	#endif
	SDL_putenv("SDL_VIDEO_WINDOW_POS");
	SDL_putenv("SDL_VIDEO_CENTERED=1");
	#if __GNUC__ >= 4
		#if __GNUC_MINOR__ >= 6
			#pragma GCC diagnostic pop
		#else
			#pragma GCC diagnostic warning "-Wwrite-strings"
		#endif
	#endif
#endif

#if defined(KR_MSAA) && KR_MSAA > 0
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, KR_MSAA);
#endif

#ifdef KR_VSYNC
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, KR_VSYNC);
#endif

	if(SDL_SetVideoMode(pWidth, pHeight, 0, flags) == NULL)
	{
		Console::log("[canvas] Failed to set videomode %dx%d: %s", pWidth, pHeight, SDL_GetError());
		return false;
	}
	else
	{
		Console::log("[canvas] size %dx%d", pWidth, pHeight);	 
	}

#if defined(KR_MSAA) && KR_MSAA > 0
	glEnable(GL_MULTISAMPLE);
#endif

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	 
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, pWidth, pHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, pWidth, pHeight, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Canvas::setClearColor(pColor != NULL ? pColor : "");
	
	return true;
}

bool Canvas::isInitialized(void)
{
	return SDL_GetVideoSurface() != NULL;
}

void Canvas::begin(void)
{
#ifdef KR_QUIRKS_MODE
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
#endif
}

void Canvas::end(void)
{
	glFinish();
	SDL_GL_SwapBuffers();
}

void Canvas::save(void)
{
	glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT | GL_TEXTURE_BIT);
	glPushMatrix();
}

void Canvas::restore(void)
{
	glPopMatrix();
	glPopAttrib(); 
}

void Canvas::scale(const double pX, const double pY)
{
	glScaled(pX, pY, 0.0);
}

void Canvas::rotate(const double pAngle)
{
	glRotated(pAngle * 180.0 / M_PI, 0.0, 0.0, 1.0);
}

void Canvas::translate(const double pX, const double pY)
{
	glTranslated(pX, pY, 0.0);
}

void Canvas::transform(const double pM11, const double pM12, const double pM21, const double pM22, const double pX, const double pY)
{
	const double m[16] =
	{
		pM11, pM21, pX, 0,
		pM12, pM22, pY, 0,
		   0,    0,  1, 0,
		   0,    0,  0, 1
	};
	glMultMatrixd(m);
}

void Canvas::setTransform(const double pM11, const double pM12, const double pM21, const double pM22, const double pX, const double pY)
{
	const double m[16] = 
	{
		pM11, pM21, pX, 0,
		pM12, pM22, pY, 0,
		   0,    0,  1, 0,
		   0,    0,  0, 1
	};
	glLoadMatrixd(m);
}

// TODO: we can make this better :)
#define KR_COLOR(c, i) reinterpret_cast<unsigned char *>(&c)[i]
#define KR_CR(c) KR_COLOR(c, 2)
#define KR_CG(c) KR_COLOR(c, 1)
#define KR_CB(c) KR_COLOR(c, 0)
#define KR_C1(c) \
do \
{\
	unsigned int color = strtoul(c + 1, NULL, 16); \
	glColor3ub(KR_CR(color), KR_CG(color), KR_CB(color)); \
} while(0)
#define KR_C2(c) \
do \
{ \
	int r = 0, g = 0, b = 0; float a = 0; \
	if(sscanf(c, "rgba(%d,%d,%d,%f)", &r, &g, &b, &a) == 4) \
		glColor4ub(r, g, b, a * 255); \
} while(0)
void Canvas::setStrokeStyle(const char *pColor)
{
	if(*pColor == '#') // #rgb
		KR_C1(pColor);
	else if(*pColor == 'r') // rgba(r,g,b,a)
		KR_C2(pColor);
}

void Canvas::setFillStyle(const char *pColor)
{
	if(*pColor == '#') // #rgb
		KR_C1(pColor);
	else if(*pColor == 'r') // rgba(r,g,b,a)
		KR_C2(pColor);
}

void Canvas::setClearColor(const char *pColor)
{
	float r = KR_CANVAS_COLOR, g = KR_CANVAS_COLOR, b = KR_CANVAS_COLOR, a = 1.0f;

	if(*pColor == '#') // #rgb
	{
		unsigned int color = strtoul(pColor+1, NULL, 16);
		r = KR_CR(color) / 255.0f;
		g = KR_CG(color) / 255.0f;
		b = KR_CB(color) / 255.0f;
	}
	else if(*pColor == 'r') // rgba(r,g,b,a)
	{
		int rr = r, gg = g, bb = b;
		if(sscanf(pColor, "rgba(%d,%d,%d,%f)", &rr, &gg, &bb, &a) == 4)
		{
			r = rr / 255.0f;
			g = rr / 255.0f;
			b = rr / 255.0f;
		}
	}

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}
#undef KR_COLOR
#undef KR_CR
#undef KR_CG
#undef KR_CB
#undef KR_C1
#undef KR_C2
void Canvas::setLineWidth(const double pW)
{
	glLineWidth(pW);
}

#define KR_RECT(x, y, w, h) \
	glVertex2d(x,   y); \
	glVertex2d(x+w, y); \
	glVertex2d(x+w, y+h); \
	glVertex2d(x,   y+h)

void Canvas::fillRect(const double pX, const double pY, const double pW, const double pH)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX, pY, pW, pH);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void Canvas::strokeRect(const double pX, const double pY, const double pW, const double pH)
{
	int lw = 0; double hlw = 0.0;
	glGetIntegerv(GL_LINE_WIDTH, &lw);
	hlw = lw / 2.0;

	glDisable(GL_TEXTURE_2D);
	
	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX, pY-hlw, pW, lw);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX+pW, pY-hlw, lw, pH);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX, pY+pH-hlw, pW+lw, lw);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX, pY-hlw, lw, pH);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}
#undef KR_RECT

void Canvas::clearRect(const double pX, const double pY, const double pW, const double pH)
{
	int vp[4] = {0,};
	glGetIntegerv(GL_VIEWPORT, vp);

	glEnable(GL_SCISSOR_TEST);
	glScissor(pX, vp[3] - pH - pY, pW, pH); // invert Y axis
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}

void Canvas::beginPolygon(const bool pFill)
{
	if(pFill)
		glBegin(GL_POLYGON);
	else
		glBegin(GL_LINE_LOOP);
}

void Canvas::drawPoint(const double pX, const double pY)
{
	glVertex2d(pX, pY);
}

void Canvas::endPolygon(void)
{
	glEnd();	 
}

void Canvas::drawImage(const Image *pImage, const double pX, const double pY)
{
	int w = pImage->width(), h = pImage->height();

	glBindTexture(GL_TEXTURE_2D, pImage->textureId());
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2i(0, 0); glVertex2d(pX,   pY  );
		glTexCoord2i(1, 0); glVertex2d(pX+w, pY  );
		glTexCoord2i(1, 1); glVertex2d(pX+w, pY+h);
		glTexCoord2i(0, 1); glVertex2d(pX,   pY+h);
	glEnd(); 
}

void Canvas::drawImage(const Image *pImage, const double pX, const double pY, const double pW, const double pH)
{
	glBindTexture(GL_TEXTURE_2D, pImage->textureId());
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2i(0, 0); glVertex2d(pX,    pY  );
		glTexCoord2i(1, 0); glVertex2d(pX+pW, pY  );
		glTexCoord2i(1, 1); glVertex2d(pX+pW, pY+pH);
		glTexCoord2i(0, 1); glVertex2d(pX,    pY+pH);
	glEnd();
}

void Canvas::drawImage(const Image *pImage, const double pSX, const double pSY, const double pSW, const double pSH, const double pX, const double pY, const double pW, const double pH)
{
	double w = pImage->widthd(), h = pImage->heightd();

	double sx = pSX * w;
	double sy = pSY * h;

	double sw = pSW * w;
	double sh = pSH * h;

	glBindTexture(GL_TEXTURE_2D, pImage->textureId());
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2d(sx,	sy	 );	glVertex2d(pX,    pY   );
		glTexCoord2d(sx+sw,	sy	 );	glVertex2d(pX+pW, pY   );
		glTexCoord2d(sx+sw,	sy+sh); glVertex2d(pX+pW, pY+pH);
		glTexCoord2d(sx,	sy+sh); glVertex2d(pX,    pY+pH);
	glEnd();
}

}
