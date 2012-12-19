#ifndef KRCANVAS_HPP
#define KRCANVAS_HPP

#include <stdio.h>

namespace kruto {

class Image;
class Canvas
{
public:
	static bool setVideoMode(const unsigned int pWidth, const unsigned int pHeight, const bool pFullScreen=false, const char *pColor=NULL);
	static bool isInitialized(void);

	static void begin(void);
	static void end(void);

	static void save(void);
	static void restore(void);
	static void scale(const double pX, const double pY);
	static void rotate(const double angle);
	static void translate(const double pX, const double pY);
	static void transform(const double pM11, const double pM12, const double pM21, const double pM22, const double pX, const double pY);
	static void setTransform(const double pM11, const double pM12, const double pM21, const double pM22, const double pX, const double pY);
	static void setStrokeStyle(const char *pColor);
	static void setFillStyle(const char *pColor);
	static void setClearColor(const char *pColor);
	static void setLineWidth(const double pW);
	static void fillRect(const double pX, const double pY, const double pW, const double pH);
	static void strokeRect(const double pX, const double pY, const double pW, const double pH);
	static void clearRect(const double pX, const double pY, const double pW, const double pH);

	static void beginPolygon(const bool pFill=true);
	static void drawPoint(const double pX, const double pY);
	static void endPolygon(void);

	static void drawImage(const Image *pImage, const double pX, const double pY);
	static void drawImage(const Image *pImage, const double pX, const double pY, const double pW, const double pH);
	static void drawImage(const Image *pImage, const double pSX, const double pSY, const double pSW, const double pSH, const double pX, const double pY, const double pW, const double pH);
};

}

#endif
