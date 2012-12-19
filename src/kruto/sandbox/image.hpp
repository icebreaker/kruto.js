#ifndef KRIMAGE_HPP
#define KRIMAGE_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace kruto {

class Image
{
public:
	Image(void);
	~Image(void);

	void setFileName(const char *pFileName);
	const char *fileName(void) const;

	bool load(void);
	void free(void);

	bool isLoaded(void) const;

	int width(void) const;
	double widthd(void) const;
	
	int height(void) const;
	double heightd(void) const;		

	int bpp(void) const;

	bool hasAlpha(void) const;

	unsigned int textureId(void) const;

public:
	static bool initialize(void);
	static void uninitialize(void);

private:
	SDL_Surface *mSurface;
	double mWidthD;
	double mHeightD;
	unsigned int mTextureId;
	char mFileName[256];
};

}

#endif
